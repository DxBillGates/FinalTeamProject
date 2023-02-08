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
#include"Ranking.h"
#include "InputManager.h"

ScreenUIManager* ScreenUIManager::GetInstance()
{
	static ScreenUIManager instance;
	return &instance;
}

ScreenUIManager::SpriteInfo ScreenUIManager::Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName)
{
	GE::Math::Vector2 currentWindowSizeDiff = GE::Window::GetDiffWindowSize();
	SpriteInfo result;
	result.transform.position = { pos.x * currentWindowSizeDiff.x,pos.y * currentWindowSizeDiff.y,0 };
	result.transform.scale = { scale.x * currentWindowSizeDiff.x,scale.y * currentWindowSizeDiff.y,0 };
	result.color = color;
	result.textureName = textureName;
	return result;
}

ScreenUIManager::SpriteInfo ScreenUIManager::Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName, GE::Math::Vector2 texSize, GE::Math::Vector2 clipSize)
{
	GE::Math::Vector2 currentWindowSizeDiff = GE::Window::GetDiffWindowSize();
	SpriteInfo result;
	result.transform.position = { pos.x * currentWindowSizeDiff.x,pos.y * currentWindowSizeDiff.y,0 };
	result.transform.scale = { scale.x * currentWindowSizeDiff.x,scale.y * currentWindowSizeDiff.y,0 };
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
	object["title_endless"].isDraw = isActive;
	object["title_exit"].isDraw = isActive;
}

