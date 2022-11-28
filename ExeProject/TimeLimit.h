#pragma once
class TimeLimit
{
private:
	int timer = 1;//制限時間
	int minutes = 0;//分数
	int tenSeconds = 0;//秒数の十の位
	int oneSeconds = 0;//秒数の一の位
	int fps = 144;//フレームレートの数値

	
public:
	TimeLimit(const int& timer);

	void Start();
	void Update();
	void Draw();
};

