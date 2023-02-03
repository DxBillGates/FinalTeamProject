#include "Over.h"
#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>
#include <GatesEngine/Header/Util/Random.h           >
#include "FieldObjectManager.h"
#include "CameraControl.h"
#include "OverChild.h"
#include "StartTree.h"
#include "FieldObject.h"

bool Over::nowOver = false;
float Over::CameraSpeed = 0.07f;

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
	nowOver = true;
	CameraSpeed = 0.07f;

	gameObjectManager.Awake();
	gameObjectManager.Start();

	//光の方向変更
	directionalLight->GetAngle() = { 20,80 };

	FieldObjectManager::GetInstance()->LoadPosition("Resources/tree.txt");
	CameraControl::GetInstance()->SetOverCameraPosition();
}

void Over::Update(float deltaTime)
{
	//カメラコントロールの更新
	CameraControl::GetInstance()->Update(deltaTime);

	gameObjectManager.Update(deltaTime);

	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE)
		|| inputDevice->GetJoyconR()->GetTriggerButton(GE::JoyconButtonData::B))
	{
		nowOver = false;
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.flag = true;
		changeSceneInfo.initNextSceneFlag = true;
	}
}

void Over::Draw()
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

void Over::LateDraw()
{
	gameObjectManager.LateDraw();
}

void Over::Load()
{
	nowOver = true;

	auto* Object = gameObjectManager.AddGameObject(new GE::GameObject("over", "over"));
	GE::Math::Vector2 diffWindowSize = GE::Window::GetDiffWindowSize();
	Object->GetTransform()->position = { 450 * diffWindowSize.x,200 * diffWindowSize.y,0 };
	Object->GetTransform()->scale = { 800 * diffWindowSize.x,400 * diffWindowSize.y,0 };
	//Object->GetTransform()->position = { GE::Window::GetWindowSize().x / 2,GE::Window::GetWindowSize().y / 2,0 };
	//Object->GetTransform()->scale = { 700,400,0 };
	auto* clearComponent = Object->AddComponent<OverTex>();

	{
		auto* testObject = gameObjectManager.AddGameObject(new GE::GameObject("directionalLight", "directionalLight"));
		directionalLight = testObject->AddComponent<GE::DirectionalLight>();
		//directionalLight->SetTarget(gameObjectManager.FindGameObjectWithTag("Player", "player")->GetTransform());
		testObject->GetTransform()->position = { 0,0,0 };

		//CameraControl::GetInstance()->SetTargetObject(testObject);
	}

	{
		//落ちたヒナ
		for (int i = 0; i < StartTree::goalCollect; i++)
		{
			auto* object = gameObjectManager.AddGameObject(new GE::GameObject("overChild", "overChild"));
			auto* sampleComponent = object->AddComponent<OverChild>();
			//ヒナたちの設定
			if (i == 0)object->GetTransform()->position = { -91,0, -8038 };
			else if (i == 1)object->GetTransform()->position = { -1498,0, -7756.910 };
			else if (i == 2)object->GetTransform()->position = { -951,0, -8205.910 };
			else if (i == 3)object->GetTransform()->position = { -544,0, -8799.764 };
			else if (i == 4)object->GetTransform()->position = { -1321,0, -8835.764 };
			object->GetTransform()->scale = { 2 };
			float randRot = GE::RandomMaker::GetFloat(0.0f, 360.0f);
			//仰向け　それぞれランダム方向に
			object->GetTransform()->rotation = GE::Math::Quaternion::Euler({ -90,randRot,0 });

			//カメラの視点方向
			if (i == 0)CameraControl::GetInstance()->SetTargetObject(object);
		}
	}
	{
		//落ちた巣
		//auto* object = gameObjectManager.AddGameObject(new GE::GameObject("nest", "nest"));
		//auto* sampleComponent = object->AddComponent<FieldObjectComponent>();
		//object->GetComponent<FieldObjectComponent>()->modelName = "Nest";
		//object->GetComponent<FieldObjectComponent>()->shaderName = "DefaultMeshShader";
		//object->SetColor(GE::Color(0.8f, 0.6f, 0.6f, 1.f));
		//object->GetTransform()->scale = { 200,300,200 };
		//object->GetTransform()->position = { -91,0,-8038 };
	}
	FieldObjectManager::GetInstance()->Start(&gameObjectManager);
}

void Over::UnLoad()
{
	// gameObjectsを削除する
	FieldObjectManager::GetInstance()->UnLoad();
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