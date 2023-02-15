#include "DashModeScene.h"
#include "PlayerComponent.h"
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >

#include"MiniMapViewer.h"
#include"UIObject.h"

#include <GatesEngine/Header/GameFramework/Component/DirectionalLight.h>

#include"ScreenUI.h"
#include"FieldObjectDeBugTransform.h"
#include"FieldObjectManager.h"
#include"EnemyManager.h"
#include"TimeLimit.h"
#include"Ranking.h"
#include"InputManager.h"
#include <GatesEngine/External/DirectXTex/DirectXTex.h>
#include "PlayerColectObject.h"
#include <GatesEngine/External/imgui/imgui.h>

DashModeScene::DashModeScene()
	:DashModeScene("DashModeSceneScene")
{
}

DashModeScene::DashModeScene(const std::string& sceneName)
	: Scene(sceneName)
{
}

DashModeScene::DashModeScene(const std::string& sceneName, const GE::SceneInitializer& initializer)
	: Scene(sceneName, initializer)
{
}

DashModeScene::~DashModeScene()
{
}

void DashModeScene::Initialize()
{
	gameObjectManager.Awake();
	gameObjectManager.Start();
	//directionalLight->GetAngle() = { 10,190 };
	ScreenUIManager::GetInstance()->DashModeStart();
	UIObject::GetInstance()->DashModeStart();
	Ranking::GetInstance()->Start();

	audioManager->Get("natsunoyama1", 0)->Stop();
	audioManager->Get("BGM2", 0)->Reset();

	comboFlagControler.Initialize();
}

void DashModeScene::Update(float deltaTime)
{
	int beforeCombo = PlayerComponent::combo;

	audioManager->Get("BGM2", 0)->Start();

	gameObjectManager.Update(deltaTime);
	collisionManager.Update();

	int currentCombo = PlayerComponent::combo;

	// LCtr + S で敵の位置保存
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LCONTROL) &&
		inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::S))
	{
		EnemyManager::GetInstance()->SaveCurrentPosition("Resources/dashModeEnemies.txt");
	}

	FieldObjectManager::GetInstance()->OtherUpdate();
	FieldObjectDebugTransform::GetInstance()->Update();
	TimeLimit::GetInstance()->Update(audioManager);
	ScreenUIManager::GetInstance()->DashModeUpdate(deltaTime);
	UIObject::GetInstance()->DashModeUpdate(deltaTime);

	bool enemyDead = true;
	for (int i = 0; i < EnemyManager::GetInstance()->GetAllEnemies().size(); i++)
	{
		if (EnemyManager::GetInstance()->GetAllEnemies()[i]->GetComponent<Enemy>()->statas == Enemy::Statas::ALIVE)
		{
			enemyDead = false;
		}
	}

	if ((PlayerComponent::statas == PlayerComponent::PlayerStatas::OVER && InputManager::GetInstance()->GetActionButton()) ||
		enemyDead)
	{
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.flag = true;
		changeSceneInfo.initNextSceneFlag = true;
		PlayerComponent::dashMode = false;
	}
	if (TimeLimit::GetInstance()->GetTimeOver() && PlayerComponent::statas != PlayerComponent::PlayerStatas::OVER)
	{
		PlayerComponent::statas = PlayerComponent::PlayerStatas::OVER;
		//スコアセット
		Ranking::GetInstance()->SetRanking(PlayerComponent::takeEnemyCount);

	}
	GE::Math::Matrix4x4 viewPort, proj, view;
	auto pos = ImGui::GetWindowPos();
	auto size = ImGui::GetWindowSize();
	GE::Math::Vector2 windowPos;
	GE::Math::Vector2 windowSize;
#ifdef _DEBUG
	windowPos = { pos.x,pos.y };
	windowSize = { size.x,size.y };
#else
	windowPos = { 0, 0 };
	windowSize = GE::Window::GetWindowSize();
