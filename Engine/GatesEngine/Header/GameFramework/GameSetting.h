#pragma once
#include "FlagController.h"

namespace GE
{
	namespace GameSetting
	{
		class Time
		{
		private:
			static float deltaTime;
			static float applicationElapsedTime;

			static float gameTime;
			static float beforeGameTime;
			static float afterTime;
			static FlagController gameTimeFlagController;
			static bool isLerp;
		public:
			/// <summary>
			/// �f���^�^�C���A�A�v���o�ߎ��ԈȊO��������
			/// </summary>
			static void Initialize();

			/// <summary>
			/// �A�v���N������x�̂݌Ă�
			/// </summary>
			static void Start();

			/// <summary>
			/// ���t���[���Ă�
			/// </summary>
			/// <param name="frameTime">�O�t���[���̌o�ߎ���</param>
			static void Update(float frameTime);

			/// <summary>
			/// �O�t���[���̌o�ߎ���(sec)���擾
			/// </summary>
			/// <returns>�O�t���[���̌o�ߎ���(sec)</returns>
			static float GetDeltaTime();

			/// <summary>
			/// �A�v�����N�����Ă��獡�܂ł̌o�ߎ���(sec)���擾
			/// </summary>
			/// <returns>�A�v�����N�����Ă��獡�܂ł̌o�ߎ���(sec)</returns>
			static float GetApplicationElapsedTime();

			/// <summary>
			/// �Q�[�������Ԃ��擾
			/// </summary>
			/// <returns>�Q�[��������</returns>
			static float GetGameTime();

			/// <summary>
			/// �Q�[�������Ԃ�ݒ�
			/// </summary>
			/// <param name="setTime">�ݒ肷�鎞�� �Q�[���𓮂������x 0 = �����Ȃ� 1 = �ʏ� 2 = �{�� 3...</param>
			/// <param name="takeTime">���f������܂ł̎���(sec)</param>
			/// <param name="isLerp">�C�[�W���O�Őݒ莞�Ԃ܂ŏ��������邩</param>
			static void SetGameTime(float setTime, float takeTime = 0,bool switchOptionIsLerp = false);
		};
	}
}
