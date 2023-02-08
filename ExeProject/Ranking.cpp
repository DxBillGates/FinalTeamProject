#include "Ranking.h"
Ranking* Ranking::GetInstance()
{
	static Ranking instance;
	return &instance;
}

void Ranking::Start()
{
	currentScore = 0;
	score.resize(5);
	score = { 4,0,0,0,0 };
}

void Ranking::SetRanking(int currentScore)
{
	this->currentScore = currentScore;
	for (int i = 0; i < score.size(); i++)
	{
		if (score[i] < this->currentScore)
		{
			score.insert(score.begin() + i, this->currentScore);
			score.pop_back();
			return;
		}
	}
}