void ScreenUIManager::OptionMenuActive(bool isActive)
{
	object["bgm_num"].isDraw = isActive;
	object["se_num"].isDraw = isActive;
	object["bgm_info"].isDraw = isActive;
	object["se_info"].isDraw = isActive;
	object["option_back"].isDraw = isActive;
	object["option_right"].isDraw = isActive;
	object["option_left"].isDraw = isActive;
}
void ScreenUIManager::ResultActive(bool isActive)
{
	object["ranking_info"].isDraw = isActive;
	object["1st"].isDraw = isActive;
	object["1st_score_one"].isDraw = isActive;
	object["1st_score_ten"].isDraw = isActive;
	object["1st_score_hnd"].isDraw = isActive;
	object["1st_kurai"].isDraw = isActive;
	object["2nd"].isDraw = isActive;
	object["2nd_score_one"].isDraw = isActive;
	object["2nd_score_ten"].isDraw = isActive;
	object["2nd_score_hnd"].isDraw = isActive;
	object["2nd_kurai"].isDraw = isActive;
	object["3rd"].isDraw = isActive;
	object["3rd_score_one"].isDraw = isActive;
	object["3rd_score_ten"].isDraw = isActive;
	object["3rd_score_hnd"].isDraw = isActive;
	object["3rd_kurai"].isDraw = isActive;

}
void ScreenUIManager::SetAudioManager(GE::AudioManager* a)
{
	audioManager = a;
}
float ScreenUIManager::SetLerp(std::string name, float lerpTime, float deltaTime)
{
	if (object[name].lerpCount < lerpTime)
	{
		object[name].lerpCount += deltaTime;
		object[name].isLerp = true;
		return object[name].lerpCount / lerpTime;
	}
	else
	{
		if (object[name].isLerp)
		{
			audioManager->Use("click")->Start();
		}
		object[name].isLerp = false;
		return 1.0f;
	}
}
void ScreenUIManager::SampleSceneStart()
{
	winSize = GE::Window::GetWindowSize();
	center = winSize / 2.0f;
	a = 0.0f;

	//微調整の座標手打ちだから環境でずれそう
#pragma region オプション
	object["bgm_num"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["se_num"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["bgm_info"] = Set({}, { 200, 100, 0 }, GE::Color::White(), "bgm_info_tex");
	object["se_info"] = Set({}, { 100,100,0 }, GE::Color::White(), "se_info_tex");
	object["option_back"] = Set({}, { 150, 100, 0 }, GE::Color::White(), "texture_back");
	object["option_right"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_next");
	object["option_right"].transform.rotation = GE::Math::Quaternion::Euler({ 0, 0, 180 });
	object["option_left"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_next");
#pragma endregion

#pragma region タイトル
	object["title_name"] = Set(GE::Math::Vector3(1500, winSize.y / 2 - 220.0f, 0.0f), { 1319 / 2.0f,642 / 2.0f,0 }, GE::Color::White(), "texture_title");
	object["title_start"] = Set(GE::Math::Vector3(winSize.x - 300, center.y, 0.0f), { 300,100,0 }, GE::Color::White(), "texture_start");
	object["title_endless"] = Set(GE::Math::Vector3(winSize.x - 300, center.y + 100, 0.0f), { 300,100,0 }, GE::Color::White(), "endless_tex");
	object["title_option"] = Set(GE::Math::Vector3(winSize.x - 300, center.y + 200, 0.0f), { 300,100,0 }, GE::Color::White(), "texture_option");
	object["title_exit"] = Set(GE::Math::Vector3(winSize.x - 300, center.y + 300, 0.0f), { 300,100,0 }, GE::Color::White(), "texture_exit");
#pragma endregion

#pragma region タイムリミット
	object["time_minutes"] = Set(GE::Math::Vector3(80, 950, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["time_symbol"] = Set(GE::Math::Vector3(160, 950, 0.0f), { 50,100,0 }, GE::Color::White(), "texture_symbol", { 64,64 }, { 32,64 });
	object["time_tenSeconds"] = Set(GE::Math::Vector3(240, 950, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["time_oneSeconds"] = Set(GE::Math::Vector3(340, 950, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
#pragma endregion
	object["crash"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), { 400,100,0 }, GE::Color::White(), "crash_info_tex");
	object["crash_keyboard"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), { 400,70,0 }, GE::Color::White(), "crash_info_keyboard_tex");
	object["lockon_info"] = Set(GE::Math::Vector3(217, 600, 0.0f), GE::Math::Vector3(600, 64, 0) * 0.6f, GE::Color::White(), "lockon_info_tex");
	object["lockon_info_keyboard"] = Set(GE::Math::Vector3(217 + 21, 600, 0.0f), GE::Math::Vector3(664, 64, 0) * 0.6f, GE::Color::White(), "lockon_info_keyboard_tex");
	object["dash_info"] = Set(GE::Math::Vector3(248, 670, 0.0f), GE::Math::Vector3(384, 64, 0) * 1.1f, GE::Color::White(), "control_info_1_tex", { 768,384 }, { 768, 128 });
	object["dash_info"].pivotPos = 2;
	object["push_b"] = Set(GE::Math::Vector3(winSize.x - 300.f, winSize.y - 100.f, 0.f), GE::Math::Vector3(256, 64, 0) * 0.6f, GE::Color::White(), "push_b_tex");
	object["is_lockon_info"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), GE::Math::Vector3(400, 70, 0), GE::Color::White(), "is_lockon_info_tex");
	object["search_info"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), GE::Math::Vector3(384, 128, 0) * 0.6f, GE::Color::White(), "search_info_tex");

	object["dash_info_keyboard"] = Set(GE::Math::Vector3(188, 670, 0.0f), GE::Math::Vector3(256, 64, 0) * 1.1f, GE::Color::White(), "control_info_keyboard_tex", { 512,384 }, { 512, 128 });
	object["dash_info_keyboard"].pivotPos = 2;
	object["push_space"] = Set(GE::Math::Vector3(winSize.x - 300.f, winSize.y - 100.f, 0.f), GE::Math::Vector3(400, 70, 0) * 0.57f, GE::Color::White(), "push_space_tex");

	object["crash_xctrl"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), { 400,70,0 }, GE::Color::White(), "clash_info_xctrl_tex");
	object["lockon_info_xctrl"] = Set(GE::Math::Vector3(217 + 21, 600, 0.0f), GE::Math::Vector3(664, 64, 0) * 0.6f, GE::Color::White(), "lockon_info_xctrl_tex");
	object["push_a"] = Set(GE::Math::Vector3(winSize.x - 300.f, winSize.y - 100.f, 0.f), GE::Math::Vector3(256, 64, 0) * 0.6f, GE::Color::White(), "push_a_tex");
	object["dash_info_xctrl"] = Set(GE::Math::Vector3(188, 670, 0.0f), GE::Math::Vector3(256, 64, 0) * 1.1f, GE::Color::White(), "control_info_xctrl_tex", { 512,384 }, { 512, 128 });
	object["dash_info_xctrl"].pivotPos = 2;

	//遷移の値初期化
	for (auto o : object)
	{
		o.second.lerpCount = 0.0f;
	}
}

void ScreenUIManager::DashModeStart()
{
	object["combo"] = Set(GE::Math::Vector3(1500, center.y - 220.0f, 0.0f), { 256,128,0 }, GE::Color::White(), "combo_tex");
	object["comboNum"] = Set(GE::Math::Vector3(1300, center.y - 220.0f, 0.0f), { 128,128,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });

	object["take_info"] = Set(GE::Math::Vector3(217, 100, 0.0f), GE::Math::Vector3(400, 100, 0) * 0.6f, GE::Color::White(), "take_info_tex");
	object["take_hnd"] = Set(GE::Math::Vector3(400, 100, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["take_ten"] = Set(GE::Math::Vector3(500, 100, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["take_one"] = Set(GE::Math::Vector3(600, 100, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });

#pragma region タイムリミット
	object["time_minutes"] = Set(GE::Math::Vector3(80, 950, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["time_symbol"] = Set(GE::Math::Vector3(160, 950, 0.0f), { 50,100,0 }, GE::Color::White(), "texture_symbol", { 64,64 }, { 32,64 });
	object["time_tenSeconds"] = Set(GE::Math::Vector3(240, 950, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["time_oneSeconds"] = Set(GE::Math::Vector3(340, 950, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
#pragma endregion

	object["keyboard_info"] = Set(GE::Math::Vector3(winSize.x - 300, center.y + 100, 0.0f), GE::Math::Vector3(386, 64, 0), GE::Color::White(), "keyboard_info_tex");
#pragma region ランキング

	GE::Math::Vector3 scrPos = { 600,0,0 };
	GE::Math::Vector3 kuraiPos = { 100,0,0 };
	object["ranking_info"] = Set(GE::Math::Vector3(center.x, center.y / 2.0f, 0.0f), { 256,64,0 }, GE::Color::White(), "ranking_info_tex");

	object["1st"] = Set(GE::Math::Vector3(center.x / 1.5f, center.y, 0.0f), { 100,100,0 }, { 0.8f,0.8f,0.0f,1.0f }, "texture_Number", { 320,64 }, { 32,64 });
	object["1st"].pivotPos = 1;
	object["1st_kurai"] = Set(object["1st"].transform.position + kuraiPos, { 100,100,0 }, { 0.8f,0.8f,0.0f,1.0f }, "kurai_tex");
	object["1st_score_one"] = Set(object["1st"].transform.position + scrPos, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["1st_score_ten"] = Set(object["1st"].transform.position + scrPos / 1.2f, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["1st_score_hnd"] = Set(object["1st"].transform.position + scrPos / 1.5f, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });

	object["2nd"] = Set(GE::Math::Vector3(center.x / 1.5f, center.y + 100, 0.0f), { 100,100,0 }, { 0.8f,0.8f ,0.8f ,1.0f }, "texture_Number", { 320,64 }, { 32,64 });
	object["2nd"].pivotPos = 2;
	object["2nd_kurai"] = Set(object["2nd"].transform.position + kuraiPos, { 100,100,0 }, { 0.8f,0.8f,0.8f,1.0f }, "kurai_tex");
	object["2nd_score_one"] = Set(object["2nd"].transform.position + scrPos, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["2nd_score_ten"] = Set(object["2nd"].transform.position + scrPos / 1.2f, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["2nd_score_hnd"] = Set(object["2nd"].transform.position + scrPos / 1.5f, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });

	object["3rd"] = Set(GE::Math::Vector3(center.x / 1.5f, center.y + 200, 0.0f), { 100,100,0 }, { 0.5f,0.2f,0.2f,1.0f }, "texture_Number", { 320,64 }, { 32,64 });
	object["3rd"].pivotPos = 3;
	object["3rd_kurai"] = Set(object["3rd"].transform.position + kuraiPos, { 100,100,0 }, { 0.5f,0.2f,0.2f,1.0f }, "kurai_tex");
	object["3rd_score_one"] = Set(object["3rd"].transform.position + scrPos, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["3rd_score_ten"] = Set(object["3rd"].transform.position + scrPos / 1.2f, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["3rd_score_hnd"] = Set(object["3rd"].transform.position + scrPos / 1.5f, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
#pragma endregion

	object["title_name"] = Set(GE::Math::Vector3(1500, winSize.y / 2 - 220.0f, 0.0f), { 1319 / 2.0f,642 / 2.0f,0 }, GE::Color::White(), "title_dashmode_tex");

	//遷移の値初期化
	for (auto o : object)
	{
		o.second.lerpCount = 0.0f;
	}
}
void ScreenUIManager::SampleSceneUpdate(float deltaTime)
{
	const float f = 144.0f / (1.0f / deltaTime);
	const float addCount = 0.1f * f;
	GE::Math::Vector3 vive = Vivlate(f);
	//Title
	TitleMenuActive(false);

	object["crash"].isDraw = false;
	object["crash_keyboard"].isDraw = false;
	object["control_info_2"].isDraw = true;
	object["lockon_info"].isDraw = false;
	object["lockon_info_keyboard"].isDraw = false;
	object["dash_info"].isDraw = false;
	object["dash_info_keyboard"].isDraw = false;
	object["is_lockon_info"].isDraw = false;
	object["search_info"].isDraw = false;
	object["push_b"].isDraw = false;
	object["push_space"].isDraw = false;
	object["crash_xctrl"].isDraw = false;
	object["lockon_info_xctrl"].isDraw = false;
	object["push_a"].isDraw = false;
	object["dash_info_xctrl"].isDraw = false;

	TimeLimitActive(f);
	if (TimeLimit::GetInstance()->GetLimit())
	{
		object["time_minutes"].color = GE::Color::Red();
		object["time_minutes"].transform.scale += TimeLimit::GetInstance()->GetTScale();
		if (object["time_minutes"].transform.scale.x < 100)
		{
			object["time_minutes"].transform.scale = { 100,100,0 };
		}

		object["time_tenSeconds"].color = GE::Color::Red();
		object["time_tenSeconds"].transform.scale += TimeLimit::GetInstance()->GetTScale();
		if (object["time_tenSeconds"].transform.scale.x < 100)
		{
			object["time_tenSeconds"].transform.scale = { 100,100,0 };
		}

		object["time_oneSeconds"].color = GE::Color::Red();
		object["time_oneSeconds"].transform.scale += TimeLimit::GetInstance()->GetTScale();
		if (object["time_oneSeconds"].transform.scale.x < 100)
		{
			object["time_oneSeconds"].transform.scale = { 100,100,0 };
		}

		object["time_symbol"].color = GE::Color::Red();
	}
	else
	{
		object["time_minutes"].color = GE::Color::White();
		object["time_minutes"].transform.scale = { 100,100,0 };

		object["time_tenSeconds"].color = GE::Color::White();
		object["time_tenSeconds"].transform.scale = { 100,100,0 };

		object["time_oneSeconds"].color = GE::Color::White();
		object["time_oneSeconds"].transform.scale = { 100,100,0 };

		object["time_symbol"].color = GE::Color::White();
	}

	auto inputManager = InputManager::GetInstance();
	auto inputDeviceState = inputManager->GetCurrentInputDeviceState();

	switch (PlayerComponent::statas)
	{
	case PlayerComponent::PlayerStatas::CRASH:
		if (inputDeviceState == InputManager::InputDeviceState::JOYCON) {
			object["crash"].isDraw = true;
			object["crash"].transform.scale = GE::Math::Vector3(600, 64, 0) + GE::Math::Vector3(sin(object["crash"].lerpCount), sin(object["crash"].lerpCount) / 6.4f, 0) * 50;
			object["crash"].lerpCount += 0.05f * f;
		}
		else if (inputDeviceState == InputManager::InputDeviceState::KEYBOARD)
		{
			object["crash_keyboard"].transform.scale = GE::Math::Vector3(400, 70, 0) + GE::Math::Vector3(sin(object["crash_keyboard"].lerpCount), sin(object["crash_keyboard"].lerpCount) / 7.0f, 0) * 50;
			object["crash_keyboard"].isDraw = true;
			object["crash_keyboard"].lerpCount += 0.05f * f;
		}
		else if (inputDeviceState == InputManager::InputDeviceState::XCTRL)
		{
			object["crash_xctrl"].transform.scale = GE::Math::Vector3(400, 70, 0) + GE::Math::Vector3(sin(object["crash_keyboard"].lerpCount), sin(object["crash_keyboard"].lerpCount) / 7.0f, 0) * 50;
			object["crash_xctrl"].isDraw = true;
			object["crash_xctrl"].lerpCount += 0.05f * f;
		}
		break;
	case PlayerComponent::PlayerStatas::TITLE:
		object["title_name"].isDraw = true;
		if (inputDeviceState == InputManager::InputDeviceState::JOYCON) { object["push_b"].isDraw = true; }
		else if (inputDeviceState == InputManager::InputDeviceState::KEYBOARD) { object["push_space"].isDraw = true; }
		else if (inputDeviceState == InputManager::InputDeviceState::XCTRL) { object["push_a"].isDraw = true; }

		break;
	case PlayerComponent::PlayerStatas::TITLE_MENU:
		//タイトル描画
		TitleMenuActive(true);
		if (!Title::GetInstance()->GetSelect(Title::States::option))
		{
			//横からフェードイン
			object["title_start"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y, 0.0f), SetLerp("title_start", 5.0f, addCount));
			object["title_endless"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 100, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 100, 0.0f), SetLerp("title_endless", 6.0f, addCount));
			object["title_option"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 200, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 200, 0.0f), SetLerp("title_option", 7.0f, addCount));
			object["title_exit"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 300, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 300, 0.0f), SetLerp("title_exit", 8.0f, addCount));
		}
		if (inputDeviceState == InputManager::InputDeviceState::JOYCON) { object["push_b"].isDraw = true; }
		else if (inputDeviceState == InputManager::InputDeviceState::KEYBOARD) { object["push_space"].isDraw = true; }
		else if (inputDeviceState == InputManager::InputDeviceState::XCTRL) { object["push_a"].isDraw = true; }

		break;
	case PlayerComponent::PlayerStatas::STAY_TREE:
		object["title_name"].isDraw = false;

		break;
	case PlayerComponent::PlayerStatas::LOCKON_SHOOT:
		object["is_lockon_info"].isDraw = true;
		object["is_lockon_info"].transform.position = GE::Math::Vector3(center.x, center.y - 150.f, 0.f) + vive;

		break;
	case PlayerComponent::PlayerStatas::MOVE:
		if (inputDeviceState == InputManager::InputDeviceState::JOYCON) {
			object["lockon_info"].isDraw = true;
		}
		else if (inputDeviceState == InputManager::InputDeviceState::KEYBOARD)
		{
			object["lockon_info_keyboard"].isDraw = true;
		}
		else if (inputDeviceState == InputManager::InputDeviceState::XCTRL)
		{
			object["lockon_info_xctrl"].isDraw = true;
		}

		if (inputDeviceState == InputManager::InputDeviceState::JOYCON) { object["dash_info"].isDraw = true; }
		else if (inputDeviceState == InputManager::InputDeviceState::KEYBOARD) { object["dash_info_keyboard"].isDraw = true; }
		else if (inputDeviceState == InputManager::InputDeviceState::XCTRL) { object["dash_info_xctrl"].isDraw = true; }

		switch (PlayerComponent::lockonState)
		{
		case PlayerComponent::LockOnState::SEARCH:
			object["search_info"].isDraw = true;
			break;
		case PlayerComponent::LockOnState::LOCKON_SLOW:
			object["is_lockon_info"].isDraw = true;
			object["is_lockon_info"].transform.position = GE::Math::Vector3(center.x, center.y - 150.f, 0.f) + vive;
			viveVelocity = { 20,20 };
			break;
		}

		break;
	}
#pragma region オプション中
	OptionMenuActive(false);
	if (Title::GetInstance()->GetSelect(Title::States::option))
	{
		TitleMenuActive(false);
		OptionMenuActive(true);
		object["option_right"].transform.position.x = GE::Math::Lerp(winSize.x + 1000, winSize.x - 200, SetLerp("bgm_num", 5.0f, addCount));
		object["option_left"].transform.position.x = GE::Math::Lerp(winSize.x + 1000, winSize.x - 450, SetLerp("bgm_num", 5.0f, addCount));

		object["bgm_num"].pivotPos.x = OptionData::BGM_vol;
		object["bgm_num"].color = GE::Color::White();
		object["bgm_num"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y, 0.0f), SetLerp("bgm_num", 5.0f, addCount));
		object["bgm_info"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 700, center.y, 0.0f), GE::Math::Vector3(winSize.x - 600, center.y, 0.0f), SetLerp("bgm_num", 5.0f, addCount));

		if (Option::select == Option::Select::BGM_VOL) { object["bgm_num"].color = GE::Color::Red(); }
		object["se_num"].pivotPos.x = OptionData::SE_vol;
		object["se_num"].color = GE::Color::White();
		object["se_num"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 100, 0.0f), GE::Math::Vector3(winSize.x - 305, center.y + 100, 0.0f), SetLerp("se_num", 6.0f, addCount));
		object["se_info"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 700, center.y + 100, 0.0f), GE::Math::Vector3(winSize.x - 605, center.y + 100, 0.0f), SetLerp("se_num", 6.0f, addCount));

		if (Option::select == Option::Select::SE_VOL) { object["se_num"].color = GE::Color::Red(); }
		object["option_back"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 200, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 200, 0.0f), SetLerp("option_back", 5.0f, addCount));
		if (Option::select == Option::Select::Back)
		{
			object["option_back"].color = GE::Color::Red();
			object["option_right"].isDraw = false;
			object["option_left"].isDraw = false;
			//タイトルメニューの遷移初期化
			object["title_start"].lerpCount = 0.f;
			object["title_option"].lerpCount = 0.f;
			object["title_endless"].lerpCount = 0.f;
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
		object["bgm_num"].lerpCount = 0.f;
		object["se_num"].lerpCount = 0.f;
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
		object["title_endless"].color = GE::Color::White();
		object["title_exit"].color = GE::Color::White();

		switch (Title::GetInstance()->states)
		{
		case Title::States::start:
			object["title_start"].color = GE::Color::Red();
			break;
		case Title::States::option:
			object["title_option"].color = GE::Color::Red();
			break;
		case Title::States::endless:
			object["title_endless"].color = GE::Color::Red();
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
void ScreenUIManager::DashModeUpdate(float deltaTime)
{
	const float f = 144.0f / (1.0f / deltaTime);

	GE::Math::Vector3 vive = Vivlate(f);
	GE::Math::Vector3 comboPos = GE::Math::Vector3(1500, winSize.y / 2 - 220.0f, 0.0f);
	GE::Math::Vector3 comboNumPos = GE::Math::Vector3(1300, winSize.y / 2 - 220.0f, 0.0f);
	TimeLimitActive(f);
#pragma region 時間加算の演出


	object["time_tenSeconds"].transform.scale = GE::Math::Vector3(100, 100, 0) + GE::Math::Vector3(sin(object["time_tenSeconds"].lerpCount) * 30.f);
	object["time_oneSeconds"].transform.scale = GE::Math::Vector3(100, 100, 0) + GE::Math::Vector3(sin(object["time_tenSeconds"].lerpCount) * 30.f);

	object["time_tenSeconds"].lerpCount < 3.14f ? object["time_tenSeconds"].lerpCount += 0.1f * f : 0;
	float col = GE::Math::Lerp(0.f, 1.f, object["time_tenSeconds"].lerpCount / 3.14f);
	object["time_oneSeconds"].color = { col,1.f,col,1.f };
	object["time_tenSeconds"].color = { col,1.f,col,1.f };

	object["keyboard_info"].isDraw = false;
	object["title_name"].isDraw = false;

#pragma endregion
	if (PlayerComponent::combo != 0)
	{
		object["combo"].isDraw = true;
		object["comboNum"].isDraw = true;
		object["combo"].transform.position = comboPos + vive;
		object["comboNum"].transform.position = comboNumPos + vive;
		object["comboNum"].pivotPos = PlayerComponent::combo;

		object["combo"].color = GE::Color::White();
		object["comboNum"].color = GE::Color::White();
		//コンボ時のカラー
		switch (PlayerComponent::combo)
		{
		case 4:
			object["combo"].color = { 0.8f,0.8f,0.0f,1.0f };
			object["comboNum"].color = { 0.8f,0.8f,0.0f,1.0f };
			break;
		case 5:
			object["combo"].color = { 1.f,0.5f,0.0f,1.0f };
			object["comboNum"].color = { 1.f,0.5f,0.0f,1.0f };
			break;
		case 6:
			object["combo"].color = GE::Color::Red();
			object["comboNum"].color = GE::Color::Red();
			break;
		}

	}
	else
	{
		object["combo"].isDraw = false;
		object["comboNum"].isDraw = false;
	}
	ResultActive(false);
	object["take_info"].isDraw = true;
	object["take_ten"].isDraw = true;
	object["take_hnd"].isDraw = true;
	if (PlayerComponent::takeEnemyCount < 10) { object["take_ten"].isDraw = false; }
	if (PlayerComponent::takeEnemyCount < 100) { object["take_hnd"].isDraw = false; }
	object["take_one"].isDraw = true;
	object["take_one"].pivotPos = PlayerComponent::takeEnemyCount % 10;
	object["take_ten"].pivotPos = (int)(PlayerComponent::takeEnemyCount / 10);
	object["take_hnd"].pivotPos = (int)(PlayerComponent::takeEnemyCount / 100);

	switch (PlayerComponent::statas)
	{
	case PlayerComponent::PlayerStatas::TITLE:
		object["title_name"].isDraw = true;

		object["take_info"].isDraw = false;
		object["take_one"].isDraw = false;
		object["keyboard_info"].isDraw = true;
		viveVelocity = { 20,20 };
		object["keyboard_info"].transform.position = GE::Math::Vector3(winSize.x - 300, center.y + 100, 0.0f) + Vivlate(f);

		break;
	case PlayerComponent::PlayerStatas::TITLE_MENU:
		object["take_info"].isDraw = false;
		object["take_one"].isDraw = false;
		break;
	case PlayerComponent::PlayerStatas::OVER:
		ResultActive(true);
		object["1st_score_one"].pivotPos = Ranking::GetInstance()->score[0] % 10;
		object["1st_score_ten"].pivotPos = (int)(Ranking::GetInstance()->score[0] / 10);
		object["1st_score_hnd"].pivotPos = (int)(Ranking::GetInstance()->score[0] / 100);
		object["2nd_score_one"].pivotPos = Ranking::GetInstance()->score[1];
		object["2nd_score_ten"].pivotPos = (int)(Ranking::GetInstance()->score[1] / 10);
		object["2nd_score_hnd"].pivotPos = (int)(Ranking::GetInstance()->score[1] / 100);
		object["3rd_score_one"].pivotPos = Ranking::GetInstance()->score[2];
		object["3rd_score_ten"].pivotPos = (int)(Ranking::GetInstance()->score[2] / 10);
		object["3rd_score_hnd"].pivotPos = (int)(Ranking::GetInstance()->score[2] / 100);

		object["time_minutes"].isDraw = false;
		object["time_symbol"].isDraw = false;
		object["time_tenSeconds"].isDraw = false;
		object["time_oneSeconds"].isDraw = false;
		break;
	}
}

void ScreenUIManager::TimeLimitActive(float deltaTime)
{
	//タイムリミット
	object["time_minutes"].isDraw = true;
	object["time_minutes"].pivotPos = TimeLimit::GetInstance()->GetMinutes();
	object["time_tenSeconds"].isDraw = true;
	object["time_tenSeconds"].pivotPos = TimeLimit::GetInstance()->GetTenSeconds();
	object["time_oneSeconds"].isDraw = true;
	object["time_oneSeconds"].pivotPos = TimeLimit::GetInstance()->GetOneSeconds();
	object["time_symbol"].isDraw = true;

	switch (PlayerComponent::statas)
	{
	case PlayerComponent::PlayerStatas::TITLE:
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
		break;
	}
}
void ScreenUIManager::TimeLimitAddEffect()
{
	object["time_oneSeconds"].lerpCount = 0.f;
	object["time_tenSeconds"].lerpCount = 0.f;

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


GE::Math::Vector3 ScreenUIManager::Vivlate(float deltaTime)
{
	//カメラシェイクの減衰
	viveVelocity.x > 0 ? viveVelocity.x -= 1.0f * (int)deltaTime : viveVelocity.x = 0.0f;
	viveVelocity.y > 0 ? viveVelocity.y -= 1.0 * (int)deltaTime : viveVelocity.y = 0.0f;

	//ランダムで移動量
	GE::Math::Vector2 randVel = GE::Math::Vector2(viveVelocity.x > 0 ? rand() % (int)viveVelocity.x - viveVelocity.x / 2 : 0, viveVelocity.y > 0 ? rand() % (int)viveVelocity.y - viveVelocity.y / 2 : 0);
	//移動量をセットする
	return GE::Math::Vector3(randVel.x, randVel.y, 0);
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

