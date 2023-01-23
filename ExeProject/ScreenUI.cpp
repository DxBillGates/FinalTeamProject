#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics\Window.h>
#include<fstream>
#include<sstream>
#include<cassert>

#include"ScreenUI.h"
#include"PlayerComponent.h"
#include"Option.h"
#include"Title.h"
#include"TimeLimit.h"



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

ScreenUIManager::SpriteInfo ScreenUIManager::Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName, GE::Math::Vector2 texSize, GE::Math::Vector2 clipSize)
{
	SpriteInfo result;
	result.transform.position = pos;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	result.texSize = texSize;
	result.clipSize = clipSize;
	return result;
}
void ScreenUIManager::TitleMenuActive(bool isActive)
{
	object["title_start"].isDraw = isActive;
	object["title_option"].isDraw = isActive;
	object["title_exit"].isDraw = isActive;
	object["title_name"].isDraw = isActive;
}

void ScreenUIManager::OptionMenuActive(bool isActive)
{
	object["option_bgm"].isDraw = isActive;
	object["option_se"].isDraw = isActive;
	object["option_back"].isDraw = isActive;
	object["option_right"].isDraw = isActive;
	object["option_left"].isDraw = isActive;
}
float ScreenUIManager::SetLerp(std::string name, float lerpTime, float deltaTime)
{
	if (object[name].lerpCount < lerpTime)
	{
		object[name].lerpCount += deltaTime;
		return object[name].lerpCount / lerpTime;
	}
	else
	{
		return 1.0f;
	}
}
void ScreenUIManager::Start()
{
	winSize = GE::Window::GetWindowSize();
	center = winSize / 2.0f;
	a = 0.0f;

	//微調整の座標手打ちだから環境でずれそう
#pragma region オプション
	object["option_bgm"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["option_se"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["option_back"] = Set({}, { 150, 100, 0 }, GE::Color::White(), "texture_back");
	object["option_right"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_next");
	object["option_right"].transform.rotation = GE::Math::Quaternion::Euler({ 0, 0, 180 });
	object["option_left"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_next");
#pragma endregion

#pragma region タイトル
	object["title_start"] = Set(GE::Math::Vector3(winSize.x - 300, center.y, 0.0f), { 300,100,0 }, GE::Color::White(), "texture_start");
	object["title_option"] = Set(GE::Math::Vector3(winSize.x - 300, center.y + 100, 0.0f), { 300,100,0 }, GE::Color::White(), "texture_option");
	object["title_exit"] = Set(GE::Math::Vector3(winSize.x - 300, center.y + 200, 0.0f), { 300,100,0 }, GE::Color::White(), "texture_exit");
	object["title_name"] = Set(GE::Math::Vector3(1500, winSize.y / 2 - 220.0f, 0.0f), { 1319 / 2.0f,642 / 2.0f,0 }, GE::Color::White(), "texture_title");
#pragma endregion

#pragma region タイムリミット
	object["time_minutes"] = Set(GE::Math::Vector3(80, 850, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["time_symbol"] = Set(GE::Math::Vector3(160, 850, 0.0f), { 50,100,0 }, GE::Color::White(), "texture_symbol", { 64,64 }, { 32,64 });
	object["time_tenSeconds"] = Set(GE::Math::Vector3(240, 850, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["time_oneSeconds"] = Set(GE::Math::Vector3(340, 850, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
#pragma endregion

	object["crash"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), { 400,100,0 }, GE::Color::White(), "crash_info_tex");
	object["lockon_info"] = Set(GE::Math::Vector3(217, 600, 0.0f), GE::Math::Vector3(600, 64, 0) * 0.6f, GE::Color::White(), "lockon_info_tex");
	object["dash_info"] = Set(GE::Math::Vector3(168, 670, 0.0f), GE::Math::Vector3(256, 64, 0) * 1.1f, GE::Color::White(), "control_info_1_tex", { 512,384 }, { 512, 128 });
	object["dash_info"].pivotPos = 2;
	object["go_tree"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), GE::Math::Vector3(256, 64, 0) * 0.6f, GE::Color::White(), "control_info_2_tex");
	//ジョイコン振ってるアニメーション
	//object["gyro_shake"] = Set(object["crash"].transform.position - GE::Math::Vector3(300, 0, 0), { 150,150,0 }, GE::Color::White(), "gyro_shake_tex", { 896,128 }, { 128,128 });

	//遷移の値初期化
	for (auto o : object)
	{
		o.second.lerpCount = 0.0f;
	}
}
void ScreenUIManager::Update(float deltaTime)
{
	const float f = 144.0f / (1.0f / deltaTime);
	const float addCount = 0.1f * f;
	//Title
	TitleMenuActive(false);

	object["crash"].isDraw = false;
	object["gyro_shake"].isDraw = false;
	object["control_info_2"].isDraw = true;
	object["lockon_info"].isDraw = false;
	object["dash_info"].isDraw = false;
	object["go_tree"].isDraw = false;

	//タイムリミット
	object["time_minutes"].isDraw = true;
	object["time_minutes"].pivotPos = TimeLimit::GetInstance()->GetMinutes();
	object["time_tenSeconds"].isDraw = true;
	object["time_tenSeconds"].pivotPos = TimeLimit::GetInstance()->GetTenSeconds();
	object["time_oneSeconds"].isDraw = true;
	object["time_oneSeconds"].pivotPos = TimeLimit::GetInstance()->GetOneSeconds();
	object["time_symbol"].isDraw = true;
	if (PlayerComponent::isGoTree)
	{
		object["go_tree"].isDraw = true;
	}
	switch (PlayerComponent::statas)
	{
	case PlayerComponent::PlayerStatas::CRASH:
		object["crash"].isDraw = true;

		//object["gyro_shake"].isDraw = true;
		break;
	case PlayerComponent::PlayerStatas::TITLE:
		object["title_name"].isDraw = true;

		//タイムリミット描画しない
		object["time_minutes"].isDraw = false;
		object["time_symbol"].isDraw = false;
		object["time_tenSeconds"].isDraw = false;
		object["time_oneSeconds"].isDraw = false;
		break;
	case PlayerComponent::PlayerStatas::TITLE_MENU:
		//タイムリミット描画しない
		object["time_minutes"].isDraw = false;
		object["time_symbol"].isDraw = false;
		object["time_tenSeconds"].isDraw = false;
		object["time_oneSeconds"].isDraw = false;

		//タイトル描画
		TitleMenuActive(true);
		//横からフェードイン
		object["title_start"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y, 0.0f), SetLerp("title_start", 5.0f, addCount));
		object["title_option"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 100, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 100, 0.0f), SetLerp("title_option", 6.0f, addCount));
		object["title_exit"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 200, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 200, 0.0f), SetLerp("title_exit", 7.0f, addCount));
		break;
	case PlayerComponent::PlayerStatas::LOCKON_SHOOT:

		break;
	case PlayerComponent::PlayerStatas::MOVE:

		object["lockon_info"].isDraw = true;
		object["dash_info"].isDraw = true;

		break;
	}

#pragma region オプション中
	OptionMenuActive(false);
	if (Title::GetInstance()->GetSelect(Title::States::option))
	{
		TitleMenuActive(false);
		OptionMenuActive(true);
		object["option_right"].transform.position.x = GE::Math::Lerp(winSize.x + 1000, winSize.x - 200, SetLerp("option_right", 5.0f, addCount));
		object["option_left"].transform.position.x = GE::Math::Lerp(winSize.x + 1000, winSize.x - 450, SetLerp("option_left", 5.0f, addCount));

		object["option_bgm"].pivotPos.x = OptionData::BGM_vol;
		object["option_bgm"].color = GE::Color::White();
		object["option_bgm"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y, 0.0f), SetLerp("option_bgm", 5.0f, addCount));

		if (Option::select == Option::Select::BGM_VOL) { object["option_bgm"].color = GE::Color::Red(); }
		object["option_se"].pivotPos.x = OptionData::SE_vol;
		object["option_se"].color = GE::Color::White();
		object["option_se"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 100, 0.0f), GE::Math::Vector3(winSize.x - 305, center.y + 100, 0.0f), SetLerp("option_se", 6.0f, addCount));

		if (Option::select == Option::Select::SE_VOL) { object["option_se"].color = GE::Color::Red(); }
		object["option_back"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 200, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 200, 0.0f), SetLerp("option_back", 7.0f, addCount));
		if (Option::select == Option::Select::Back)
		{
			object["option_back"].color = GE::Color::Red();
			object["option_right"].isDraw = false;
			object["option_left"].isDraw = false;
			//タイトルメニューの遷移初期化
			object["title_start"].lerpCount = 0.f;
			object["title_option"].lerpCount = 0.f;
			object["title_exit"].lerpCount = 0.f;
			object["title_name"].lerpCount = 0.f;

		}
		else
		{
			object["option_back"].color = GE::Color::White();
			object["option_right"].transform.position.y = (GE::Window::GetWindowSize().y / 2) + (int)Option::select * 100;
			object["option_left"].transform.position.y = (GE::Window::GetWindowSize().y / 2) + (int)Option::select * 100;
		}
	}
	else
	{
		//オプションメニューの遷移初期化
		object["option_bgm"].lerpCount = 0.f;
		object["option_se"].lerpCount = 0.f;
		object["option_back"].lerpCount = 0.f;
		object["option_right"].lerpCount = 0.f;
		object["option_left"].lerpCount = 0.f;
	}
#pragma endregion

#pragma region タイトル
	if (!Title::GetInstance()->GetDecid())
	{
		object["title_start"].color = GE::Color::White();
		object["title_option"].color = GE::Color::White();
		object["title_exit"].color = GE::Color::White();

		switch (Title::GetInstance()->states)
		{
		case Title::States::start:
			object["title_start"].color = GE::Color::Red();
			break;
		case Title::States::option:
			object["title_option"].color = GE::Color::Red();
			break;
		case Title::States::exit:
			object["title_exit"].color = GE::Color::Red();
			break;
		default:
			break;
		}
	}
#pragma endregion

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



	}
	file.close();
}

void ScreenUIManager::SaveCurrentPosition(const std::string& filename)
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//内容初期化
	writing_file.clear();
	if (!writing_file.is_open()) { assert(0); }

	writing_file.close();
}

void ScreenUIManager::UnLoad()
{
	object.clear();
}

