#pragma once
class TimeLimit
{
private:
	int timer = 1;//��������
	int minutes = 0;//����
	int tenSeconds = 0;//�b���̏\�̈�
	int oneSeconds = 0;//�b���̈�̈�
	int fps = 144;//�t���[�����[�g�̐��l

	
public:
	TimeLimit(const int& timer);

	void Start();
	void Update();
	void Draw();
};

