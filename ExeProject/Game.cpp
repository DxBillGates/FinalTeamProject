#include "Game.h"
#include "SampleScene.h"
#include "Clear.h"
#include "Over.h"
#include "DashModeScene.h"
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Graphics/CBufferStruct.h>
#include <GatesEngine/Header/GUI/GUIManager.h>
#include <GatesEngine/Header/Graphics/FbxLoader.h>
#include <GatesEngine/Header/Graphics/Texture.h>
#include "PlayerComponent.h"
#include "Option.h"
#include "Title.h"
#include "InputManager.h"

Game::Game()
	: Application()
{
}

Game::Game(const GE::WindowData& windowData, const GE::Math::Vector2& resolution)
	: Application(windowData, resolution)
{

}

Game::Game(const GE::Math::Vector2& size, const GE::Math::Vector2& resolution, const std::string& title, GE::WindowMode mode)
	: Application(size, resolution, title, mode)
{
}

Game::~Game()
{
}

bool Game::LoadContents()
{
	Application::LoadContents();
	using namespace GE;
	auto* device = graphicsDevice.GetDevice();
	auto* cmdList = graphicsDevice.GetCmdList();
	auto* shaderResourceHeap = graphicsDevice.GetShaderResourceHeap();

	auto* meshManager = graphicsDevice.GetMeshManager();
	Mesh* mesh;

	std::string resourcesFolder = "Resources/";
	std::string modelFolder = "Model/";
	std::string objModelFolder = "objModel/";
	std::string fbxModelFolder = "fbxModel/";

	std::string objModelPath = resourcesFolder + modelFolder + objModelFolder;
	std::string fbxModelPath = resourcesFolder + modelFolder + fbxModelFolder;

	MeshData<Vertex_UV_Normal> modelDataNest;
	MeshCreater::LoadObjModelData("Resources/Model/nest", modelDataNest);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataNest);
	meshManager->Add(mesh, "Nest");

	MeshData<Vertex_UV_Normal> modelTree1;
	MeshCreater::LoadObjModelData("Resources/Model/tree1", modelTree1);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelTree1);
	meshManager->Add(mesh, "tree1");

	MeshData<Vertex_UV_Normal> modelTreeLeaf2;
	MeshCreater::LoadObjModelData("Resources/Model/tree_leaf2", modelTreeLeaf2);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelTreeLeaf2);
	meshManager->Add(mesh, "Tree_Leaf2");

	MeshData<Vertex_UV_Normal> modelTreeLeaf3;
	MeshCreater::LoadObjModelData("Resources/Model/tree_leaf3", modelTreeLeaf3);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelTreeLeaf3);
	meshManager->Add(mesh, "Tree_Leaf3");
	
	MeshData<Vertex_UV_Normal> modelGroundLeaf2;
	MeshCreater::LoadObjModelData("Resources/Model/ground_leaf1_b", modelGroundLeaf2);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelGroundLeaf2);
	meshManager->Add(mesh, "Ground_Leaf2");

	MeshData<Vertex_UV_Normal> modelFrog;
	MeshCreater::LoadObjModelData("Resources/Model/frog", modelFrog);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelFrog);
	meshManager->Add(mesh, "modelFrog");

	MeshData<Vertex_UV_Normal> modelDragonfly;
	MeshCreater::LoadObjModelData("Resources/Model/Dragonfly", modelDragonfly);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDragonfly);
	meshManager->Add(mesh, "modelDragonfly");

	meshManager->Add(FbxLoader::Load("Bird", &graphicsDevice), "Player");
	meshManager->Add(FbxLoader::Load("hina", &graphicsDevice), "Hina");
	meshManager->Add(FbxLoader::Load("Frog", &graphicsDevice), "Frog");
	meshManager->Add(FbxLoader::Load("Dragonfly", &graphicsDevice), "Dragonfly");

	//効果音hit_wall
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/hit_wall.wav"), "hitWall");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "hitWall"));
		audio->SetVolume(0.3f);
		audio = audioManager.AddAudio(new GE::Audio(audioData, "hitWall"));
		audio->SetVolume(0.3f);
	}
	//効果音air
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/air.wav"), "air1");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "air1"));
		audio->SetVolume(0.3f);
	}
	//効果音catch
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/catch2.wav"), "catch2");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "catch2"));
		audio->SetVolume(0.3f);
		audio = audioManager.AddAudio(new GE::Audio(audioData, "catch2"));
		audio->SetVolume(0.3f);
		audio = audioManager.AddAudio(new GE::Audio(audioData, "catch2"));
		audio->SetVolume(0.3f);
	}
	//効果音flapping
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/flapping.wav"), "flapping1");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "flapping1"));
		audio->SetVolume(0.3f);
		audio = audioManager.AddAudio(new GE::Audio(audioData, "flapping1"));
		audio->SetVolume(0.3f);
	}
	//BGM2
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/BGM2.wav"), "BGM2");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "BGM2"));
		audio->SetVolume(0.05f);
	}
	//jag
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/jag.wav"), "jag");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "jag"));
		audio->SetVolume(0.3f);
		audio = audioManager.AddAudio(new GE::Audio(audioData, "jag"));
		audio->SetVolume(0.3f);
		audio = audioManager.AddAudio(new GE::Audio(audioData, "jag"));
		audio->SetVolume(0.3f);
	}
	//効果音hine
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/hine.wav"), "hine1");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "hine1"));
		audio->SetVolume(0.2f);
	}
	//効果音click
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/click.wav"), "click");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "click"));
		audio->SetVolume(0.4f);
		audio = audioManager.AddAudio(new GE::Audio(audioData, "click"));
		audio->SetVolume(0.4f);
		audio = audioManager.AddAudio(new GE::Audio(audioData, "click"));
		audio->SetVolume(0.4f);
	}
	//効果音natsunoyama1
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/natsunoyama1.wav"), "natsunoyama1");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "natsunoyama1"));
		audio->SetVolume(0.2f);
	}
	//gameover
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/gameover.wav"), "gameover");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "gameover"));
		audio->SetVolume(0.3f);
	}
	//gameover
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/coin3.wav"), "coin3");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "coin3"));
		audio->SetVolume(0.2f);
	}
	//gameclear
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/gameclear.wav"), "gameclear");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "gameclear"));
		audio->SetVolume(0.2f);
	}
	////rank_in
	//{
	//	auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/rank_in.wav"), "rank_in");
	//	auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "rank_in"));
	//	audio->SetVolume(0.5f);
	//}

	auto* textureManager = graphicsDevice.GetTextureManager();
	Texture* nullTexture = new Texture();
	nullTexture->Load("title.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_title");

	nullTexture = new Texture();
	nullTexture->Load("start.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_start");

	nullTexture = new Texture();
	nullTexture->Load("stage2.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_stage2");

	nullTexture = new Texture();
	nullTexture->Load("option.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_option");

	nullTexture = new Texture();
	nullTexture->Load("exit.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_exit");

	nullTexture = new Texture();
	nullTexture->Load("next.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_next");

	nullTexture = new Texture();
	nullTexture->Load("Number.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_Number");

	nullTexture = new Texture();
	nullTexture->Load("symbol.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_symbol");

	nullTexture = new Texture();
	nullTexture->Load("Game_Clear.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_clear");

	nullTexture = new Texture();
	nullTexture->Load("Game_Over.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_over");

	nullTexture = new Texture();
	nullTexture->Load("Chick.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_Chick");

	nullTexture = new Texture();
	nullTexture->Load("crash_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "crash_info");

	nullTexture = new Texture();
	nullTexture->Load("texture_tree_leaf.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_tree_leaf");

	nullTexture = new Texture();
	nullTexture->Load("groundTex1.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "groundTex1");

	nullTexture = new Texture();
	nullTexture->Load("texture_ground_leaf1.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "leafTex1");
	
	nullTexture = new Texture();
	nullTexture->Load("groundTex2.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "groundTex2");

	nullTexture = new Texture();
	nullTexture->Load("back.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_back");

	nullTexture = new Texture();
	nullTexture->Load("control_info_1.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "control_info_1_tex");

	nullTexture = new Texture();
	nullTexture->Load("lockon_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "lockon_info_tex");

	nullTexture = new Texture();
	nullTexture->Load("crash_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "crash_info_tex");

	nullTexture = new Texture();
	nullTexture->Load("control_info_2.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "control_info_2_tex");

	nullTexture = new Texture();
	nullTexture->Load("push_b.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "push_b_tex");

	nullTexture = new Texture();
	nullTexture->Load("bgm_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "bgm_info_tex");

	nullTexture = new Texture();
	nullTexture->Load("se_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "se_info_tex");
	
	nullTexture = new Texture();
	nullTexture->Load("take_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "take_info_tex");

	nullTexture = new Texture();
	nullTexture->Load("control_info_1_keyboard.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "control_info_keyboard_tex");

	nullTexture = new Texture();
	nullTexture->Load("push_space.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "push_space_tex");

	nullTexture = new Texture();
	nullTexture->Load("search_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "search_info_tex");

	nullTexture = new Texture();
	nullTexture->Load("is_lockon_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "is_lockon_info_tex");

	nullTexture = new Texture();
	nullTexture->Load("lockon_info_keyboard.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "lockon_info_keyboard_tex");
	
	nullTexture = new Texture();
	nullTexture->Load("crash_info_keyboard.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "crash_info_keyboard_tex");
	
	nullTexture = new Texture();
	nullTexture->Load("keyboard_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "keyboard_info_tex");
	
	nullTexture = new Texture();
	nullTexture->Load("endless.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "endless_tex");

	nullTexture = new Texture();
	nullTexture->Load("Enemy_MiniMap.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "Enemy_MiniMap");

	nullTexture = new Texture();
	nullTexture->Load("Enemy2_MiniMap.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "Enemy2_MiniMap");

	nullTexture = new Texture();
	nullTexture->Load("MiniMap.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "MiniMap");

	nullTexture = new Texture();
	nullTexture->Load("Player_MiniMap.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "Player_MiniMap");

	nullTexture = new Texture();
	nullTexture->Load("combo_tex.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "combo_tex");
	
	nullTexture = new Texture();
	nullTexture->Load("kurai.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "kurai_tex");
	
	nullTexture = new Texture();
	nullTexture->Load("ranking_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "ranking_info_tex");
	
	nullTexture = new Texture();
	nullTexture->Load("title_dashmode.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "title_dashmode_tex");

	auto* testScene = sceneManager.AddScene(new SampleScene("SampleScene", sceneManager.GetSceneInitializer()));
	sceneManager.AddScene(new Clear("ClearScene", sceneManager.GetSceneInitializer()));
	sceneManager.AddScene(new Over("OverScene", sceneManager.GetSceneInitializer()));
	sceneManager.AddScene(new DashModeScene("DashModeScene", sceneManager.GetSceneInitializer()));
	sceneManager.ChangeScene("SampleScene");

	nullTexture = new Texture();
	nullTexture->Load("keyboard.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "keyboard_tex");

	nullTexture = new Texture();
	nullTexture->Load("xinput.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "xinput_tex");

	nullTexture = new Texture();
	nullTexture->Load("joycon.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "joycon_tex");

	nullTexture = new Texture();
	nullTexture->Load("control_info_1_xctrl.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "control_info_xctrl_tex");

	nullTexture = new Texture();
	nullTexture->Load("crash_info_xctrl.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "clash_info_xctrl_tex");

	nullTexture = new Texture();
	nullTexture->Load("lockon_info_xctrl.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "lockon_info_xctrl_tex");

	nullTexture = new Texture();
	nullTexture->Load("push_a.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "push_a_tex");

	nullTexture = new Texture();
	nullTexture->Load("plus.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "plus_tex");

	nullTexture = new Texture();
	nullTexture->Load("go_nest_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "go_nest_info_tex");

	nullTexture = new Texture();
	nullTexture->Load("lockon_release_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "lockon_release_info_tex");

	nullTexture = new Texture();
	nullTexture->Load("indicatorArrow.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_indicatorArrow");

	return true;
}

bool Game::Initialize()
{
	Application::Initialize();

	// fpsの表示切替
	timer.SetIsShow(false);
	timer.SetFrameRate(60);

	float gaussValue = 2.5f;
	GE::Math::Vector2 right = { 1,0 };
	GE::Math::Vector2 up = { 0,1 };
	GE::Math::Vector2 windowSize = GE::Window::GetWindowSize();
	for (int i = 0; i < 6; ++i)
	{
		GE::Math::Vector2 vector = {};
		if (i % 2 == 0)
		{
			windowSize /= 2;
			vector = right;
		}
		else
		{
			vector = up;
		}

		GE::Math::SetGaussFilterData(windowSize, vector, gaussValue, &gaussFilterData[i], 0);

		gaussValue *= 2.0f;
	}

	sceneManager.GetCurrentScene()->IsChangeScene().sceneTransitionFadein.SetFlag(true);
	sceneColor = 0;
	resultBlurFlagControler.Initialize();
	return true;
}

bool Game::Update()
{
	beforeState = PlayerComponent::statas;
	GE::GUIManager::StartFrame();
	Application::Update();
	currentState = PlayerComponent::statas;

	if (beforeState != PlayerComponent::PlayerStatas::OVER && currentState == PlayerComponent::PlayerStatas::OVER)
	{
		resultBlurFlagControler.Initialize();
		resultBlurFlagControler.SetFlag(true);
		resultBlurFlagControler.SetMaxTimeProperty(1);
	}

	if (resultBlurFlagControler.GetOverTimeTrigger())
	{
		resultBlurFlagControler.Initialize();
		resultBlurFlagControler.SetTime(1);
	}

	resultBlurFlagControler.Update(timer.GetElapsedTime());

	GE::Scene* currentScene = sceneManager.GetCurrentScene();
	GE::ChangeSceneInfo sceneInfo = currentScene->IsChangeScene();

	if (currentScene->GetSceneName() == "SampleScene")
	{
		resultBlurFlagControler.Initialize();
	}

	if (sceneInfo.sceneTransitionFadein.GetFlag())
	{
		sceneColor = sceneInfo.sceneTransitionFadein.GetTime();
	}
	else
	{
		sceneColor = 1 - sceneManager.GetCurrentScene()->IsChangeScene().sceneTransitionFadeout.GetTime();
	}
#ifdef _DEBUG
	ImGui::Begin("Debug");
	ImGui::Text("FPS : %.3f", 1.0f / timer.GetElapsedTime());
	ImGui::End();
#endif // _DEBUG

	audioManager.Get("testBGM", 0)->SetVolume(sceneColor * OptionData::BGM_vol / 10.f);


	if (Title::GetInstance()->isExit == true)return false;
	return true;
}

bool Game::Draw()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice.GetCBufferAllocater();

	graphicsDevice.SetCurrentRenderQueue(true);
	GE::RenderQueue* renderQueue = graphicsDevice.GetRenderQueue();

	graphicsDevice.ClearDefaultRenderTarget(GE::Color::Blue());
	//graphicsDevice.SetDefaultRenderTarget();

	graphicsDevice.ClearLayer("resultLayer");
	graphicsDevice.ClearLayer("effectLayer");
	graphicsDevice.SetLayer("resultLayer");

	graphicsDevice.SetCurrentRenderQueue(false);
	graphicsDevice.SetLayer("resultLayer");
	graphicsDevice.SetCurrentRenderQueue(true);

	graphicsDevice.SetShaderResourceDescriptorHeap();
	graphicsDevice.ResetCBufferAllocater();
	graphicsDevice.SetShader("DefaultMeshShader");

	{
		GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ 25 });
		const GE::CameraInfo& cameraInfo = mainCamera->GetCameraInfo();
		GE::Material material;
		GE::DirectionalLightInfo directionalLight;

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material, sizeof(GE::Material)) });
		renderQueue->AddSetConstantBufferInfo({ 3,cbufferAllocater->BindAndAttachData(3, &directionalLight, sizeof(GE::DirectionalLightInfo)) });
		graphicsDevice.DrawMesh("Sphere");

		graphicsDevice.SetShader("DefaultLineShader");
		modelMatrix = GE::Math::Matrix4x4::Identity();
		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		graphicsDevice.DrawMesh("Grid");

		//Application::Draw();
		sceneManager.Draw();
	}

	graphicsDevice.ExecuteRenderQueue();
	graphicsDevice.ExecuteCommands();

#pragma region 輝度抽出
	graphicsDevice.SetCurrentRenderQueue(true);
	graphicsDevice.SetShaderResourceDescriptorHeap();
	graphicsDevice.ClearLayer("brightnessLayer");
	graphicsDevice.SetLayer("brightnessLayer");
	graphicsDevice.SetShader("BrightnessSamplingShader");
	auto windowSize = GE::Window::GetWindowSize();
	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ windowSize.x,windowSize.y,0 });
	windowSize.x /= 2;
	windowSize.y /= 2;
	modelMatrix *= GE::Math::Matrix4x4::Translate({ windowSize.x,windowSize.y,0 });
	GE::Material material;
	material.color = GE::Color::White();

	GE::CameraInfo cameraInfo;
	cameraInfo = graphicsDevice.GetMainCamera()->GetCameraInfo();
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	GE::TextureAnimationInfo textureAnimationInfo;
	GE::IRenderTexture* brightnessLayerRenderTexture = graphicsDevice.GetLayerManager()->Get("brightnessLayer")->GetRenderTexture();
	textureAnimationInfo.clipSize = brightnessLayerRenderTexture->GetSize();
	textureAnimationInfo.textureSize = textureAnimationInfo.clipSize;

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4,&textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	static float BRIGHTNESS = 0;
#ifdef _DEBUG
	ImGui::Begin("Debug");
	ImGui::DragFloat("BrightnessSamplingValue", &BRIGHTNESS, 0.01f, 0, 1);
	ImGui::End();
#endif // _DEBUG
	renderQueue->AddSetConstantBufferInfo({ 5,cbufferAllocater->BindAndAttachData(5,&BRIGHTNESS,sizeof(float)) });
	renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("effectLayer")->GetRenderTexture()->GetSRVNumber() });
	graphicsDevice.DrawMesh("2DPlane");

	graphicsDevice.ExecuteRenderQueue();
	graphicsDevice.ExecuteCommands();
#pragma endregion

#pragma region ブラー
	for (int i = 0; i < 6; ++i)
	{
		graphicsDevice.SetShaderResourceDescriptorHeap();
		graphicsDevice.ClearLayer("BloomLayer_" + std::to_string(i));
		graphicsDevice.SetLayer("BloomLayer_" + std::to_string(i));
		graphicsDevice.SetShader("GaussBlurShader");

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material, sizeof(GE::Material)) });
		renderQueue->AddSetConstantBufferInfo({ 5,cbufferAllocater->BindAndAttachData(5,&gaussFilterData[i],sizeof(GE::Math::GaussFilterData)) });

		if (i == 0)
		{
			renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("brightnessLayer")->GetRenderTexture()->GetSRVNumber() });
		}
		else
		{
			renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("BloomLayer_" + std::to_string(i - 1))->GetRenderTexture()->GetSRVNumber() });
		}

		graphicsDevice.DrawMesh("2DPlane");
		graphicsDevice.ExecuteRenderQueue();
		graphicsDevice.ExecuteCommands();
	}
#pragma endregion

#pragma region 被写界深度用ブラー
	for (int i = 0; i < 6; ++i)
	{
		graphicsDevice.SetShaderResourceDescriptorHeap();
		graphicsDevice.ClearLayer("DofLayer_" + std::to_string(i));
		graphicsDevice.SetLayer("DofLayer_" + std::to_string(i));
		graphicsDevice.SetShader("GaussBlurShader");

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material, sizeof(GE::Material)) });
		renderQueue->AddSetConstantBufferInfo({ 5,cbufferAllocater->BindAndAttachData(5,&gaussFilterData[i],sizeof(GE::Math::GaussFilterData)) });

		if (i == 0)
		{
			renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("resultLayer")->GetRenderTexture()->GetSRVNumber() });
		}
		else
		{
			renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("DofLayer_" + std::to_string(i - 1))->GetRenderTexture()->GetSRVNumber() });
		}

		graphicsDevice.DrawMesh("2DPlane");
		graphicsDevice.ExecuteRenderQueue();
		graphicsDevice.ExecuteCommands();
	}
#pragma endregion


	// ブラーした結果を元画像に合成
	graphicsDevice.SetShaderResourceDescriptorHeap();
	//graphicsDevice.SetDefaultRenderTarget();
	graphicsDevice.ClearLayer("defaultLayer");
	graphicsDevice.SetLayer("defaultLayer");
	graphicsDevice.SetShader("MixedTextureShader");


	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material, sizeof(GE::Material)) });

	static GE::Math::Vector3 dofInfo = { 0.62f,1.3f,-0.1f };
#ifdef _DEBUG
	ImGui::Begin("Debug");
	ImGui::DragFloat3("dofInfo", dofInfo.value, 0.001f);
	ImGui::End();
#endif // _DEBUG
	GE::Math::Vector4 dof = { dofInfo.x,dofInfo.y ,dofInfo.z ,1 };
	renderQueue->AddSetConstantBufferInfo({ 13,cbufferAllocater->BindAndAttachData(15, &dof, sizeof(GE::Math::Vector4)) });
	renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("resultLayer")->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 17,graphicsDevice.GetLayerManager()->Get("effectLayer")->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 18,graphicsDevice.GetLayerManager()->Get("BloomLayer_" + std::to_string(1))->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 19,graphicsDevice.GetLayerManager()->Get("BloomLayer_" + std::to_string(3))->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 20,graphicsDevice.GetLayerManager()->Get("BloomLayer_" + std::to_string(5))->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 21,graphicsDevice.GetLayerManager()->Get("resultLayer")->GetDepthTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 22,graphicsDevice.GetLayerManager()->Get("DofLayer_" + std::to_string(1))->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 23,graphicsDevice.GetLayerManager()->Get("DofLayer_" + std::to_string(5))->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 24,graphicsDevice.GetLayerManager()->Get("effectLayer")->GetDepthTexture()->GetSRVNumber() });
	graphicsDevice.DrawMesh("2DPlane");

	graphicsDevice.ExecuteRenderQueue();
	graphicsDevice.ExecuteCommands();

	//graphicsDevice.SetCurrentRenderQueue(false);
	//graphicsDevice.SetLayer("defaultLayer");
	//graphicsDevice.SetShaderResourceDescriptorHeap();
	//sceneManager.LateDraw();
	//graphicsDevice.ExecuteRenderQueue();
	//graphicsDevice.ExecuteCommands();

	// ブラーした結果を元画像に合成
	graphicsDevice.SetCurrentRenderQueue(true);
	graphicsDevice.SetShaderResourceDescriptorHeap();

	graphicsDevice.SetLayer("MainLayer");
	graphicsDevice.SetShader("volumetricCloudShader");

	struct RayInfo
	{
		GE::Math::Vector3 boundMin;
		float pad;
		GE::Math::Vector3 boundMax;
		float pad2;
		float samplingValue = 0.001f;
		float threshold = 0;
		GE::Math::Vector2 center = { 0.5f };
	};

	static RayInfo rayInfo;
	rayInfo.center = sceneManager.GetCurrentScene()->GetBlurUV();
	//rayInfo.threshold = sceneManager.GetCurrentScene()->GetBlurThreshold();
	//rayInfo.samplingValue = sceneManager.GetCurrentScene()->GetBlurSampling();
#ifdef _DEBUG
	//static GE::Math::Vector3 pos = { 0,0,0 };
	//static GE::Math::Vector3 scale = 2000;
	//ImGui::DragFloat3("CloudPos", pos.value, 1.0f);
	//ImGui::DragFloat3("CloudScale", scale.value, 1.0f);
	//rayInfo.boundMin = pos - scale / 2;
	//rayInfo.boundMax = pos + scale / 2;
	//ImGui::Text("color : %.3f", sceneColor);
	ImGui::Begin("Debug");
	ImGui::DragFloat("SamplingValue", &rayInfo.samplingValue, 0.01f, 0, 1);
	ImGui::DragFloat("Threshold", &rayInfo.threshold, 0.01f, 0, 100);
	ImGui::DragFloat2("uv", rayInfo.center.value, 0.01f, 0, 1);
	ImGui::End();

#endif // _DEBUG
	material.color = { sceneColor,sceneColor ,sceneColor,1 };

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material, sizeof(GE::Material)) });

	renderQueue->AddSetConstantBufferInfo({ 13,cbufferAllocater->BindAndAttachData(13, &rayInfo, sizeof(RayInfo)) });

	renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("defaultLayer")->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 17,graphicsDevice.GetLayerManager()->Get("resultLayer")->GetDepthTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 18,graphicsDevice.GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });
	graphicsDevice.DrawMesh("2DPlane");
	graphicsDevice.ExecuteRenderQueue();
	graphicsDevice.ExecuteCommands();

	for (int i = 0; i < 2; ++i)
	{
		graphicsDevice.SetShaderResourceDescriptorHeap();
		graphicsDevice.ClearLayer("FinalBlurLayer_" + std::to_string(i));
		graphicsDevice.SetLayer("FinalBlurLayer_" + std::to_string(i));
		graphicsDevice.SetShader("GaussBlurShader");

		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material, sizeof(GE::Material)) });
		renderQueue->AddSetConstantBufferInfo({ 5,cbufferAllocater->BindAndAttachData(5,&gaussFilterData[i],sizeof(GE::Math::GaussFilterData)) });

		if (i == 0)
		{
			renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("MainLayer")->GetRenderTexture()->GetSRVNumber() });
		}
		else
		{
			renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("FinalBlurLayer_" + std::to_string(i - 1))->GetRenderTexture()->GetSRVNumber() });
		}

		graphicsDevice.DrawMesh("2DPlane");
		graphicsDevice.ExecuteRenderQueue();
		graphicsDevice.ExecuteCommands();
	}

	YamadaPostEffect2();

	graphicsDevice.SetShaderResourceDescriptorHeap();
	graphicsDevice.SetCurrentRenderQueue(false);
	graphicsDevice.SetLayer("finalPostEffectLayer");
	sceneManager.LateDraw();
	InputManager::GetInstance()->Draw(&graphicsDevice);
	graphicsDevice.ExecuteRenderQueue();
	graphicsDevice.ExecuteCommands();

	YamadaPostEffect();

