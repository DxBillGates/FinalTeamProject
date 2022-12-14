#include "Game.h"
#include "SampleScene.h"
#include "Clear.h"
#include "Over.h"
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Graphics/CBufferStruct.h>
#include <GatesEngine/Header/GUI/GUIManager.h>

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

	//効果音hit_wall
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/hit_wall.wav"), "hitWall");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "hitWall"));
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
	}
	//効果音flapping
	{
		auto* audioData = audioManager.AddAudioData(new GE::AudioData("Resources/Audio/flapping.wav"), "flapping1");
		auto* audio = audioManager.AddAudio(new GE::Audio(audioData, "flapping1"));
		audio->SetVolume(0.3f);
	}
	auto* testScene = sceneManager.AddScene(new SampleScene("SampleScene",sceneManager.GetSceneInitializer()));
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
