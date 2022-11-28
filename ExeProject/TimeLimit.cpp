#include "TimeLimit.h"

TimeLimit::TimeLimit(const int& timer)
{
	this->timer = timer * 60 * fps;//������b���ɒ�����144fps��������
}

void TimeLimit::Start()
{
}

void TimeLimit::Update()
{
	timer--;//�J�E���g�_�E��
	minutes = timer / fps / 60;//�����̌v�Z
	tenSeconds = timer / fps % 60 / 10;//�b���̏\�̈ʂ̌v�Z
	oneSeconds= timer / fps % 60 % 10;//�b���̈�̈ʂ̌v�Z
}

void TimeLimit::Draw()
{
}
