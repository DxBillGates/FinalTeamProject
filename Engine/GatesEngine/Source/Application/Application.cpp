#include "..\..\Header\Application\Application.h"
#include "..\..\Header\Graphics\Shader.h"
#include "..\..\Header\Graphics\RootSignature.h"
#include "..\..\Header\Graphics\GraphicsPipeline.h"
#include "..\..\Header\Graphics\Mesh.h"
#include "..\..\Header\Graphics\MeshData.h"
#include "..\..\Header\Graphics\VertexData.h"
#include "..\..\Header\Graphics\MeshCreater.h"
#include "..\..\Header\Graphics\Camera3DDebug.h"
#include "..\..\Header\Graphics\RenderTexture.h"
#include "..\..\Header\Graphics\DepthTexture.h"
#include "..\..\Header\Graphics\Layer.h"
#include "..\..\Header\Graphics\Texture.h"
#include "..\..\Header\GameFramework\GameObject\GameObject.h"
#include "..\..\Header\GUI\GUIManager.h"
#include "..\..\Header\GameFramework\GameSetting.h"
#include "..\..\Header\Graphics\FbxLoader.h"

GE::Application::Application()
	: Application(Math::Vector2(1920, 1080), Math::Vector2(1920, 1080))
{
}

GE::Application::Application(const WindowData& windowData, const Math::Vector2& resolution)
	: timer(Utility::Timer())
	, mainWindow(Window())
	, inputDevice(InputDevice::GetInstance())
	, audioManager(AudioManager())
	, sceneManager(SceneManager())
	, graphicsDevice(GraphicsDeviceDx12())
	, mainCamera(nullptr)
{
	timer.SetFrameRate(144);
	timer.SetIsShow(false);

	mainWindow.Create(windowData);

	inputDevice->Create(mainWindow.GetHandle(), mainWindow.GetHInstance());
	graphicsDevice.Create(resolution, mainWindow.GetHandle());

	mainCamera = new Camera3DDebug();

	graphicsDevice.SetMainCamera(mainCamera);
	GameObject::SetGraphicsDevice(&graphicsDevice);

	SceneInitializer sceneInitializer = { &audioManager,inputDevice,&graphicsDevice };
	sceneManager.SetSceneInitializer(sceneInitializer);

	GUIManager::Initialize(mainWindow.GetHandle(), &graphicsDevice);

	mainWindow.PreviewWindow();
}

GE::Application::Application(const Math::Vector2& size, const GE::Math::Vector2& resolution, const std::string& title, WindowMode mode)
	: Application(WindowData(NULL, NULL, NULL, size, title, mode), resolution)
{
}

GE::Application::~Application()
{
	delete mainCamera;

	FbxLoader::Finalize();
	GUIManager::Finalize();
}