#endif // _DEBUG

	const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();

	view = cameraInfo.viewMatrix;
	proj = cameraInfo.projMatrix;
	viewPort = GE::Math::Matrix4x4::GetViewportMatrix(windowSize, windowPos);
	GE::Math::Vector3 playerScreenPosition = GE::Math::Matrix4x4::Transform(gameObjectManager.FindGameObjectWithTag("Player", "player")->GetTransform()->position, view * proj);
	playerScreenPosition -= -1;
	playerScreenPosition /= 2;
	blurUV = { playerScreenPosition.x,playerScreenPosition.y };
	blurSampling = 16;
	blurThreshold = 0.0001f;

	// コンボしたときのブラーフラグ管理系
	if (comboFlagControler.GetFlag())
	{
		blurThreshold = GE::Math::Lerp(1, 0.0001f, comboFlagControler.GetTime());
	}

	if (comboFlagControler.GetOverTimeTrigger())
	{
		comboFlagControler.Initialize();
	}

	comboFlagControler.Update(deltaTime);

	// コンボが更新されたタイミングでコンボを管理しているやつをオンにする
	if (currentCombo > beforeCombo)
	{
		comboFlagControler.Initialize();
		comboFlagControler.SetFlag(true);

		float setTime = currentCombo > 3 ? 2 : 0.5f;
		comboFlagControler.SetMaxTimeProperty(setTime);
		if (InputManager::GetInstance()->GetCurrentInputDeviceState() == InputManager::InputDeviceState::XCTRL)
		{
			if (currentCombo > 3)inputDevice->GetXCtrler()->Vibration(0.5f, 1);
		}
	}
}

void DashModeScene::Draw()
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

	FieldObjectManager::GetInstance()->OtherDraw();
	gameObjectManager.Draw();
	FieldObjectDebugTransform::GetInstance()->Draw();
	UIObject::GetInstance()->Draw(graphicsDevice);

}

void DashModeScene::LateDraw()
{
	gameObjectManager.LateDraw();
	ScreenUIManager::GetInstance()->DrawSprite(graphicsDevice);
}

void DashModeScene::Load()
{
	PlayerComponent::dashMode = true;
	FieldObjectManager::GetInstance()->LoadPosition("Resources/tree.txt");
	FieldObjectManager::GetInstance()->Start(&gameObjectManager);
	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("Player", "player"));
		auto* sampleComponent = testObject->AddComponent<PlayerComponent>();
		testObject->GetComponent<PlayerComponent>()->SetAudioManager(audioManager);
		auto* playerCollider = testObject->AddComponent < GE::SphereCollider >();
		playerCollider->SetCenter({ 0,0,0 });
		playerCollider->SetSize({ 20 });
	}
	EnemyManager::GetInstance()->LoadPosition("Resources/dashModeEnemies.txt");
	EnemyManager::GetInstance()->Start(&gameObjectManager, true);
	TimeLimit::GetInstance()->TimeSet = { 0,20 };
	TimeLimit::GetInstance()->Start(&gameObjectManager);

	collisionManager.AddTagCombination("player", "enemy");
	collisionManager.AddTagCombination("player", "frog");
	collisionManager.AddTagCombination("player", "ground");
	collisionManager.AddTagCombination("player", "StartTree");
	collisionManager.AddTagCombination("player", "nest");
	collisionManager.AddTagCombination("player", "tile");
	collisionManager.AddTagCombination("player", "fieldtree");
	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("directionalLight", "directionalLight"));
		directionalLight = testObject->AddComponent<GE::DirectionalLight>();
		//directionalLight->SetTarget(gameObjectManager.FindGameObjectWithTag("Player", "player")->GetTransform());
		testObject->GetTransform()->position = { 0,0,0 };
	}

	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("miniMap", "miniMap"));
		auto* sampleComponent = testObject->AddComponent<MiniMapViewer>();
		sampleComponent->SetPlayer(gameObjectManager.FindGameObject("Player")->GetComponent<PlayerComponent>());
	}

}

void DashModeScene::UnLoad()
{
	audioManager->Get("BGM2", 0)->Stop();
	EnemyManager::GetInstance()->UnLoad();
	FieldObjectManager::GetInstance()->UnLoad();
	FieldObjectDebugTransform::GetInstance()->UnLoad();
	UIObject::GetInstance()->UnLoad();
	PlayerColectObject::GetInstance()->ClearObject();
	ScreenUIManager::GetInstance()->UnLoad();
	// gameObjectsを削除する
	Scene::UnLoad();


}