#include "TimeLimit.h"

#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include<GatesEngine/Header/Graphics/Texture.h>

TimeLimit* TimeLimit::GetInstance()
{
	static TimeLimit instance;
	return &instance;
}

TimeLimit::TimeLimit()
{
}

void TimeLimit::Start(GE::GameObjectManager* gameObjectManager)
{
	timer = 1;//��������(���w��)
	timer = timer * 60 * frameRate;//������b���ɒ�����144fps������
	minutes = timer / frameRate / 60;//�����̌v�Z
	tenSeconds = timer / frameRate % 60 / 10;//�b���̌v�Z(�\�̈�)
	oneSeconds = timer / frameRate % 60 % 10;//�b���̌v�Z(��̈�)
	timeOver = false;//�^�C���I�[�o�[�t���O

	//�e�e�N�X�`������
	Create("minutes", "texture_Number", gameObjectManager,80,100,0);//�����̃e�N�X�`������
	Create("symbol", "texture_symbol", gameObjectManager, 160, 50, 3);//�����ƕb���̊Ԃ̃e�N�X�`������
	Create("tenSeconds", "texture_Number", gameObjectManager, 240, 100, 1);//�b���̃e�N�X�`������(�\�̈�)
	Create("oneSeconds", "texture_Number", gameObjectManager, 340, 100, 2);//�b���̃e�N�X�`������(��̈�)
}

void TimeLimit::Update()
{
	if (timer <= 0)
	{
		timeOver = true;
	}
	else
	{
		timer--;//�J�E���g�_�E��
		minutes = timer / frameRate / 60;//�����̌v�Z
		tenSeconds = timer / frameRate % 60 / 10;//�b���̌v�Z(�\�̈�)
		oneSeconds = timer / frameRate % 60 % 10;//�b���̌v�Z(��̈�)
	}
}

void TimeLimit::AddSeconds(const int& seconds)
{
	timer += seconds * frameRate;
}

void TimeLimit::Create(const std::string& gui_tag, const std::string& tex_tag, GE::GameObjectManager* gameObjectManager, float posX, float scaleX, int timeNum)
{
	auto* timeObject = gameObjectManager->AddGameObject(new GE::GameObject(gui_tag, "time"));
	auto* timeComponent = timeObject->AddComponent<TimeTex>();
	timeComponent->tag = tex_tag;
	timeComponent->num = timeNum;//TimeTex��switch���̐؂�ւ�
	timeObject->GetTransform()->position = { posX,850,0 };//�|�W�V�����w��
	timeObject->GetTransform()->scale = { scaleX,100,0 };//�T�C�Y�w��
}

void TimeTex::Start()
{
}

void TimeTex::Update(float deltaTime)
{
	switch (num)
	{
	case (int)TimeName::minutes://�����̕`����
		pivotPosX = TimeLimit::GetInstance()->GetMinutes();//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 320;//�摜�T�C�Y
		break;
	case (int)TimeName::tenSeconds://�b���̕`����(�\�̈�)
		pivotPosX = TimeLimit::GetInstance()->GetTenSeconds();//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 320;//�摜�T�C�Y
		break;
	case (int)TimeName::oneSeconds://�b���̕`����(��̈�)
		pivotPosX = TimeLimit::GetInstance()->GetOneSeconds();//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 320;//�摜�T�C�Y
		break;
	default:
		pivotPosX = 0;//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 64;//�摜�T�C�Y
		break;
	}
}

void TimeTex::LateDraw()
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
	textureAnimationInfo.textureSize = {texSizeX,64};
	// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
	// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
	textureAnimationInfo.clipSize = {32,64};
	// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
	textureAnimationInfo.pivot = { pivotPosX,0 };

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,texture->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}