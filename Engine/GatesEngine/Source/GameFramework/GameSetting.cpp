#include "..\..\Header\GameFramework\GameSetting.h"
#include "..\..\Header\Util\Math\Easing.h"
#include "..\..\Header\Util\Math\Math.h"

float GE::GameSetting::Time::deltaTime = 0;
float GE::GameSetting::Time::applicationElapsedTime = 0;
float GE::GameSetting::Time::gameTime = 0;
float GE::GameSetting::Time::beforeGameTime = 0;
float GE::GameSetting::Time::afterTime = 0;
GE::FlagController GE::GameSetting::Time::gameTimeFlagController = {};
bool GE::GameSetting::Time::isLerp = 0;

void GE::GameSetting::Time::Initialize()
{
	gameTime = 1;
	beforeGameTime = 0;
	afterTime = 0;
	gameTimeFlagController.Initialize();
	isLerp = false;
}

void GE::GameSetting::Time::Start()
{
	deltaTime = 0;
	applicationElapsedTime = 0;
	gameTime = 1;
	beforeGameTime = 0;
	afterTime = 0;
	gameTimeFlagController.Initialize();
	isLerp = false;
}

void GE::GameSetting::Time::Update(float frameTime)
{
	deltaTime = frameTime;
	applicationElapsedTime += deltaTime;

	// ÉQÅ[ÉÄì‡éûä‘Ç™ê›íËÇ≥ÇÍÇΩÇ∆Ç´ÇÃèàóù
	if (gameTimeFlagController.GetFlag() == false)return;

	if (isLerp)
	{
		gameTime = Math::Lerp(beforeGameTime, afterTime, Math::Easing::EaseInCirc(gameTimeFlagController.GetTime()));
	}

	if (gameTimeFlagController.GetOverTimeTrigger())
	{
		gameTime = afterTime;
		gameTimeFlagController.Initialize();
		return;
	}

	gameTimeFlagController.Update(deltaTime);
}

float GE::GameSetting::Time::GetDeltaTime()
{
	return deltaTime;
}

float GE::GameSetting::Time::GetApplicationElapsedTime()
{
	return applicationElapsedTime;
}

float GE::GameSetting::Time::GetGameTime()
{
	return gameTime;
}

void GE::GameSetting::Time::SetGameTime(float setTime, float takeTime,bool switchOptionIsLerp)
{
	if (takeTime == 0)
	{
		gameTime = setTime;
		return;
	}

	gameTimeFlagController.Initialize();
	gameTimeFlagController.SetMaxTimeProperty(takeTime);
	gameTimeFlagController.SetFlag(true);

	afterTime = setTime;
	isLerp = switchOptionIsLerp;
}
