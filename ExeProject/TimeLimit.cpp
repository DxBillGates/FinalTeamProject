#include "TimeLimit.h"

TimeLimit::TimeLimit(const int& timer)
{
	this->timer = timer * 60 * fps;//分数を秒数に直して144fpsをかける
}

void TimeLimit::Start()
{
}

void TimeLimit::Update()
{
	timer--;//カウントダウン
	minutes = timer / fps / 60;//分数の計算
	tenSeconds = timer / fps % 60 / 10;//秒数の十の位の計算
	oneSeconds= timer / fps % 60 % 10;//秒数の一の位の計算
}

void TimeLimit::Draw()
{
}
