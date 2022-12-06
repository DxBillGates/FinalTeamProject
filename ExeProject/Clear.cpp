#include "Clear.h"

#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >

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
}

void Clear::Update(float deltaTime)
{
	gameObjectManager.Update(deltaTime);

	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::SPACE)
		|| inputDevice->GetJoyconR()->GetButton(GE::JoyconButtonData::A))
	{
		changeSceneInfo.name = "SampleScene";
		changeSceneInfo.flag = true;
		changeSceneInfo.initNextSceneFlag = true;
	}
}

void Clear::Draw()
{
	gameObjectManager.Draw();
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
}

void Clear::UnLoad()
{
	// gameObjects���폜����
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

	// �A�j���[�V�����̏��
	GE::TextureAnimationInfo textureAnimationInfo;
	// �摜�̌��T�C�Y
	textureAnimationInfo.textureSize = 1;
	// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
	// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
	textureAnimationInfo.clipSize = 1;
	// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
	textureAnimationInfo.pivot = { 0,0 };

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get("texture_clear")->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}
