#include "TimeLimit.h"

TimeLimit::TimeLimit(const int& timer)
{
	this->timer = timer * 60 * fps;//分数を秒数に直して144fpsかける
}

void TimeLimit::Start()
{
}

void TimeLimit::Update()
{
	timer--;//カウントダウン
	minutes = timer / fps / 60;//分数の計算
	tenSeconds = timer / fps % 60 / 10;//秒数の十の位の計算
	oneSeconds = timer / fps % 60 % 10;//秒数の一の位の計算
}

void TimeLimit::Draw()
{
	//// 描画するためのGraphicsDeviceを取得しとくように！
	
	//// レンダーキューを2d用に切り替える
	//graphicsDevice.SetCurrentRenderQueue(false);

	//// テクスチャ描画用のシェーダーをセット
	//graphicsDevice.SetShader("DefaultSpriteWithTextureShader");
	
	//// 描画位置とかサイズとかの設定
	//auto windowSize = GE::Window::GetWindowSize();
	//GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ windowSize.x,windowSize.y,0 });
	//modelMatrix *= GE::Math::Matrix4x4::Translate({ 0,0,0 });
	
	//// 画像の色変えたりするよう
	//GE::Material material;
	//material.color = GE::Color::White();

	//// 2d用のカメラ情報 基本的に買えなくてok
	//GE::CameraInfo cameraInfo;
	//cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
	//cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	//// アニメーションの情報
	//GE::TextureAnimationInfo textureAnimationInfo;
	//GE::Texture* texture = graphicsDevice.GetTextureManager()->Get("テクスチャ名");
	//// 画像の元サイズ
	//textureAnimationInfo.textureSize = texture->GetSize();
	//// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
	//// textureSizeと一緒にすると切り抜かれずに描画される
	//textureAnimationInfo.clipSize = texture->GetSize();
	//// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
	//textureAnimationInfo.pivot = {0,0};

	//renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	//renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	//renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	//renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	//renderQueue->AddSetShaderResource({ 5,texture.GetSRVNumber() });
	//graphicsDevice.DrawMesh("2DPlane");
}