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

	MeshData<Vertex_UV_Normal> modelDataBird1;
	MeshCreater::LoadObjModelData(objModelPath + "bird1", modelDataBird1);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataBird1);
	meshManager->Add(mesh, "Bird1");

	MeshData<Vertex_UV_Normal> modelDataTree1;
	MeshCreater::LoadObjModelData("Resources/Model/ki", modelDataTree1);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataTree1);
	meshManager->Add(mesh, "Tree1");

	MeshData<Vertex_UV_Normal> modelDataGroundTest;
	MeshCreater::LoadObjModelData("Resources/Model/groundTest", modelDataGroundTest);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataGroundTest);
	meshManager->Add(mesh, "GroundTest");

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
	MeshData<Vertex_UV_Normal> modelTreeLeaf1;
	MeshCreater::LoadObjModelData("Resources/Model/tree_leaf1", modelTreeLeaf1);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelTreeLeaf1);
	meshManager->Add(mesh, "tree_leaf1");

	meshManager->Add(FbxLoader::Load("Bird", &graphicsDevice), "Player");
	//Œø‰Ê‰¹hit_wall
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/hit_wall.wav"), "hitWall");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "hitWall"));
		audio->SetVolume(0.3f);
	}
	//Œø‰Ê‰¹air
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/air.wav"), "air1");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "air1"));
		audio->SetVolume(0.3f);
	}
	//Œø‰Ê‰¹catch
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/catch2.wav"), "catch2");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "catch2"));
		audio->SetVolume(0.3f);
	}
	//Œø‰Ê‰¹flapping
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/flapping.wav"), "flapping1");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "flapping1"));
		audio->SetVolume(0.3f);
	}
	auto* textureManager = graphicsDevice.GetTextureManager();
	Texture* nullTexture = new Texture();
	nullTexture->Load("title.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_title");

	nullTexture = new Texture();
	nullTexture->Load("stage1.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_stage1");

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

	auto* testScene = sceneManager.AddScene(new SampleScene("SampleScene",sceneManager.GetSceneInitializer()));
	sceneManager.AddScene(new Clear("ClearScene", sceneManager.GetSceneInitializer()));
	sceneManager.AddScene(new Over("OverScene", sceneManager.GetSceneInitializer()));
	sceneManager.ChangeScene("SampleScene");

	return true;
}

bool Game::Initialize()
{
	Application::Initialize();

	// fps‚Ì•\Ž¦Ø‘Ö
	timer.SetIsShow(false);
	return true;
}

bool Game::Update()
{
	GE::GUIManager::StartFrame();
	Application::Update();
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

	graphicsDevice.SetCurrentRenderQueue(true);
	graphicsDevice.SetShaderResourceDescriptorHeap();
	graphicsDevice.SetDefaultRenderTarget();
	graphicsDevice.SetShader("SpriteTextureForPosteffectShader");
	auto windowSize = GE::Window::GetWindowSize();
	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ windowSize.x,windowSize.y,0 });
	windowSize.x /= 2;
	windowSize.y /= 2;
	modelMatrix *= GE::Math::Matrix4x4::Translate({ windowSize.x,windowSize.y,0 });
	GE::Material material;
	material.color = GE::Color::White();

	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	GE::TextureAnimationInfo textureAnimationInfo;
	GE::IRenderTexture* resultLayerRenderTexture = graphicsDevice.GetLayerManager()->Get("resultLayer")->GetRenderTexture();
	textureAnimationInfo.clipSize = resultLayerRenderTexture->GetSize();
	textureAnimationInfo.textureSize = textureAnimationInfo.clipSize;

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4,&textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,graphicsDevice.GetLayerManager()->Get("resultLayer")->GetRenderTexture()->GetSRVNumber() });
	graphicsDevice.DrawMesh("2DPlane");

	graphicsDevice.ExecuteRenderQueue();
	GE::GUIManager::EndFrame();
	graphicsDevice.ExecuteCommands();
	graphicsDevice.ScreenFlip();
	return true;
}
