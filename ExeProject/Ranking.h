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
	//�t�@�C���ɕۑ�����Ă�����W�擾
	void LoadRanking(const std::string& filename);
	//���݂̍��W���t�@�C���ɕۑ�
	void SaveRanking(const std::string& filename);
	void UnLoad();
private:
	Ranking() = default;

	~Ranking() = default;
	void operator=(const Ranking& obj) = delete;
	Ranking(const Ranking& obj) = delete;
};