bool GE::Application::LoadContents()
{
	// sample bgm ???[?h??????
	auto* testBGMData = audioManager.AddAudioData(new AudioData("Resources/Audio/BGM1.wav"), "testBGMData");
	auto* testBGM = audioManager.AddAudio(new Audio(testBGMData, "testBGM"));
	testBGM->Start();
	testBGM->SetVolume(0.1f);
	testBGM->SetPitch(1);

	auto* device = graphicsDevice.GetDevice();
	auto* cmdList = graphicsDevice.GetCmdList();
	auto* shaderResourceHeap = graphicsDevice.GetShaderResourceHeap();

	auto* meshManager = graphicsDevice.GetMeshManager();
	FbxLoader::Initialize();

	Mesh* mesh;
	// ???|??????
	MeshData<Vertex_UV_Normal> meshDataPlane;
	MeshCreater::CreatePlane(meshDataPlane);
	mesh = new Mesh();
	mesh->Create(device, cmdList, meshDataPlane);
	meshManager->Add(mesh, "Plane");

	MeshData<Vertex_UV> meshData2DPlane;
	MeshCreater::Create2DPlane(meshData2DPlane);
	mesh = new Mesh();
	mesh->Create(device, cmdList, meshData2DPlane);
	meshManager->Add(mesh, "2DPlane");

	MeshData<Vertex_UV> meshData2DPlanePivot;
	MeshCreater::Create2DPlane(meshData2DPlanePivot, { 1 }, { 1 }, { -1,0 });
	mesh = new Mesh();
	mesh->Create(device, cmdList, meshData2DPlanePivot);
	meshManager->Add(mesh, "2DPlanePivot");

	// ?O???b?h????
	MeshData<Vertex_Color> meshDataGrid;
	MeshCreater::CreateGrid(meshDataGrid, 100.0, 100000);
	mesh = new Mesh();
	mesh->Create(device, cmdList, meshDataGrid);
	meshManager->Add(mesh, "Grid");

	// lineCube????
	MeshData<Vertex_Color> meshDataLineCube;
	MeshCreater::CreateLineCube(meshDataLineCube);
	mesh = new Mesh();
	mesh->Create(device, cmdList, meshDataLineCube);
	meshManager->Add(mesh, "LineCube");

	// lineCircle????
	MeshData<Vertex_Color> meshDataLineCircle;
	MeshCreater::CreateLineCircle(meshDataLineCircle);
	mesh = new Mesh();
	mesh->Create(device, cmdList, meshDataLineCircle);
	meshManager->Add(mesh, "LineCircle");

	// line????
	MeshData<Vertex_Color> meshDataLine;
	MeshCreater::CreateLine(meshDataLine);
	mesh = new Mesh();
	mesh->Create(device, cmdList, meshDataLine);
	meshManager->Add(mesh, "Line");

	// lineAxis????
	MeshData<Vertex_Color> meshDataLineAxis;
	MeshCreater::CreateLineAxis(meshDataLineAxis);
	mesh = new Mesh();
	mesh->Create(device, cmdList, meshDataLineAxis);
	meshManager->Add(mesh, "LineAxis");

	std::string resourcesFolder = "Resources/";
	std::string modelFolder = "Model/";
	std::string objModelFolder = "objModel/";
	std::string fbxModelFolder = "fbxModel/";

	std::string objModelPath = resourcesFolder + modelFolder + objModelFolder;
	std::string fbxModelPath = resourcesFolder + modelFolder + fbxModelFolder;

	// model????????
	MeshData<Vertex_UV_Normal> modelDataCube;
	MeshCreater::LoadObjModelData(objModelPath + "cube", modelDataCube);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataCube);
	meshManager->Add(mesh, "Cube");

	MeshData<Vertex_UV_Normal> modelDataSphere;
	MeshCreater::LoadObjModelData(objModelPath + "sphere", modelDataSphere);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataSphere);
	meshManager->Add(mesh, "Sphere");

	MeshData<Vertex_UV_Normal> modelDataPlatonic;
	MeshCreater::LoadObjModelData(objModelPath + "platonic", modelDataPlatonic);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataPlatonic);
	meshManager->Add(mesh, "Platonic");

	MeshData<Vertex_UV_Normal> modelDataCorn;
	MeshCreater::LoadObjModelData(objModelPath + "corn", modelDataCorn);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataCorn);
	meshManager->Add(mesh, "Corn");

	MeshData<Vertex_UV_Normal> modelDataCylinder;
	MeshCreater::LoadObjModelData(objModelPath + "cylinder", modelDataCylinder);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataCylinder);
	meshManager->Add(mesh, "Cylinder");

	MeshData<Vertex_UV_Normal> modelDataSkydome;
	MeshCreater::LoadObjModelData(objModelPath + "skydome", modelDataSkydome);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataSkydome);
	meshManager->Add(mesh, "Skydome");

	MeshData<Vertex_UV_Normal> modelDataTorus;
	MeshCreater::LoadObjModelData(objModelPath + "torus", modelDataTorus);
	mesh = new Mesh();
	mesh->Create(device, cmdList, modelDataTorus);
	meshManager->Add(mesh, "Torus");

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

	meshManager->Add(FbxLoader::Load("Bird", &graphicsDevice), "Player");

	// texture load
	auto* textureManager = graphicsDevice.GetTextureManager();
	Texture* nullTexture = new Texture();
	nullTexture->Load("texture_null.png", device, shaderResourceHeap);
	textureManager->Add(nullTexture, "texture_null");

	nullTexture = new Texture();
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

	// shader compile
	Shader defaultMeshVertexShader, defaultMeshPixelShader;
	defaultMeshVertexShader.CompileShaderFileWithoutFormat(L"DefaultMeshVertexShader", "vs_5_0");
	defaultMeshPixelShader.CompileShaderFileWithoutFormat(L"DefaultMeshPixelShader", "ps_5_0");
	Shader defaultSkinMeshVertexShader;
	defaultSkinMeshVertexShader.CompileShaderFileWithoutFormat(L"DefaultSkinMeshVertexShader", "vs_5_0");
	Shader defaultLineVertexShader, defaultLinePixelShader;
	defaultLineVertexShader.CompileShaderFileWithoutFormat(L"DefaultLineVertexShader", "vs_5_0");
	defaultLinePixelShader.CompileShaderFileWithoutFormat(L"DefaultLinePixelShader", "ps_5_0");
	Shader defaultMeshWithTexturePixelShader;
	defaultMeshWithTexturePixelShader.CompileShaderFileWithoutFormat(L"DefaultMeshWithTexturePixelShader", "ps_5_0");
	Shader defaultSpriteVertexShader, defaultSpritePixelShader;
	defaultSpriteVertexShader.CompileShaderFileWithoutFormat(L"DefaultSpriteVertexShader", "vs_5_0");
	defaultSpritePixelShader.CompileShaderFileWithoutFormat(L"DefaultSpritePixelShader", "ps_5_0");
	Shader gaussBlurPixelShader;
	gaussBlurPixelShader.CompileShaderFileWithoutFormat(L"GaussBlurPixelShader", "ps_5_0");
	Shader defaultSpriteWithTextureVertexShader, defaultSpriteWithTexturePixelShader;
	defaultSpriteWithTextureVertexShader.CompileShaderFileWithoutFormat(L"DefaultSpriteWithTextureVertexShader", "vs_5_0");
	defaultSpriteWithTexturePixelShader.CompileShaderFileWithoutFormat(L"DefaultSpriteWithTexturePixelShader", "ps_5_0");
	Shader spriteTextureForPosteffectPixelShader;
	spriteTextureForPosteffectPixelShader.CompileShaderFileWithoutFormat(L"SpriteTextureForPosteffectPixelShader", "ps_5_0");

	// rootSignature????
	auto* rootSignatureManager = graphicsDevice.GetRootSignatureManager();
	RootSignature* defaultMeshRootSignature = new RootSignature();
	defaultMeshRootSignature->Create(device, { DescriptorRangeType::CBV,DescriptorRangeType::CBV,DescriptorRangeType::CBV,DescriptorRangeType::CBV });
	rootSignatureManager->Add(defaultMeshRootSignature, "CBV4");
	// texture ?t?????[?g?V?O?l?`??
	RootSignature* defaultMeshWithOneSrvRootSignature = new RootSignature();
	defaultMeshWithOneSrvRootSignature->Create(device, { DescriptorRangeType::CBV,DescriptorRangeType::CBV,DescriptorRangeType::CBV,DescriptorRangeType::CBV,DescriptorRangeType::SRV });
	rootSignatureManager->Add(defaultMeshWithOneSrvRootSignature, "CBV4SRV1");
	// cbv5srv1???[?g?V?O?l?`??
	RootSignature* cbv5srv1RootSignature = new RootSignature();
	cbv5srv1RootSignature->Create(device, { DescriptorRangeType::CBV,DescriptorRangeType::CBV ,DescriptorRangeType::CBV ,DescriptorRangeType::CBV ,DescriptorRangeType::CBV ,DescriptorRangeType::SRV });
	rootSignatureManager->Add(cbv5srv1RootSignature, "CBV5SRV1");
	// cbv16srv16???[?g?V?O?l?`??
	RootSignature* testRootSignature = new RootSignature();
	testRootSignature->Create(device, { 16,16,0 });
	rootSignatureManager->Add(testRootSignature, "CBV16SRV16");

	// demo graphicsPipeline????
	GraphicsPipelineInfo pipelineInfo = GraphicsPipelineInfo();
	auto* graphicsPipelineManager = graphicsDevice.GetGraphicsPipelineManager();
	GraphicsPipeline* defaultMeshPipline = new GraphicsPipeline({ &defaultMeshVertexShader,nullptr,nullptr,nullptr,&defaultMeshPixelShader });
	defaultMeshPipline->Create(device, { GraphicsPipelineInputLayout::POSITION,GraphicsPipelineInputLayout::UV ,GraphicsPipelineInputLayout::NORMAL }, defaultMeshRootSignature, pipelineInfo);
	graphicsPipelineManager->Add(defaultMeshPipline, "DefaultMeshShader");
	// skinMesh shader
	GraphicsPipeline* defaultSkinMeshPipline = new GraphicsPipeline({ &defaultSkinMeshVertexShader,nullptr,nullptr,nullptr,&defaultMeshPixelShader });
	defaultSkinMeshPipline->Create(device, { GraphicsPipelineInputLayout::POSITION,GraphicsPipelineInputLayout::UV ,GraphicsPipelineInputLayout::NORMAL,GraphicsPipelineInputLayout::BONEINDEX,GraphicsPipelineInputLayout::BONEWEIGHT }, defaultMeshRootSignature, pipelineInfo);
	graphicsPipelineManager->Add(defaultSkinMeshPipline, "DefaultSkinMeshShader");
	// line shader
	pipelineInfo.topologyType = GraphicsPipelinePrimitiveTopolotyType::LINE;
	GraphicsPipeline* dafaultLinePipeline = new GraphicsPipeline({ &defaultLineVertexShader,nullptr,nullptr,nullptr,&defaultLinePixelShader });
	dafaultLinePipeline->Create(device, { GraphicsPipelineInputLayout::POSITION,GraphicsPipelineInputLayout::COLOR }, defaultMeshRootSignature, pipelineInfo);
	graphicsPipelineManager->Add(dafaultLinePipeline, "DefaultLineShader");
	// texture draw shader
	pipelineInfo.topologyType = GraphicsPipelinePrimitiveTopolotyType::TRIANGLE;
	GraphicsPipeline* dafaultMeshWithTexturePipeline = new GraphicsPipeline({ &defaultMeshVertexShader,nullptr,nullptr,nullptr,&defaultMeshWithTexturePixelShader });
	dafaultMeshWithTexturePipeline->Create(device, { GraphicsPipelineInputLayout::POSITION,GraphicsPipelineInputLayout::UV ,GraphicsPipelineInputLayout::NORMAL }, defaultMeshWithOneSrvRootSignature, pipelineInfo);
	graphicsPipelineManager->Add(dafaultMeshWithTexturePipeline, "DefaultMeshWithTextureShader");
	// default sprite shader
	pipelineInfo.topologyType = GraphicsPipelinePrimitiveTopolotyType::TRIANGLE;
	pipelineInfo.isUseDepthClip = false;
	GraphicsPipeline* dafaultSpritePipeline = new GraphicsPipeline({ &defaultSpriteVertexShader,nullptr,nullptr,nullptr,&defaultSpritePixelShader });
	dafaultSpritePipeline->Create(device, { GraphicsPipelineInputLayout::POSITION,GraphicsPipelineInputLayout::UV }, defaultMeshRootSignature, pipelineInfo);
	graphicsPipelineManager->Add(dafaultSpritePipeline, "DefaultSpriteShader");
	// default sprite with texture shader
	pipelineInfo.topologyType = GraphicsPipelinePrimitiveTopolotyType::TRIANGLE;
	pipelineInfo.isUseDepthClip = false;
	GraphicsPipeline* dafaultSpriteWithTexturePipeline = new GraphicsPipeline({ &defaultSpriteWithTextureVertexShader,nullptr,nullptr,nullptr,&defaultSpriteWithTexturePixelShader });
	dafaultSpriteWithTexturePipeline->Create(device, { GraphicsPipelineInputLayout::POSITION,GraphicsPipelineInputLayout::UV }, cbv5srv1RootSignature, pipelineInfo);
	graphicsPipelineManager->Add(dafaultSpriteWithTexturePipeline, "DefaultSpriteWithTextureShader");
	GraphicsPipeline* spriteTextureForPosteffectPipeline = new GraphicsPipeline({ &defaultSpriteWithTextureVertexShader,nullptr,nullptr,nullptr,&spriteTextureForPosteffectPixelShader });
	spriteTextureForPosteffectPipeline->Create(device, { GraphicsPipelineInputLayout::POSITION,GraphicsPipelineInputLayout::UV }, cbv5srv1RootSignature, pipelineInfo);
	graphicsPipelineManager->Add(spriteTextureForPosteffectPipeline, "SpriteTextureForPosteffectShader");
	// gauss blur shader
	GraphicsPipeline* gaussBlurPipeline = new GraphicsPipeline({ &defaultSpriteVertexShader,nullptr,nullptr,nullptr,&gaussBlurPixelShader });
	gaussBlurPipeline->Create(device, { GraphicsPipelineInputLayout::POSITION,GraphicsPipelineInputLayout::UV }, cbv5srv1RootSignature, pipelineInfo);
	graphicsPipelineManager->Add(gaussBlurPipeline, "GaussBlurShader");

	// demo layer????
	auto* layerManager = graphicsDevice.GetLayerManager();

	RenderTexture* demoRenderTexture = new RenderTexture();
	DepthTexture* demoDepthTexture = new DepthTexture();
	demoRenderTexture->Create(device, shaderResourceHeap, mainWindow.GetWindowSize(), Color::Blue());
	demoDepthTexture->Create(device, shaderResourceHeap, mainWindow.GetWindowSize());
	layerManager->Add(new Layer(demoRenderTexture, demoDepthTexture), "demoLayer");

	RenderTexture* resultRenderTexture = new RenderTexture();
	DepthTexture* resultDepthTexture = new DepthTexture();
	resultRenderTexture->Create(device, shaderResourceHeap, mainWindow.GetWindowSize(), Color::Blue());
	resultDepthTexture->Create(device, shaderResourceHeap, mainWindow.GetWindowSize());
	layerManager->Add(new Layer(resultRenderTexture, resultDepthTexture), "resultLayer");

	return true;
}

bool GE::Application::Initialize()
{
	mainCamera->Initialize();
	sceneManager.Initialize();

	GameSetting::Time::Initialize();
	return true;
}

bool GE::Application::Update()
{
	sceneManager.Update(timer.GetElapsedTime());
	mainCamera->Update();
	return true;
}

bool GE::Application::Draw()
{
	sceneManager.Draw();
	sceneManager.LateDraw();
	return true;
}

int GE::Application::Run()
{
	if (!LoadContents())return -1;

	inputDevice->Initialize();
	GameSetting::Time::Start();

	if (!Initialize())return -1;

	while (!inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::ESC))
	{
		if (timer.Update())continue;
		inputDevice->Update();
		GameSetting::Time::Update(timer.GetElapsedTime());
		if (!Update())return -1;
		if (!Draw())return -1;

		if (!mainWindow.ProcessMessage())return 0;
	}

	return 0;
}
