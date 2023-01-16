#include "Clear.h"

#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>
#include "PlayerComponent.h"
#include "FieldObjectManager.h"
#include "ClearBird.h"
#include"StartTree.h"
#include"CameraControl.h"

Clear::Clear()
	:Clear("ClearScene")
{
}

Clear::Clear(const std::string& sceneName)
	:Scene(sceneName)
{
}

Clear::Clear(const std::string& sceneName, const GE::SceneInitializer& initializer)
	: Scene(sceneName, initializer)
{
}

Clear::~Clear()
{
}

void Clear::Initialize()
{
	gameObjectManager.Awake();
	gameObjectManager.Start();

	FieldObjectManager::GetInstance()->LoadPosition("Resources/tree.txt");
}

void Clear::Update(float deltaTime)
{
	//カメラコントロールの更新
	//CameraControl::GetInstance()->Update(deltaTime);

	gameObjectManager.Update(deltaTime);

	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE)
		|| inputDevice->GetJoyconR()->GetTriggerButton(GE::JoyconButtonData::B))
	{
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.flag = true;
		changeSceneInfo.initNextSceneFlag = true;
	}
}

void Clear::Draw()
{
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::Camera* mainCamera = graphicsDevice->GetMainCamera();

	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = directionalLight->GetViewMatrix();
	cameraInfo.projMatrix = directionalLight->GetProjectionMatrix();
	cameraInfo.lightMatrix = directionalLight->GetVPMatrix();

	graphicsDevice->ClearLayer("shadowLayer");
	graphicsDevice->SetLayer("shadowLayer");
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	directionalLight->SetDirectionalLightInfo();
	gameObjectManager.DrawShadow();

	graphicsDevice->ExecuteRenderQueue();
	graphicsDevice->ExecuteCommands();

	graphicsDevice->SetShaderResourceDescriptorHeap();
	cameraInfo = mainCamera->GetCameraInfo();
	cameraInfo.lightMatrix = directionalLight->GetVPMatrix();
	graphicsDevice->SetLayer("resultLayer");
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	directionalLight->SetDirectionalLightInfo();
	gameObjectManager.Draw();
	FieldObjectManager::GetInstance()->OtherDraw();
}

void Clear::LateDraw()
{
	gameObjectManager.LateDraw();
}

void Clear::Load()
{
	auto* Object = gameObjectManager.AddGameObject(new GE::GameObject("clear", "clear"));
	//titleObject->SetDrawAxisEnabled(true);
	Object->GetTransform()->position = { GE::Window::GetWindowSize().x / 2,GE::Window::GetWindowSize().y / 2,0 };
	Object->GetTransform()->scale = { 700,400,0 };
	auto* clearComponent = Object->AddComponent<ClearTex>();

	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("directionalLight", "directionalLight"));
		directionalLight = testObject->AddComponent<GE::DirectionalLight>();
		//directionalLight->SetTarget(gameObjectManager.FindGameObjectWithTag("Player", "player")->GetTransform());
		testObject->GetTransform()->position = { 0,0,0 };

		//CameraControl::GetInstance()->SetTargetObject(testObject);
	}

	//飛び立つ鳥
	for (int i = 0; i < StartTree::goalCollect; i++)
	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("CBird", "CBird"));
		testObject->GetTransform()->position = FieldObjectManager::StartPosition;
		testObject->GetTransform()->position.z += i * 100;
		auto* sampleComponent = testObject->AddComponent<ClearBird>();
		sampleComponent->SetTarget({ 0,100,0 });
	}
	FieldObjectManager::GetInstance()->Start(&gameObjectManager);
}

void Clear::UnLoad()
{
	// gameObjectsを削除する
	FieldObjectManager::GetInstance()->UnLoad();
	Scene::UnLoad();
}

void ClearTex::Awake()
{
}

void ClearTex::Start()
{
}

void ClearTex::Update(float deltaTime)
{
}

void ClearTex::LateDraw()
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
	renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("texture_clear")->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}
