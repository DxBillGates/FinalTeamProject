#include "Ranking.h"

#include<fstream>
#include<sstream>
#include<cassert>

Ranking* Ranking::GetInstance()
{
	static Ranking instance;
	return &instance;
}

void Ranking::Start()
{
	currentScore = 0;
	score.resize(5);
	LoadRanking("Resources/dashmode_score.txt");
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
			SaveRanking("Resources/dashmode_score.txt");
			return;
		}
	}
}

void Ranking::LoadRanking(const std::string& filename)
{

	std::ifstream file;
	//�t�@�C�����J��
	file.open(filename);
	//�t�@�C�����J���Ȃ�������Ƃ܂�
	if (file.fail()) {
		assert(0);
	}

	// 1�s���ǂݍ���
	std::string line;
	while (getline(file, line)) {

		// 1�s���̕�����
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "no1") {
			line_stream >> score[0];
		}
		else if (key == "no2")
		{
			line_stream >> score[1];
		}
		else if (key == "no3")
		{
			line_stream >> score[2];
		}
		else if (key == "no4")
		{
			line_stream >> score[3];
		}
		else if (key == "no5")
		{
			line_stream >> score[4];
		}
	}
	file.close();
}

void Ranking::SaveRanking(const std::string& filename)
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//���e������
	writing_file.clear();
	if (!writing_file.is_open()) { assert(0); }
	//�X�R�A�ۑ�
	writing_file << "no1 " << score[0] << std::endl;
	writing_file << "no2 " << score[1] << std::endl;
	writing_file << "no3 " << score[2] << std::endl;
	writing_file << "no4 " << score[3] << std::endl;
	writing_file << "no5 " << score[4] << std::endl;
	writing_file.close();
}
