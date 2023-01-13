#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics\Window.h>
#include<fstream>
#include<sstream>
#include<cassert>

#include"ScreenUI.h"
#include"PlayerComponent.h"



ScreenUIManager* ScreenUIManager::GetInstance()
{
	static ScreenUIManager instance;
	return &instance;
}

ScreenUIManager::SpriteInfo ScreenUIManager::Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName)
{
	SpriteInfo result;
	result.transform.position = pos;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	return result;
}
void ScreenUIManager::Start()
{
	//微調整の座標手打ちだから環境でずれそう	
	object["crash"] = Set(GE::Math::Vector3(GE::Window::GetWindowSize().x / 2.f, GE::Window::GetWindowSize().y / 2.f - 150.f, 0.f),
		{ 400,100,0 }, GE::Color::White(), "crash_info");
}
void ScreenUIManager::Update(float deltaTime)
{
	if (PlayerComponent::statas == PlayerComponent::PlayerStatas::CRASH)
	{
		object["crash"].isDraw = true;
	}
	else
	{
		object["crash"].isDraw = false;
	}
}

void ScreenUIManager::DrawSprite(GE::IGraphicsDeviceDx12* graphicsDevice)
{
	for (auto o : object)
	{
		if (o.second.isDraw)
		{
			GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
			GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

			// レンダーキューを2d用に切り替える
			graphicsDevice->SetCurrentRenderQueue(false);

			// テクスチャ描画用のシェーダーをセット
			graphicsDevice->SetShader("DefaultSpriteWithTextureShader");

			GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ 0 });
			modelMatrix = o.second.transform.GetMatrix();

			// 画像の色変えたりするよう
			GE::Material material;
			material.color = o.second.color;

			// 2d用のカメラ情報 基本的に買えなくてok
			GE::CameraInfo cameraInfo;
			cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
			cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

			// アニメーションの情報
			GE::TextureAnimationInfo textureAnimationInfo;

			// 画像の元サイズ
			textureAnimationInfo.textureSize = o.second.texSize;
			// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
			// textureSizeと一緒にすると切り抜かれずに描画される
			textureAnimationInfo.clipSize = o.second.clipSize;
			// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
			textureAnimationInfo.pivot = o.second.pivotPos;

			renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
			renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
			renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
			renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
			renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get(o.second.textureName)->GetSRVNumber() });
			graphicsDevice->DrawMesh("2DPlane");
		}
	}
}


void ScreenUIManager::LoadPosition(const std::string& filename)
{
	std::vector<LoadObj> ft;

	std::ifstream file;
	//ファイルを開く
	file.open(filename);
	//ファイルが開けなかったらとまる
	if (file.fail()) {
		assert(0);
	}

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line)) {

		// 1行分の文字列
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "FieldTree") {
			LoadObj result;
			line_stream >> result.pos.x;
			line_stream >> result.pos.y;
			line_stream >> result.pos.z;

			line_stream >> result.rot.x;
			line_stream >> result.rot.y;
			line_stream >> result.rot.z;

			line_stream >> result.scale.x;
			line_stream >> result.scale.y;
			line_stream >> result.scale.z;

			line_stream >> result.col.r;
			line_stream >> result.col.g;
			line_stream >> result.col.b;
			result.col.a = 1.0f;
			ft.emplace_back(result);
		}

	}
	file.close();
	////ファイルの座標セット
	//int index = ft.size() < fieldTree.size() ? ft.size() : fieldTree.size();
	//for (int i = 0; i < index; i++)
	//{
	//	fieldTree[i]->GetTransform()->position = ft[i].pos;
	//	fieldTree[i]->GetComponent<FieldTree>()->rotation_euler = ft[i].rot;
	//	fieldTree[i]->GetComponent<FieldTree>()->scale = ft[i].scale.x;
	//}
}

void ScreenUIManager::SaveCurrentPosition(const std::string& filename)
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//内容初期化
	writing_file.clear();
	if (!writing_file.is_open()) { assert(0); }
	////普通の木
	//for (int i = 0; i < fieldTree.size(); i++)
	//{
	//	GE::Math::Vector3 pos = fieldTree[i]->GetTransform()->position;
	//	float scale = fieldTree[i]->GetComponent<FieldTree>()->scale;
	//	GE::Math::Vector3 rota = fieldTree[i]->GetComponent<FieldTree>()->rotation_euler;
	//	GE::Color col = fieldTree[i]->GetColor();

	//	writing_file << "FieldTree " << pos.x << " " << pos.y << " " << pos.z <<
	//		" " << rota.x << " " << rota.y << " " << rota.z <<
	//		" " << scale << " " << scale << " " << scale <<
	//		" " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
	//}

	writing_file.close();
}

void ScreenUIManager::UnLoad()
{
	object.clear();
}

