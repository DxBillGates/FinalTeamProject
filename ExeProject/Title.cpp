#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >

#include "Title.h"
#include"PlayerComponent.h"

Title* Title::GetInstance()
{
	static Title instance;
	return &instance;
}

void Title::Awake(GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device)
{
	GE::Utility::Printf("Title Awake()\n");

	inputDevice = GE::InputDevice::GetInstance();

	//�I������
	states = States::start;
	stage = Stage::stage1;

	//�e�N�X�`�������̐����A�����ݒ�
	Create("title_stage1", "texture_stage1", gameObjectManager, device);
	sprites.back()->position = { 1170.0f,400.0f,0.0f };
	sprites.back()->scale = { 300,300,0 };
	//�X�e�[�W�p�e�N�X�`���ǉ�
	textures.push_back(device->GetTextureManager()->Get("texture_stage2"));
	Create("title_option", "texture_option", gameObjectManager, device);
	sprites.back()->position = { 1170.0f,500.0f,0.0f };
	sprites.back()->scale = { 300,300,0 };
	Create("title_exit", "texture_exit", gameObjectManager, device);
	sprites.back()->position = { 1170.0f,600.0f,0.0f };
	sprites.back()->scale = { 300,300,0 };

	Create("title_name", "texture_title", gameObjectManager, device);
	sprites.back()->position = { 1000.0f,220.0f,0.0f };
	sprites.back()->scale = { 500,500,0 };
	Create("title_nextL", "texture_next", gameObjectManager, device);
	sprites.back()->position = { 70.0f,400.0f,0.0f };
	sprites.back()->scale = { 150,150,0 };
	Create("title_nextR", "texture_next", gameObjectManager, device);
	sprites.back()->position = { 1460.0f,400.0f,0.0f };
	sprites.back()->rotation = GE::Math::Quaternion::Euler({ 0, 0, 180 });
	sprites.back()->scale = { 150,150,0 };

	alpha = 0.0f;
}

void Title::Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device)
{
	auto* titleObject = gameObjectManager->AddGameObject(new GE::GameObject(gui_tag, "title"));
	//titleObject->SetDrawAxisEnabled(true);
	auto* titleComponent = titleObject->AddComponent<TitleTex>();
	titleComponent->tag = tex_tag;
	sprites.push_back(titleComponent);

	//�e�N�X�`���ۑ�
	textures.push_back(device->GetTextureManager()->Get(tex_tag));
	//�e�N�X�`�����f
	titleComponent->SetTexture(textures.back());

	//�ʒu�����Ȃ�
	//titleComponent->position = { 1170.0f,180.0f + 64 * sprites.size(),0.0f };
	//titleComponent->scale = { 300,300,0 };
}


void Title::Update()
{
	//�^�C�g����ԂȂ�A�I���\
	if (!decided)
	{
		Select();
		alpha += 0.003f;
	}
	//�I�𒆂̃e�N�X�`���F�ς�
	sprites[states]->GetGameObject()->SetColor(GE::Color::Red());
}

void Title::Select()
{
	int a;
	//�㉺�I��(�X�^�[�g,�I�v�V�����Aexit
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::UP)
		|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::W))
	{
		a = (states + (States::serectNum - 1)) % States::serectNum;
		states = (Title::States)a;
	}
	else if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::DOWN)
		|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::S))
	{
		a = (states + 1) % States::serectNum;
		states = (Title::States)a;
	}

	//���E�I��(�X�e�[�W
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::RIGHT)
		|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D))
	{
		a = (stage + (Stage::stageNum - 1)) % Stage::stageNum;
		stage = (Title::Stage)a;
		sprites[Title::States::start]->SetTexture(textures[stage]);
	}
	else if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::LEFT)
		|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::A))
	{
		a = (stage + 1) % Stage::stageNum;
		stage = (Title::Stage)a;
		sprites[Title::States::start]->SetTexture(textures[stage]);
	}

	//����
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE))
	{
		switch (states)
		{
		case Title::start:
			switch (stage)
			{
			case Title::stage1:
				GE::Utility::Printf("Title stage1\n");
				break;
			case Title::stage2:
				GE::Utility::Printf("Title stage2\n");
				break;
			default:
				break;
			}
			break;
		case Title::option:
			GE::Utility::Printf("Title option()\n");
			break;
		case Title::exit:
			GE::Utility::Printf("Title exit()\n");
			break;
		default:
			break;
		}
		decided = true;
	}
}

//�^�C�g���p�e�N�X�`��
void TitleTex::Awake()
{
}

void TitleTex::Start()
{
	transform->position = position;
	transform->scale = scale;
	transform->rotation = rotation;
}

void TitleTex::Update(float deltaTime)
{
	//�I�����Ă��Ȃ����͔̂��F��
	gameObject->SetColor(GE::Color::White());
}

void TitleTex::LateDraw()
{
	if (Title::GetInstance()->GetDecid())
	{
		return;
	}

	const float SPRITE_SIZE = 30;

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSpriteWithTextureShader");

	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ SPRITE_SIZE });

	//modelMatrix = GE::Math::Matrix4x4::RotationZ(0);
	modelMatrix = transform->GetMatrix();
	GE::Material material;
	material.color = gameObject->GetColor();
	//material.color.a = Title::GetInstance()->alpha;

	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,1,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	// �A�j���[�V�����̏��
	GE::TextureAnimationInfo textureAnimationInfo;
	// �摜�̌��T�C�Y
	textureAnimationInfo.textureSize = 1;
	// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
	// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
	textureAnimationInfo.clipSize = 1;
	// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
	textureAnimationInfo.pivot = { 0,0 };

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,tex->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}