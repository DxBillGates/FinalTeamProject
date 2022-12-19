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

UIObject::Object UIObject::AddObject(std::string tag, GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color, std::string textureName)
{
	Object result;
	result.tag = tag;
	result.transform.position = positon;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	return result;
}

UIObject::Object UIObject::AddAnimaiotnObject(std::string tag, GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color, std::string textureName,
	GE::Math::Vector2 texSize, GE::Math::Vector2 clipSize, GE::Math::Vector2 pivotPos)
{
	Object result;
	result.tag = tag;
	result.transform.position = positon;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	result.texSize = texSize;
	result.clipSize = clipSize;
	result.pivotPos = pivotPos;
	return result;
}


void UIObject::Start()
{
	GE::Utility::Printf("TestTreeComponent Start()\n");
	object.push_back(
		AddObject("test", FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 1000, 0), { 500 }, GE::Color(1, 1, 1, 1), "texture_Chick")
	);

	object.push_back(
		AddAnimaiotnObject("colect", FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 600, 0), { 250 }, GE::Color(1, 1, 1, 1), "texture_Number", { 320,64 }, { 32,64 })
	);
	object.push_back(
		AddAnimaiotnObject("symbol", {}, { 250 }, GE::Color(1, 1, 1, 1), "texture_symbol", { 64,64 }, { 32,64 }, { 1,0 })
	);
	object.push_back(
		AddAnimaiotnObject("colectMax", {}, { 250 }, GE::Color(1, 1, 1, 1), "texture_Number", { 320,64 }, { 32,64 }, { 1,0 })
	);
}

void UIObject::Update(float deltaTime)
{
	//��ɃJ�����ɐ��ʂ�������
	for (int i = 0; i < object.size(); i++)
	{
		GE::Math::Vector3 dir = cameraPosition - object[i].transform.position;
		dir.y = 0.0f;
		object[i].transform.rotation = GE::Math::Quaternion::LookDirection(dir.Normalize()) * GE::Math::Quaternion::Euler({ 90, 0, 180 });
	}

	//���艟��
	object[1].pivotPos = { (float)StartTree::collectCount,0 };
	object[2].transform.position = object[1].transform.position + object[1].transform.GetRight() * object[1].transform.scale.x;
	object[3].pivotPos = { (float)StartTree::goalCollect,0 };
	object[3].transform.position = object[2].transform.position + object[2].transform.GetRight() * object[2].transform.scale.x;
}

void UIObject::Draw(GE::IGraphicsDeviceDx12* graphicsDevice)
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithTextureShader");


	for (auto o : object)
	{
		GE::Math::Matrix4x4 modelMatrix = o.transform.GetMatrix();
		GE::Material material;
		material.color = o.color;
		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get(o.textureName)->GetSRVNumber() });

		// �A�j���[�V�����̏��
		GE::TextureAnimationInfo textureAnimationInfo;
		// �摜�̌��T�C�Y
		textureAnimationInfo.textureSize = o.texSize;
		// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
		// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
		textureAnimationInfo.clipSize = o.clipSize;
		// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
		textureAnimationInfo.pivot = o.pivotPos;
		renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });

		graphicsDevice->DrawMesh("Plane");
	}
}

void UIObject::OnGui()
{
	//ImGui::InputFloat3("Rotation_Euler", rotation_euler.value);
}

void UIObject::UnLoad()
{
	object.clear();
}

//���̊֐��܂��g���Ȃ��ł��B
UIObject::Object UIObject::GetTagObject(std::string tag)
{
	for (auto o : object)
	{
		if (o.tag == tag)
		{
			return o;
		}
	}
	//��v����^�O�����݂��Ȃ���
	return object[-1];
}