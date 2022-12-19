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


void UIObject::Start()
{
	GE::Utility::Printf("TestTreeComponent Start()\n");
	normalUI.push_back(
		AddObject("test", FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 1000, 0), { 500 }, GE::Color(1, 1, 1, 1), "texture_Chick")
	);
	/*AnimationUI.push_back(
		AddObject("test", FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 1000, 0), { 500 }, GE::Color(1, 1, 1, 1), "texture_Number")
	);*/

}

void UIObject::Update(float deltaTime)
{
	//��ɃJ�����ɐ��ʂ�������
	for (int i = 0; i < normalUI.size(); i++)
	{
		GE::Math::Vector3 dir = cameraPosition - normalUI[i].transform.position;
		dir.y = 0.0f;
		normalUI[i].transform.rotation = GE::Math::Quaternion::LookDirection(dir.Normalize()) * GE::Math::Quaternion::Euler({ 90, 0, 180 });
	}

	//for (int i = 0; i < AnimationUI.size(); i++)
	//{
	//	GE::Math::Vector3 dir = cameraPosition - AnimationUI[i].transform.position;
	//	dir.y = 0.0f;
	//	AnimationUI[i].transform.rotation = LookDirection(dir.Normalize()) * GE::Math::Quaternion::Euler({ 90, 0, 180 });
	//}


	//	pivotPosX = StartTree::collectCount;
	//	texSizeX = 320;//�摜�T�C�Y
	//	clipSizeX = 32;
	//	
}

void UIObject::Draw(GE::IGraphicsDeviceDx12* graphicsDevice)
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithTextureShader");


	for (auto o : normalUI)
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
		GE::Math::Vector2 textureSize = { 1,1 };
		textureAnimationInfo.textureSize = textureSize;
		// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
		// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
		textureAnimationInfo.clipSize = { 1,1 };
		// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
		textureAnimationInfo.pivot = { 0,0 };
		renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });

		graphicsDevice->DrawMesh("Plane");
	}
	//for (auto a : AnimationUI)
	{
		// �A�j���[�V�����̏��
		//GE::TextureAnimationInfo textureAnimationInfo;

		//// �摜�̌��T�C�Y
		//textureAnimationInfo.textureSize = { texSizeX,64 };
		//// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
		//// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
		//textureAnimationInfo.clipSize = { clipSizeX,64 };
		//// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
		//textureAnimationInfo.pivot = { pivotPosX,0 };

		//GE::Math::Matrix4x4 modelMatrix = AnimationUI[0].transform.GetMatrix();
		//GE::Material material;
		//material.color = AnimationUI[0].color;
		//renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		//renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		//renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
		//renderQueue->AddSetShaderResource({5,graphicsDevice->GetTextureManager()->Get(AnimationUI[0].textureName)->GetSRVNumber() });

		//graphicsDevice->DrawMesh("Plane");
	}
}

void UIObject::OnGui()
{
	//ImGui::InputFloat3("Rotation_Euler", rotation_euler.value);
}

void UIObject::UnLoad()
{
	normalUI.clear();
}