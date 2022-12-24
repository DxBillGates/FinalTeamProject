#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics\Window.h>
#include <GatesEngine/Header/GUI\GUIManager.h>

#include"CrashParticle.h"
#include"FieldObjectManager.h"

float CrashParticle::fireTime = 300.0f;

CrashParticle::Object CrashParticle::AddObject(GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color)
{
	Object result;
	result.transform.position = positon;
	result.transform.scale = scale;
	result.color = color;
	return result;
}

CrashParticle::Object CrashParticle::AddAnimaiotnObject(GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color)
{
	Object result;
	result.transform.position = positon;
	result.transform.scale = scale;
	result.color = color;
	return result;
}
void CrashParticle::Start()
{
	GE::Utility::Printf("TestTreeComponent Start()\n");

	count = 0.0f;
	isFire = false;
	objectsCount = 10;
	for (int i = 0; i < objectsCount; i++)
	{
		objects.push_back(
			AddObject({}, { GE::RandomMaker::GetFloat(10.0f,100.0f) }, GE::Color(1, 1, 1, 1))
		);
	}

	for (int i = 0; i < objects.size(); i++)
	{

		GE::Math::Vector3 random = { GE::RandomMaker::GetFloat(-50.0f,50.0f),GE::RandomMaker::GetFloat(-50.0f,50.0f),GE::RandomMaker::GetFloat(-50.0f,50.0f) };
		random = { GE::RandomMaker::GetFloat(0.0f,180.0f),GE::RandomMaker::GetFloat(0.0f,180.0f),GE::RandomMaker::GetFloat(0.0f,180.0f) };
		objects[i].transform.rotation = GE::Math::Quaternion::Euler(random);
		objects[i].randomPos = { GE::RandomMaker::GetFloat(-100.0f,100.0f),GE::RandomMaker::GetFloat(-100.0f,100.0f),GE::RandomMaker::GetFloat(-100.0f,100.0f) };

	}
}

void CrashParticle::Update(float deltaTime)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (isFire)
		{
			if (count < fireTime)
			{
				count++;
			}
			else
			{
				isFire = false;
				count = 0.0f;
			}
			objects[i].velocity += GE::Math::Vector3(0, -2, 0);
			objects[i].transform.position += objects[i].velocity;
		}
		else
		{

		}
	}
}
void CrashParticle::Draw(GE::IGraphicsDeviceDx12* graphicsDevice)
{
	if (!isFire)return;

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshShader");


	for (auto o : objects)
	{
		GE::Math::Matrix4x4 modelMatrix = o.transform.GetMatrix();
		GE::Material material;
		material.color = o.color;
		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });

		graphicsDevice->DrawMesh("Cube");
	}
}

void CrashParticle::Fire(GE::Math::Vector3 pos, GE::Math::Vector3 hitNormal, GE::Color color)
{
	isFire = true;
	for (auto& o : objects)
	{
		o.transform.position = pos;
		o.velocity = GE::Math::Vector3(hitNormal.Normalize() - o.randomPos / 100.f).Normalize() * 40.0f;
		o.color = color;
	}
}

void CrashParticle::OnGui()
{
	//ImGui::InputFloat3("Rotation_Euler", rotation_euler.value);
}

void CrashParticle::UnLoad()
{
	objects.clear();
}