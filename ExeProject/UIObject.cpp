#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics\Window.h>
#include <GatesEngine/Header/GUI\GUIManager.h>

#include"UIObject.h"
#include"FieldObjectManager.h"

UIObject* UIObject::GetInstance()
{
	static UIObject instance;
	return &instance;
}

void UIObject::AddObject(std::string tag, GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color, std::string textureName)
{
	Object result;
	result.tag = tag;
	result.transform.position = positon;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	objects.push_back(result);
	int a = 0;
}


void UIObject::Start()
{
	GE::Utility::Printf("TestTreeComponent Start()\n");

}

void UIObject::Update(float deltaTime)
{
	//èÌÇ…ÉJÉÅÉâÇ…ê≥ñ Çå¸ÇØÇÈ
	for (int i = 0; i < objects.size(); i++)
	{
		GE::Math::Vector3 dir = cameraPosition - objects[i].transform.position;
		dir.y = 0.0f;
		objects[i].transform.rotation = LookDirection(dir.Normalize()) * GE::Math::Quaternion::Euler({ 90, 0, 0 });
	}
}

void UIObject::Draw(GE::IGraphicsDeviceDx12* graphicsDevice)
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithTextureShader");


	for (auto o : objects)
	{
		GE::Math::Matrix4x4 modelMatrix = o.transform.GetMatrix();
		GE::Material material;
		material.color = o.color;
		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		renderQueue->AddSetShaderResource({ 4,graphicsDevice->GetTextureManager()->Get(o.textureName)->GetSRVNumber() });

		graphicsDevice->DrawMesh("Plane");
	}
}

void UIObject::OnGui()
{
	//ImGui::InputFloat3("Rotation_Euler", rotation_euler.value);
}

void UIObject::UnLoad()
{
	objects.clear();
}


GE::Math::Quaternion UIObject::LookDirection(GE::Math::Vector3 direction)
{
	GE::Math::Vector3 dir = direction.Normalize();
	GE::Math::Vector3 forward = { 0,0,1 };
	float dot;
	float theta;
	dot = GE::Math::Vector3::Dot(forward, dir);
	theta = std::acosf(dot);
	GE::Math::Vector3 cross = GE::Math::Vector3::Cross(forward, dir).Normalize();
	GE::Math::Quaternion result = { cross,theta };
	return result;
}
