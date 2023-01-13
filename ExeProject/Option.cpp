#include "Option.h"
#include"Title.h"
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include<GatesEngine/Header/Graphics/Texture.h>

int OptionData::BGM_vol = 1;
int OptionData::SE_vol = 3;
int Option::Max_vol = 5;
Option::Select Option::select;

void Option::Awake(GE::GameObjectManager* gameObjectManager)
{
	GE::Math::Vector2 pos = GE::Window::GetWindowSize();
	Create("bgm_num", "texture_Number", gameObjectManager, { pos.x / 2,pos.y / 2 }, 100, Select::BGM_VOL);
	Create("se_num", "texture_Number", gameObjectManager, { pos.x / 2,pos.y / 2 + 100 }, 100, Select::SE_VOL);
	Create("back_num", "texture_exit", gameObjectManager, { pos.x / 2,pos.y / 2 + 200 }, 100, Select::Back);
	Create("nextR", "texture_next", gameObjectManager, { pos.x / 2 - 200,pos.y / 2 }, 100, Select::volR);
	Create("nextL", "texture_next", gameObjectManager, { pos.x / 2 + 200,pos.y / 2 }, 100, Select::volL);
	select = Select::BGM_VOL;
	bgmChange = false;
	seChange = false;
}

void Option::Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager, GE::Math::Vector2 pos, float scaleX, Select handle)
{
	auto* optionObject = gameObjectManager->AddGameObject(new GE::GameObject(gui_tag, "option"));
	auto* optionComponent = optionObject->AddComponent<OptionTex>();
	optionComponent->tag = tex_tag;
	optionComponent->handle = handle;
	optionComponent->color = GE::Color::White();
	optionObject->GetTransform()->position = { pos.x,pos.y,0 };//�|�W�V�����w��
	optionObject->GetTransform()->scale = { scaleX,100,0 };//�T�C�Y�w��
}

void Option::KeySelect()
{
	int a;
	//�ύX���ڑI��
	if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::UP)
		|| GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::W)
		|| GE::InputDevice::GetInstance()->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::UP))
	{
		a = ((int)select + ((int)Select::SelectNum - 1)) % (int)Select::SelectNum;
		select = (Select)a;
	}
	else if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::DOWN)
		|| GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::S)
		|| GE::InputDevice::GetInstance()->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::DOWN))
	{
		a = ((int)select + 1) % (int)Select::SelectNum;
		select = (Select)a;
	}

	//���e
	if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::LEFT)
		|| GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::A)
		|| GE::InputDevice::GetInstance()->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::LEFT))
	{
		if (select == Select::BGM_VOL)
		{
			if (OptionData::BGM_vol > 0)
			{
				bgmChange = true;
				OptionData::BGM_vol--;
			}
		}
		else if (select == Select::SE_VOL)
		{
			if (OptionData::SE_vol > 0)
			{
				seChange = true;
				OptionData::SE_vol--;
			}
		}
	}
	else if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::RIGHT)
		|| GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::D)
		|| GE::InputDevice::GetInstance()->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::LEFT))
	{
		if (select == Select::BGM_VOL)
		{
			if (OptionData::BGM_vol < Max_vol)
			{
				OptionData::BGM_vol++;
				bgmChange = true;
			}
		}
		else if (select == Select::SE_VOL)
		{
			if (OptionData::SE_vol < Max_vol)
			{
				OptionData::SE_vol++;
				seChange = true;
			}
		}
	}

	Back();
}

void Option::Back()
{
	if (select == Select::Back)
	{
		if (GE::InputDevice::GetInstance()->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE))
		{
			Title::GetInstance()->Back();
			select = Select::BGM_VOL;
		}
	}
}

void Option::Update(GE::AudioManager* audioManager)
{
	if (!Title::GetInstance()->GetSelect(Title::States::option))return;

	KeySelect();

	//���ʒ����������Ȃ�
	if(bgmChange)
	{
		float set = OptionData::BGM_vol / 10.0f;
		//�ύX����
		audioManager->Get("testBGM", 0)->SetVolume(set);
		bgmChange = false;
	}
	if (seChange)
	{
		float set = OptionData::SE_vol / 10.0f;
		//�ύX����
		audioManager->Get("hitWall", 0)->SetVolume(set);
		audioManager->Get("hitWall", 1)->SetVolume(set);
		audioManager->Get("air1", 0)->SetVolume(set);
		audioManager->Get("catch2", 0)->SetVolume(set);
		audioManager->Get("catch2", 1)->SetVolume(set);
		audioManager->Get("catch2", 2)->SetVolume(set);
		audioManager->Get("flapping1", 0)->SetVolume(set);
		audioManager->Get("flapping1", 1)->SetVolume(set);
		seChange = false;
	}
}

void OptionTex::Start()
{
}

void OptionTex::Update(float deltaTime)
{
	color = GE::Color::White();
	switch (handle)
	{
	case Option::Select::BGM_VOL:
		pivotPosX = OptionData::BGM_vol;//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 320;//�摜�T�C�Y
		if (Option::select == Option::Select::BGM_VOL) {
			color = GE::Color::Red();
		}
		break;
	case Option::Select::SE_VOL:
		pivotPosX = OptionData::SE_vol;//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 320;//�摜�T�C�Y
		if (Option::select == Option::Select::SE_VOL) {
			color = GE::Color::Red();
		}
		break;
	case Option::Select::Back:
		pivotPosX = 0;//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 32;//�摜�T�C�Y
		if (Option::select == Option::Select::Back) {
			color = GE::Color::Red();
		}
		break;
	case Option::Select::volR:
		pivotPosX = 0;//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 64;//�摜�T�C�Y
		transform->position.y = (GE::Window::GetWindowSize().y / 2) + (int)Option::select * 100;
		break;
	case Option::Select::volL:
		pivotPosX = 0;//�摜�̕`��J�n�ʒu�̑��
		texSizeX = 64;//�摜�T�C�Y
		transform->position.y = (GE::Window::GetWindowSize().y / 2) + (int)Option::select * 100;
		transform->rotation = GE::Math::Quaternion::Euler({ 0, 0, 180 });
		break;
	default:
		break;
	}
}

void OptionTex::LateDraw()
{
	if (!Title::GetInstance()->GetSelect(Title::States::option))return;
	if (Option::select == Option::Select::Back
		&& (handle == Option::Select::volR || handle == Option::Select::volL))
	{
		return;
	}

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
	material.color = color;

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
	textureAnimationInfo.clipSize = { 32,64 };
	// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
	textureAnimationInfo.pivot = { pivotPosX,0 };

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,texture->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}