#ifdef _DEBUG
	//graphicsDevice.SetLayer("MainLayer");
	graphicsDevice.SetCurrentRenderQueue(false);
	graphicsDevice.SetDefaultRenderTarget();
	graphicsDevice.SetShaderResourceDescriptorHeap();
	graphicsDevice.GetRenderQueue()->AddCommand();
	graphicsDevice.ExecuteRenderQueue();

	ImGui::Begin("Game");
	ImVec2 texSize = ImGui::GetWindowSize();
	texSize.x -= 20;
	texSize.y -= 25;
	ImGui::Image((ImTextureID)graphicsDevice.GetLayerManager()->Get("resultLayer")->GetRenderTexture()->GetGPUHandle().ptr,texSize);
	ImGui::End();
#endif // _DEBUG

	GE::GUIManager::EndFrame();
	graphicsDevice.ExecuteCommands();
	graphicsDevice.ScreenFlip();
	return true;
}

void Game::YamadaPostEffect()
{
#pragma region いじらんといて〜
	auto windowSize = GE::Window::GetWindowSize();
	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ windowSize.x,windowSize.y,0 });
	windowSize.x /= 2;
	windowSize.y /= 2;
	modelMatrix *= GE::Math::Matrix4x4::Translate({ windowSize.x,windowSize.y,0 });
	GE::Material material;
	material.color = { sceneColor,sceneColor ,sceneColor ,1 };

	GE::CameraInfo cameraInfo;
	cameraInfo = graphicsDevice.GetMainCamera()->GetCameraInfo();
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	GE::TextureAnimationInfo textureAnimationInfo;
	textureAnimationInfo.clipSize = { 1,1 };
	textureAnimationInfo.textureSize = { 1,1 };

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice.GetCBufferAllocater();
	graphicsDevice.SetCurrentRenderQueue(true);
	GE::RenderQueue* renderQueue = graphicsDevice.GetRenderQueue();
	graphicsDevice.SetShaderResourceDescriptorHeap();

