#include "Collect.h"

#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include<GatesEngine/Header/Graphics/Texture.h>

Collect* Collect::GetInstance()
{
	static Collect instance;
	return &instance;
}

void Collect::Start(GE::GameObjectManager* gameObjectManager)
{
	collectCount = 0;//���W��
	maxCollect = 5;//���W��(�ő吔)
	//�e�e�N�X�`������
	Create("chick", "texture_Chick", gameObjectManager, 80, 100, 3);//���̃e�N�X�`������
	Create("collect", "texture_Number", gameObjectManager, 190, 100, 0);//���W���̃e�N�X�`������
	Create("symbol", "texture_symbol", gameObjectManager, 270, 50, 2);//�X���b�V���̃e�N�X�`������
	Create("maxCollect", "texture_Number", gameObjectManager, 340, 100, 1);//���W��(�ő吔)�̃e�N�X�`������
}

void Collect::Update(const int& count, const int& goalCollect)
{
	collectCount = count;
	maxCollect = goalCollect;
}

void Collect::Create(const std::string& gui_tag, const std::string& tex_tag, GE::GameObjectManager* gameObjectManager, float posX, float scaleX, int collectNum)
{
	auto* collectObject = gameObjectManager->AddGameObject(new GE::GameObject(gui_tag, "collect"));
	auto* collectComponent = collectObject->AddComponent<CollectTex>();
	collectComponent->tag = tex_tag;
	collectComponent->num = collectNum;//CollectTex��switch���̐؂�ւ�
	collectObject->GetTransform()->position = { posX,950,0 };//�|�W�V�����w��
	collectObject->GetTransform()->scale = { scaleX,100,0 };//�T�C�Y�w��
}

void CollectTex::Start()
{
}

void CollectTex::Update(float deltaTime)
{
	switch (num)
	{
	case (int)CollectName::collect://���W���̕`����
		pivotPosX = Collect::GetInstance()->GetCollectCount();//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 320;//�摜�T�C�Y
		clipSizeX = 32;
		break;
	case (int)CollectName::maxCollect://���W��(�ő吔)�̕`����
		pivotPosX = Collect::GetInstance()->GetMaxCollect();//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 320;//�摜�T�C�Y
		clipSizeX = 32;
		break;
	case (int)CollectName::symbol://�X���b�V���̕`����
		pivotPosX = 1;//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 64;//�摜�T�C�Y
		clipSizeX = 32;
		break;
	default:
		pivotPosX = 0;//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 64;//�摜�T�C�Y
		clipSizeX = 64;
		break;
	}
}

void CollectTex::LateDraw()
{
	// �`�悷�邽�߂�GraphicsDevice���擾���Ƃ��悤�ɁI
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	// �����_�[�L���[��2d�p�ɐ؂�ւ���
	graphicsDevice->SetCurrentRenderQueue(false);

	// �e�N�X�`���`��p�̃V�F�[�_�[���Z�b�g
	graphicsDevice->SetShader("DefaultSpriteWithTextureShader");

	// �`��ʒu�Ƃ��T�C�Y�Ƃ��̐ݒ�
	//auto windowSize = GE::Window::GetWindowSize();
	//GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ windowSize.x,windowSize.y,0 });
	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ 0 });
	//modelMatrix *= GE::Math::Matrix4x4::Translate({ 0,0,0 });
	modelMatrix = transform->GetMatrix();

	// �摜�̐F�ς����肷��悤
	GE::Material material;
	material.color = GE::Color::White();

	// 2d�p�̃J������� ��{�I�ɔ����Ȃ���ok
	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	// �A�j���[�V�����̏��
	GE::TextureAnimationInfo textureAnimationInfo;
	GE::ITexture* texture = graphicsDevice->GetTextureManager()->Get(tag);

	// �摜�̌��T�C�Y
	textureAnimationInfo.textureSize = { texSizeX,64 };
	// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
	// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
	textureAnimationInfo.clipSize = { clipSizeX,64 };
	// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
	textureAnimationInfo.pivot = { pivotPosX,0 };

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,texture->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}
