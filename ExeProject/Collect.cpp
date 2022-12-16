#include "Collect.h"

#include <GatesEngine/Header/Util/Utility.h          >
#include <GatesEngine/Header/Graphics\Window.h       >
#include<GatesEngine/Header/Graphics/Texture.h>

Collect* Collect::GetInstance()
{
	static Collect instance;
	return &instance;
}

void Collect::Start(GE::GameObjectManager* gameObjectManager)
{
	collectCount = 0;//収集物
	maxCollect = 5;//収集物(最大数)
	//各テクスチャ生成
	Create("chick", "texture_Chick", gameObjectManager, 80, 100, 3);//雛のテクスチャ生成
	Create("collect", "texture_Number", gameObjectManager, 190, 100, 0);//収集物のテクスチャ生成
	Create("symbol", "texture_symbol", gameObjectManager, 270, 50, 2);//スラッシュのテクスチャ生成
	Create("maxCollect", "texture_Number", gameObjectManager, 340, 100, 1);//収集物(最大数)のテクスチャ生成
}

void Collect::Update(const int& count, const int& goalCollect)
{
	collectCount = count;
	maxCollect = goalCollect;
}

void Collect::Create(const std::string& gui_tag, const std::string& tex_tag, GE::GameObjectManager* gameObjectManager, float posX, float scaleX, int collectNum)
{
	auto* collectObject = gameObjectManager->AddGameObject(new GE::GameObject(gui_tag, "collect"));
	auto* collectComponent = collectObject->AddComponent<CollectTex>();
	collectComponent->tag = tex_tag;
	collectComponent->num = collectNum;//CollectTexのswitch文の切り替え
	collectObject->GetTransform()->position = { posX,950,0 };//ポジション指定
	collectObject->GetTransform()->scale = { scaleX,100,0 };//サイズ指定
}

void CollectTex::Start()
{
}

void CollectTex::Update(float deltaTime)
{
	switch (num)
	{
	case (int)CollectName::collect://収集物の描画情報
		pivotPosX = Collect::GetInstance()->GetCollectCount();//画像の描画開始位置の代入
		texSizeX = 320;//画像サイズ
		clipSizeX = 32;
		break;
	case (int)CollectName::maxCollect://収集物(最大数)の描画情報
		pivotPosX = Collect::GetInstance()->GetMaxCollect();//画像の描画開始位置の代入
		texSizeX = 320;//画像サイズ
		clipSizeX = 32;
		break;
	case (int)CollectName::symbol://スラッシュの描画情報
		pivotPosX = 1;//画像の描画開始位置の代入
		texSizeX = 64;//画像サイズ
		clipSizeX = 32;
		break;
	default:
		pivotPosX = 0;//画像の描画開始位置の代入
		texSizeX = 64;//画像サイズ
		clipSizeX = 64;
		break;
	}
}

void CollectTex::LateDraw()
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
	textureAnimationInfo.textureSize = { texSizeX,64 };
	// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
	// textureSizeと一緒にすると切り抜かれずに描画される
	textureAnimationInfo.clipSize = { clipSizeX,64 };
	// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
	textureAnimationInfo.pivot = { pivotPosX,0 };

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	renderQueue->AddSetShaderResource({ 5,texture->GetSRVNumber() });
	graphicsDevice->DrawMesh("2DPlane");
}
