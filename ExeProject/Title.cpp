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

	//選択項目
	states = States::start;
	stage = Stage::stage1;

	//テクスチャたちの生成、初期設定
	Create("title_start", "texture_start", gameObjectManager, device);
	sprites.back()->position = { GE::Window::GetWindowSize().x - 300,GE::Window::GetWindowSize().y / 2,0.0f };
	sprites.back()->scale = { 300,100,0 };
	//ステージ用テクスチャ追加
	textures.push_back(device->GetTextureManager()->Get("texture_stage2"));
	Create("title_option", "texture_option", gameObjectManager, device);
	sprites.back()->position = { GE::Window::GetWindowSize().x - 300,GE::Window::GetWindowSize().y / 2 + 100,0.0f };
	sprites.back()->scale = { 300,100,0 };
	Create("title_exit", "texture_exit", gameObjectManager, device);
	sprites.back()->position = { GE::Window::GetWindowSize().x - 300,GE::Window::GetWindowSize().y / 2 + 200,0.0f };
	sprites.back()->scale = { 300,100,0 };

	Create("title_name", "texture_title", gameObjectManager, device);
	sprites.back()->position = { 1100,GE::Window::GetWindowSize().y / 2 - 200.0f,0 };
	sprites.back()->scale = { 1319 / 2.0f,642 / 2.0f,0 };
	//Create("title_nextL", "texture_next", gameObjectManager, device);
	//sprites.back()->position = { 70.0f,GE::Window::GetWindowSize().y / 2,0.0f };
	//sprites.back()->scale = { 150,150,0 };
	//Create("title_nextR", "texture_next", gameObjectManager, device);
	//sprites.back()->position = { GE::Window::GetWindowSize().x - 70,GE::Window::GetWindowSize().y / 2,0.0f };
	//sprites.back()->rotation = GE::Math::Quaternion::Euler({ 0, 0, 180 });
	//sprites.back()->scale = { 150,150,0 };

	alpha = 0.0f;
	decided = false;
	option.Awake(gameObjectManager);

}

void Title::Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device)
{
	auto* titleObject = gameObjectManager->AddGameObject(new GE::GameObject(gui_tag, "title"));
	//titleObject->SetDrawAxisEnabled(true);
	auto* titleComponent = titleObject->AddComponent<TitleTex>();
	titleComponent->tag = tex_tag;
	sprites.push_back(titleComponent);

	//テクスチャ保存
	textures.push_back(device->GetTextureManager()->Get(tex_tag));
	//テクスチャ反映
	titleComponent->SetTexture(textures.back());

	//位置調整など
	//titleComponent->position = { 1170.0f,180.0f + 64 * sprites.size(),0.0f };
	//titleComponent->scale = { 300,300,0 };
}


void Title::Update(GE::AudioManager* audioManager)
{
	//タイトル状態なら、選択可能
	if (!decided)
	{
		//Select();
		alpha += 0.003f;
		//選択中のテクスチャ色変え
		sprites[(int)states]->GetGameObject()->SetColor(GE::Color::Red());
	}
	//オプション中処理
	option.Update(audioManager);
}

void Title::Select()
{
	if (decided) {
		return;
	}

	int a;
	//上下選択(スタート,オプション、exit
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::UP)
		|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::W)
		|| inputDevice->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::UP))
	{
		a = ((int)states + ((int)States::serectNum - 1)) % (int)States::serectNum;
		states = (Title::States)a;
	}
	else if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::DOWN)
		|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::S)
		|| inputDevice->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::DOWN))
	{
		a = ((int)states + 1) % (int)States::serectNum;
		states = (Title::States)a;
	}

	////左右選択(ステージ
	//if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::RIGHT)
	//	|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D)
	//	|| inputDevice->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::RIGHT))
	//{
	//	a = (stage + (Stage::stageNum - 1)) % Stage::stageNum;
	//	stage = (Title::Stage)a;
	//	sprites[Title::States::start]->SetTexture(textures[stage]);
	//}
	//else if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::LEFT)
	//	|| inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::A)
	//	|| inputDevice->GetJoyconL()->GetTriggerButton(GE::JoyconButtonData::LEFT))
	//{
	//	a = (stage + 1) % Stage::stageNum;
	//	stage = (Title::Stage)a;
	//	sprites[Title::States::start]->SetTexture(textures[stage]);
	//}

	//決定
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE)
		|| inputDevice->GetJoyconR()->GetTriggerButton(GE::JoyconButtonData::B))
	{
		switch (states)
		{
		case Title::States::start:
			switch (stage)
			{
			case Title::Stage::stage1:
				GE::Utility::Printf("Title stage1\n");
				break;
			case Title::Stage::stage2:
				GE::Utility::Printf("Title stage2\n");
				break;
			default:
				break;
			}
			break;
		case Title::States::option:
			GE::Utility::Printf("Title option()\n");
			break;
		case Title::States::exit:
			GE::Utility::Printf("Title exit()\n");
			states = States::start;
			break;
		default:
			break;
		}
		decided = true;
	}
}

bool Title::GetSelect(Title::States s)
{
	if (decided)
	{
		if (s == states)
		{
			return true;
		}
	}
	return false;
}

void Title::ClearGameObject()
{
	sprites.clear();
	textures.clear();
}


//タイトル用テクスチャ
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
	//選択していないものは白色で
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

	// アニメーションの情報
	GE::TextureAnimationInfo textureAnimationInfo;
	// 画像の元サイズ
	textureAnimationInfo.textureSize = 1;
	// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
	// textureSizeと一緒にすると切り抜かれずに描画される
	textureAnimationInfo.clipSize = 1;
	// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
	textureAnimationInfo.pivot = { 0,0 };

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,tex->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}
