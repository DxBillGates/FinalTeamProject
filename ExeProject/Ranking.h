#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>


class Ranking
{
public:
	int currentScore;
	std::vector<int> score;
	std::vector<bool> isRankin;
public:
	static Ranking* GetInstance();
	void Start();
	void Update(float deltaTime);
	void SetRanking(int currentScore);
	//ファイルに保存されている座標取得
	void LoadRanking(const std::string& filename);
	//現在の座標をファイルに保存
	void SaveRanking(const std::string& filename);
	void UnLoad();
private:
	Ranking() = default;

	~Ranking() = default;
	void operator=(const Ranking& obj) = delete;
	Ranking(const Ranking& obj) = delete;
};