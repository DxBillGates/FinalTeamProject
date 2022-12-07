#include "Over.h"
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >

Over::Over()
	:Over("OverScene")
{
}

Over::Over(const std::string& sceneName)
	: Scene(sceneName)
{
}

Over::Over(const std::string& sceneName, const GE::SceneInitializer& initializer)
	: Scene(sceneName, initializer)
{
}

Over::~Over()
{
}

void Over::Initialize()
{
	gameObjectManager.Awake();
	gameObjectManager.Start();
}

void Over::Update(float deltaTime)
{
	gameObjectManager.Update(deltaTime);

	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE)
		|| inputDevice->GetJoyconR()->GetTriggerButton(GE::JoyconButtonData::B))
	{
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.flag = true;
		changeSceneInfo.initNextSceneFlag = true;
	}
}

void Over::Draw()
{
	gameObjectManager.Draw();
}

void Over::LateDraw()
{
	gameObjectManager.LateDraw();
}

void Over::Load()
{
	auto* Object = gameObjectManager.AddGameObject(new GE::GameObject("over", "over"));
	//titleObject->SetDrawAxisEnabled(true);
	Object->GetTransform()->position = { GE::Window::GetWindowSize().x / 2,GE::Window::GetWindowSize().y / 2,0 };
	Object->GetTransform()->scale = { 700,400,0 };
	auto* clearComponent = Object->AddComponent<OverTex>();
}

void Over::UnLoad()
{
	// gameObjectsを削除する
	Scene::UnLoad();
}

void OverTex::Awake()
{
}

void OverTex::Start()
{
}

void OverTex::Update(float deltaTime)
{
}

void OverTex::LateDraw()
{
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
	renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("texture_over")->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}