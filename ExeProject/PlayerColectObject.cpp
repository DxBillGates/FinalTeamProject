#include "PlayerComponent.h"
#include "PlayerColectObject.h"
#include <GatesEngine/Header/Util/Random.h           >
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>
#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

PlayerColectObject* PlayerColectObject::GetInstance()
{
	static PlayerColectObject instance;
	return &instance;
}

void PlayerColectObject::Start(int colectMax, GE::GameObject* targetObject)
{
	this->targetObject = targetObject;
	fallenObject.transform.scale = { 50.f };
}

void PlayerColectObject::Update(float deltaTime)
{
	//足でつかんでいる取集物の更新
	for (int i = 0; i < colectingObjs.size(); i++)
	{
		colectingObjs[i].transform.position = targetObject->GetTransform()->position - targetObject->GetTransform()->GetUp() * colectingObjs[i].LocalPosition.y
			+ targetObject->GetTransform()->GetForward() * colectingObjs[i].LocalPosition.z
			+ targetObject->GetTransform()->GetRight() * colectingObjs[i].LocalPosition.x;

		colectingObjs[i].transform.rotation = targetObject->GetTransform()->rotation * colectingObjs[i].LocalRotation;
	}
	if (colectingObjs.size() != 0)
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

void PlayerColectObject::DrawShadow(GE::IGraphicsDeviceDx12* graphicsDevice)
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");

	GE::Material material;
	material.color = GE::Color::Red();

	//所持しているえさの描画
	for (int i = 0; i < colectingObjs.size(); i++)
	{
		GE::Math::Matrix4x4 modelMatrix;
		modelMatrix = colectingObjs[i].transform.GetMatrix();

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		graphicsDevice->DrawMesh(colectingObjs[i].modelName);

	}
	//落下するオブジェクト
	if (targetObject->GetComponent<PlayerComponent>()->statas == PlayerComponent::PlayerStatas::CRASH && colectingObjs.size() != 0)
	{
		GE::Math::Matrix4x4 modelMatrix;
		modelMatrix = fallenObject.transform.GetMatrix();

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		graphicsDevice->DrawMesh("Sphere");
	}
}

void PlayerColectObject::Draw(GE::IGraphicsDeviceDx12* graphicsDevice, GE::GameObjectManager* manager)
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithShadowShader");

	GE::Material material;
	material.color = GE::Color::Red();

	//所持しているえさの描画
	for (int i = 0; i < colectingObjs.size(); i++)
	{
		GE::Math::Matrix4x4 modelMatrix;
		modelMatrix = colectingObjs[i].transform.GetMatrix();

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });

		GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
		cameraInfo.lightMatrix = manager->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->GetVPMatrix();
		manager->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->SetDirectionalLightInfo();
		renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
		renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });

		graphicsDevice->DrawMesh(colectingObjs[i].modelName);

	}
	//落下するオブジェクト
	if (targetObject->GetComponent<PlayerComponent>()->statas == PlayerComponent::PlayerStatas::CRASH && colectingObjs.size() != 0)
	{
		GE::Math::Matrix4x4 modelMatrix;
		modelMatrix = fallenObject.transform.GetMatrix();

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });

		GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
		cameraInfo.lightMatrix = manager->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->GetVPMatrix();
		manager->FindGameObjectWithTag("directionalLight", "directionalLight")->GetComponent<GE::DirectionalLight>()->SetDirectionalLightInfo();
		renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
		renderQueue->AddSetShaderResource({ 17,graphicsDevice->GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });

		graphicsDevice->DrawMesh("Sphere");
	}
}

void PlayerColectObject::SetFallen(GE::Math::Vector3 normal)
{
	if (colectingObjs.size() == 0)return;

	velocity = normal * 40.f;
	colectingObjs.pop_back();
}

void PlayerColectObject::AddObject(std::string modelName)
{
	colectingObjs.push_back(SetColectObject({}, {}, {}, GE::Color::Red(), modelName));

}

void PlayerColectObject::ClearObject()
{
	colectingObjs.clear();
}

PlayerColectObject::ColectingObject PlayerColectObject::SetColectObject(GE::Math::Vector3 pos, GE::Math::Quaternion rot, GE::Math::Vector3 scl, GE::Color color, std::string modelName)
{
	ColectingObject result;

	result.transform.position = pos;
	result.transform.rotation = rot;
	//result.transform.scale = scl;
	result.color = color;
	result.modelName = modelName;

	GE::Math::Vector3 random = { GE::RandomMaker::GetFloat(-50.0f,50.0f),GE::RandomMaker::GetFloat(80.0f,100.0f),GE::RandomMaker::GetFloat(-100.0f,0.0f) };
	result.LocalPosition = random;
	random = { GE::RandomMaker::GetFloat(50.0f, 100.0f) };
	result.transform.scale = random;
	result.LocalRotation = GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), GE::RandomMaker::GetFloat(0.0f, 6.28f));
	return result;
}
