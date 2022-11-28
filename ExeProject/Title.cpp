#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >

#include "Title.h"
#include"PlayerComponent.h"

Title* Title::GetInstance()
{
	static Title instance;
	return &instance;
}

void Title::Start(GE::GameObjectManager* gameObjectManager, GE::GameObject* t)
{
	GE::Utility::Printf("Title Start()\n");

	inputDevice = GE::InputDevice::GetInstance();
	targetObject = t;

	//選択項目
	states = States::start;
	stage = Stage::stage1;
	
	//テクスチャたちの生成、初期設定
	Create("title_name", "texture_title", gameObjectManager, t);
	Create("title_stage1", "texture_stage1", gameObjectManager, t);
	Create("title_option", "texture_option", gameObjectManager, t);
	Create("title_exit", "texture_exit", gameObjectManager, t);
}

void Title::Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager, GE::GameObject* t)
{
	auto* titleObject = gameObjectManager->AddGameObject(new GE::GameObject(gui_tag, "title"));
	//titleObject->SetDrawAxisEnabled(true);
	auto* titleComponent = titleObject->AddComponent<TitleTex>();
	titleComponent->SetTargetObject(t);
	titleComponent->tag = tex_tag;
	sprites.push_back(titleObject);
	//位置調整など
	titleComponent->position = { 1170.0f,180.0f + 64 * sprites.size(),0.0f };
	titleComponent->scale = { 300,300,0 };
}


void Title::Update()
{
	//タイトル状態なら、選択可能
	if (targetObject->GetComponent<PlayerComponent>()->statas
		== PlayerComponent::PlayerStatas::STAY_LAND)
	{
		Serect();
	}
	//選択中のテクスチャ色変え
	sprites[states+1]->SetColor(GE::Color::Red());
}

void Title::Serect()
{
	int a;
	//上下選択(スタート,オプション、exit
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::UP))
	{
		a = (states + (States::serectNum - 1)) % States::serectNum;
		states = (Title::States)a;
	}
	else if(inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::DOWN))
	{
		a = (states + 1) % States::serectNum;
		states = (Title::States)a;
	}

	//左右選択(ステージ
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::RIGHT))
	{
		a = (stage + (Stage::stageNum - 1)) % Stage::stageNum;
		stage = (Title::Stage)a;
	}
	else if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::LEFT))
	{
		a = (stage + 1) % Stage::stageNum;
		stage = (Title::Stage)a;
	}

	//決定
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::RETURN))
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
	}
}

//タイトル用テクスチャ
void TitleTex::Awake()
{
}

void TitleTex::Start()
{
	transform->position = position;
	transform->scale = scale;
}

void TitleTex::Update(float deltaTime)
{
	//選択していないものは白色で
	gameObject->SetColor(GE::Color::White());
}

void TitleTex::LateDraw()
{
	if (targetObject->GetComponent<PlayerComponent>()->statas
		!= PlayerComponent::PlayerStatas::STAY_LAND)
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

	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,1,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	// アニメーションの情報
	GE::TextureAnimationInfo textureAnimationInfo;
	// 画像の元サイズ
	textureAnimationInfo.textureSize = 1;
	// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
	// textureSizeと一緒にすると切り抜かれずに描画される
	textureAnimationInfo.clipSize = 1;
	// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
	textureAnimationInfo.pivot = {0,0};

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get(tag)->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}
