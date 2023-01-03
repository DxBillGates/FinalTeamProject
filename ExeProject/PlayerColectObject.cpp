#include "PlayerComponent.h"
#include "PlayerColectObject.h"
#include <GatesEngine/Header/Util/Random.h           >

PlayerColectObject* PlayerColectObject::GetInstance()
{
	static PlayerColectObject instance;
	return &instance;
}

void PlayerColectObject::Start(int colectMax, GE::GameObject* targetObject)
{
	this->targetObject = targetObject;
	colect = 0;
	// 足でつかんでいるオブジェクトの初期化
	colectingObjs.resize(colectMax);
	for (int i = 0; i < colectingObjs.size(); i++)
	{
		GE::Math::Vector3 random = { GE::RandomMaker::GetFloat(-50.0f,50.0f),GE::RandomMaker::GetFloat(100.0f,120.0f),GE::RandomMaker::GetFloat(-100.0f,0.0f) };
		colectingObjs[i].LocalPosition = random;
		colectingObjs[i].transform.scale = GE::RandomMaker::GetFloat(30.0f, 60.0f);
	}
	fallenObject.transform.scale = { 50.f };
}

void PlayerColectObject::Update(float deltaTime, int colectCount)
{
	colect = colectCount;
	//足でつかんでいる取集物の更新
	for (int i = 0; i < colect; i++)
	{
		colectingObjs[i].transform.position = targetObject->GetTransform()->position - targetObject->GetTransform()->GetUp() * colectingObjs[i].LocalPosition.y
			+ targetObject->GetTransform()->GetForward() * colectingObjs[i].LocalPosition.z
			+ targetObject->GetTransform()->GetRight() * colectingObjs[i].LocalPosition.x;
	}
	if (colect != 0)
	{
		if (targetObject->GetComponent<PlayerComponent>()->statas == PlayerComponent::PlayerStatas::CRASH)
		{
			fallenObject.transform.position += velocity;
			velocity.y -= 2.f;
		}
		else
		{
			fallenObject.transform.position = targetObject->GetTransform()->position;
		}
	}
}

void PlayerColectObject::Draw(GE::IGraphicsDeviceDx12* graphicsDevice)
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	GE::Material material;
	material.color = GE::Color::Red();

	//所持しているえさの描画
	for (int i = 0; i < colect; i++)
	{
		GE::Math::Matrix4x4 modelMatrix;
		modelMatrix = colectingObjs[i].transform.GetMatrix();

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		graphicsDevice->DrawMesh("Sphere");

	}
	//落下するオブジェクト
	if (targetObject->GetComponent<PlayerComponent>()->statas == PlayerComponent::PlayerStatas::CRASH && colect != 0)
	{
		GE::Math::Matrix4x4 modelMatrix;
		modelMatrix = fallenObject.transform.GetMatrix();

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		graphicsDevice->DrawMesh("Sphere");
	}
}

void PlayerColectObject::SetFallen(GE::Math::Vector3 normal)
{
	velocity = normal * 40.f;
}