#ifdef _DEBUG
	graphicsDevice.SetLayer("resultLayer");
#else
	graphicsDevice.SetDefaultRenderTarget();
#endif // _DEBUG
	graphicsDevice.SetShader("SpriteTextureForPosteffectShader");
	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material, sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4,&textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("finalPostEffectLayer")->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 17,graphicsDevice.GetLayerManager()->Get("finalPostEffectLayer")->GetRenderTexture()->GetSRVNumber() });
#pragma endregion

	//// 5 ~ 13まで自由にシェーダーに構造体遅れる
	//renderQueue->AddSetConstantBufferInfo({ 5,cbufferAllocater->BindAndAttachData(5, &lerpValue, sizeof(float)) });
	////// 17 ~ 31まで自由にテクスチャセットできる
	//renderQueue->AddSetShaderResource({ 17,graphicsDevice.GetLayerManager()->Get("FinalBlurLayer_1")->GetDepthTexture()->GetSRVNumber()});

	graphicsDevice.DrawMesh("2DPlane");

	graphicsDevice.ExecuteRenderQueue();
	graphicsDevice.ExecuteCommands();
}

void Game::YamadaPostEffect2()
{
#pragma region いじらんといて〜
	auto windowSize = GE::Window::GetWindowSize();
	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ windowSize.x,windowSize.y,0 });
	windowSize.x /= 2;
	windowSize.y /= 2;
	modelMatrix *= GE::Math::Matrix4x4::Translate({ windowSize.x,windowSize.y,0 });
	GE::Material material;
	material.color = GE::Color::White();

	GE::CameraInfo cameraInfo;
	cameraInfo = graphicsDevice.GetMainCamera()->GetCameraInfo();
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	GE::TextureAnimationInfo textureAnimationInfo;
	textureAnimationInfo.clipSize = { 1,1 };
	textureAnimationInfo.textureSize = { 1,1 };

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice.GetCBufferAllocater();
	graphicsDevice.SetCurrentRenderQueue(true);
	GE::RenderQueue* renderQueue = graphicsDevice.GetRenderQueue();
	graphicsDevice.SetShaderResourceDescriptorHeap();

	graphicsDevice.SetLayer("finalPostEffectLayer");
	graphicsDevice.SetShader("SpriteTextureForPosteffectShader");
	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material, sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4,&textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("MainLayer")->GetRenderTexture()->GetSRVNumber() });
#pragma endregion

	float lerpValue = 0;
	lerpValue = resultBlurFlagControler.GetTime();

	// 5 ~ 13まで自由にシェーダーに構造体遅れる
	renderQueue->AddSetConstantBufferInfo({ 5,cbufferAllocater->BindAndAttachData(5, &lerpValue, sizeof(float)) });
	//// 17 ~ 31まで自由にテクスチャセットできる
	renderQueue->AddSetShaderResource({ 17,graphicsDevice.GetLayerManager()->Get("FinalBlurLayer_1")->GetRenderTexture()->GetSRVNumber() });

	graphicsDevice.DrawMesh("2DPlane");

	graphicsDevice.ExecuteRenderQueue();
	graphicsDevice.ExecuteCommands();
}
