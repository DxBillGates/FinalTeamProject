#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>


class Ranking
{
public:
	int currentScore;
	std::vector<int> score;
public:
	static Ranking* GetInstance();
	void Start();
	void Update(float deltaTime);
	void SetRanking(int currentScore);
	void UnLoad();
private:
	Ranking() = default;

	~Ranking() = default;
	void operator=(const Ranking& obj) = delete;
	Ranking(const Ranking& obj) = delete;
};