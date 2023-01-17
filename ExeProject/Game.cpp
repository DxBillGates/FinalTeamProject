#include "Game.h"
#include "SampleScene.h"
#include "Clear.h"
#include "Over.h"
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Graphics/CBufferStruct.h>
#include <GatesEngine/Header/GUI/GUIManager.h>
#include <GatesEngine/Header/Graphics/FbxLoader.h>
#include <GatesEngine/Header/Graphics/Texture.h>

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

	MeshData<Vertex_UV_Normal> modelGroundLeaf1;
	MeshCreater::LoadObjModelData("Resources/Model/ground_leaf1_a", modelGroundLeaf1);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelGroundLeaf1);
	meshManager->Add(mesh, "Ground_Leaf1");

	meshManager->Add(FbxLoader::Load("Bird", &graphicsDevice), "Player");
	meshManager->Add(FbxLoader::Load("hina", &graphicsDevice), "Hina");
	meshManager->Add(FbxLoader::Load("Frog", &graphicsDevice), "Frog");

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
	nullTexture->Load("texture_tree_leaf.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_tree_leaf");

	nullTexture = new Texture();
	nullTexture->Load("crash_info.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "crash_info");

	nullTexture = new Texture();
	nullTexture->Load("groundTex1.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "groundTex1");

	nullTexture = new Texture();
	nullTexture->Load("texture_ground_leaf1.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "leafTex1");

	nullTexture = new Texture();
	nullTexture->Load("back.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_back");

	nullTexture = new Texture();
	nullTexture->Load("gyro_shake_tex.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "gyro_shake_tex");


	auto* testScene = sceneManager.AddScene(new SampleScene("SampleScene", sceneManager.GetSceneInitializer()));
	sceneManager.AddScene(new Clear("ClearScene", sceneManager.GetSceneInitializer()));
	sceneManager.AddScene(new Over("OverScene", sceneManager.GetSceneInitializer()));
	sceneManager.ChangeScene("SampleScene");

	return true;
}

bool Game::Initialize()
{
	Application::Initialize();

	// fpsの表示切替
	timer.SetIsShow(false);

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

	return true;
}

bool Game::Update()
{
	GE::GUIManager::StartFrame();
	Application::Update();
	ImGui::Text("FPS : %.3f", 1.0f / timer.GetElapsedTime());
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

		/*graphicsDevice.SetShader("DefaultMeshWithTextureShader");
		modelMatrix = GE::Math::Matrix4x4::Identity();
		modelMatrix *= GE::Math::Matrix4x4::Scale({ 50 });
		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetShaderResource({ 4,graphicsDevice.GetTextureManager()->Get("texture_null")->GetSRVNumber() });
		graphicsDevice.DrawMesh("Skydome");*/


		graphicsDevice.SetShader("DefaultLineShader");
		modelMatrix = GE::Math::Matrix4x4::Identity();
		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		graphicsDevice.DrawMesh("Grid");

		Application::Draw();
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
	static float BRIGHTNESS = 0.7f;
	ImGui::DragFloat("BrightnessSamplingValue", &BRIGHTNESS, 0.01f, 0, 1);
	renderQueue->AddSetConstantBufferInfo({ 5,cbufferAllocater->BindAndAttachData(5,&BRIGHTNESS,sizeof(float)) });
	renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("resultLayer")->GetRenderTexture()->GetSRVNumber() });
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

	static GE::Math::Vector3 dofInfo = { 0.4f,2.0f,0.4f };
	ImGui::DragFloat3("dofInfo", dofInfo.value, 0.001f);
	GE::Math::Vector4 dof = { dofInfo.x,dofInfo.y ,dofInfo.z ,1 };
	renderQueue->AddSetConstantBufferInfo({ 13,cbufferAllocater->BindAndAttachData(15, &dof, sizeof(GE::Math::Vector4)) });
	renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("resultLayer")->GetRenderTexture()->GetSRVNumber() });
	//renderQueue->AddSetShaderResource({ 17,graphicsDevice.GetLayerManager()->Get("EffectLayer")->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 18,graphicsDevice.GetLayerManager()->Get("BloomLayer_" + std::to_string(1))->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 19,graphicsDevice.GetLayerManager()->Get("BloomLayer_" + std::to_string(3))->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 20,graphicsDevice.GetLayerManager()->Get("BloomLayer_" + std::to_string(5))->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 21,graphicsDevice.GetLayerManager()->Get("resultLayer")->GetDepthTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 22,graphicsDevice.GetLayerManager()->Get("DofLayer_" + std::to_string(1))->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 23,graphicsDevice.GetLayerManager()->Get("DofLayer_" + std::to_string(5))->GetRenderTexture()->GetSRVNumber() });
	graphicsDevice.DrawMesh("2DPlane");

	graphicsDevice.ExecuteRenderQueue();
	graphicsDevice.ExecuteCommands();

	// ブラーした結果を元画像に合成
	graphicsDevice.SetShaderResourceDescriptorHeap();
	graphicsDevice.SetDefaultRenderTarget();
	graphicsDevice.SetShader("volumetricCloudShader");

	//cameraInfo.invProjMatrix = GE::Math::Matrix4x4::Inverse(cameraInfo.projMatrix);
	//cameraInfo.invViewMatrix = GE::Math::Matrix4x4::Inverse(cameraInfo.viewMatrix);

	struct RayInfo
	{
		GE::Math::Vector3 boundMin;
		float pad;
		GE::Math::Vector3 boundMax;
	};

	RayInfo rayInfo;
	static GE::Math::Vector3 pos = { 0,0,0 };
	static GE::Math::Vector3 scale = 2000;
	ImGui::DragFloat3("CloudPos"  , pos.value, 1.0f);
	ImGui::DragFloat3("CloudScale", scale.value, 1.0f);
	rayInfo.boundMin = pos - scale / 2;
	rayInfo.boundMax = pos + scale / 2;

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material, sizeof(GE::Material)) });

	renderQueue->AddSetConstantBufferInfo({ 13,cbufferAllocater->BindAndAttachData(13, &rayInfo, sizeof(RayInfo)) });

	renderQueue->AddSetShaderResource({ 16,graphicsDevice.GetLayerManager()->Get("defaultLayer")->GetRenderTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 17,graphicsDevice.GetLayerManager()->Get("resultLayer")->GetDepthTexture()->GetSRVNumber() });
	renderQueue->AddSetShaderResource({ 18,graphicsDevice.GetLayerManager()->Get("shadowLayer")->GetDepthTexture()->GetSRVNumber() });
	graphicsDevice.DrawMesh("2DPlane");

	graphicsDevice.ExecuteRenderQueue();

	//ImVec2 texSize = { textureAnimationInfo.textureSize.x / 4,textureAnimationInfo.textureSize.y / 4 };
	//GE::IDepthTexture* dofRenderTexture = graphicsDevice.GetLayerManager()->Get("resultLayer")->GetDepthTexture();
	//ImGui::Image((ImTextureID)dofRenderTexture->GetGPUHandle().ptr, texSize);

	GE::GUIManager::EndFrame();
	graphicsDevice.ExecuteCommands();
	graphicsDevice.ScreenFlip();
	return true;
}
