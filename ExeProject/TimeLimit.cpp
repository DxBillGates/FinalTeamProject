#include "TimeLimit.h"

#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include<GatesEngine/Header/Graphics/Texture.h>

TimeLimit* TimeLimit::GetInstance()
{
	static TimeLimit instance;
	return &instance;
}

TimeLimit::TimeLimit()
{
}

void TimeLimit::Start(GE::GameObjectManager* gameObjectManager)
{
	timer = 3;
	timer = timer * 60 * frameRate;//分数を秒数に直して144fpsかける
	minutes = timer / frameRate / 60;//分数の計算
	tenSeconds = timer / frameRate % 60 / 10;//秒数の十の位の計算
	oneSeconds = timer / frameRate % 60 % 10;//秒数の一の位の計算
	timeOver = false;

	Create("minutes", "texture_Number", gameObjectManager,80,100,0);
	Create("symbol", "texture_symbol", gameObjectManager, 160, 50, 3);
	Create("tenSeconds", "texture_Number", gameObjectManager, 240, 100, 1);
	Create("oneSeconds", "texture_Number", gameObjectManager, 340, 100, 2);
}

void TimeLimit::Update()
{
	if (timer <= 0)
	{
		timeOver = true;
	}
	else
	{
		timer--;//カウントダウン
		minutes = timer / frameRate / 60;//分数の計算
		tenSeconds = timer / frameRate % 60 / 10;//秒数の十の位の計算
		oneSeconds = timer / frameRate % 60 % 10;//秒数の一の位の計算
	}
}

void TimeLimit::Create(const std::string& gui_tag, const std::string& tex_tag, GE::GameObjectManager* gameObjectManager, float posX, float scaleX, int timeNum)
{
	auto* timeObject = gameObjectManager->AddGameObject(new GE::GameObject(gui_tag, "time"));
	auto* timeComponent = timeObject->AddComponent<TimeTex>();
	timeComponent->tag = tex_tag;
	timeComponent->num = timeNum;
	timeObject->GetTransform()->position = { posX,970,0 };
	timeObject->GetTransform()->scale = { scaleX,100,0 };
}

void TimeTex::Start()
{
}

void TimeTex::Update(float deltaTime)
{
	switch (num)
	{
	case TimeName::minutes:
		pivotPos = TimeLimit::GetInstance()->GetMinutes();
		texSize = 320;
		break;
	case TimeName::tenSeconds:
		pivotPos = TimeLimit::GetInstance()->GetTenSeconds();
		texSize = 320;
		break;
	case TimeName::oneSeconds:
		pivotPos = TimeLimit::GetInstance()->GetOneSeconds();
		texSize = 320;
		break;
	default:
		pivotPos = 0;
		texSize = 64;
		break;
	}
}

void TimeTex::LateDraw()
{
	// 描画するためのGraphicsDeviceを取得しとくように！
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();
	
	// レンダーキューを2d用に切り替える
	graphicsDevice->SetCurrentRenderQueue(false);

	// テクスチャ描画用のシェーダーをセット
	graphicsDevice->SetShader("DefaultSpriteWithTextureShader");
	
	// 描画位置とかサイズとかの設定
	//auto windowSize = GE::Window::GetWindowSize();
	//GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ windowSize.x,windowSize.y,0 });
	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ 0 });
	//modelMatrix *= GE::Math::Matrix4x4::Translate({ 0,0,0 });
	modelMatrix = transform->GetMatrix();
	
	// 画像の色変えたりするよう
	GE::Material material;
	material.color = GE::Color::White();

	// 2d用のカメラ情報 基本的に買えなくてok
	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	// アニメーションの情報
	GE::TextureAnimationInfo textureAnimationInfo;
	GE::ITexture* texture = graphicsDevice->GetTextureManager()->Get(tag);

	// 画像の元サイズ
	textureAnimationInfo.textureSize = {texSize,64};
	// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
	// textureSizeと一緒にすると切り抜かれずに描画される
	textureAnimationInfo.clipSize = {32,64};
	// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
	textureAnimationInfo.pivot = { pivotPos,0 };

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,texture->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}