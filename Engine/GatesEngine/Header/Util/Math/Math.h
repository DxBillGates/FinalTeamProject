#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Easing.h"

namespace GE
{
	namespace Math
	{
		struct GaussFilterData
		{
			GE::Math::Vector4 data[16];
		};

		const float PI = 3.14159265f;

		/// <summary>
		/// �x���@����ʓx�@�ɕϊ�����֐�
		/// </summary>
		/// <param name="value">�x��</param>
		/// <returns>���W�A��</returns>
		float ConvertToRadian(float value);

		float ConvertToAngle(float value);

		/// <summary>
		/// ���`��Ԋ֐�
		/// </summary>
		/// <param name="s">�X�^�[�g</param>
		/// <param name="e">�G���h</param>
		/// <param name="t">�^�C��</param>
		/// <returns>���`��Ԃ̒l</returns>
		float Lerp(float s, float e, float t);

		/// <summary>
		/// x��min max�̊ԂŕԂ�
		/// ��) (x = 1000,min = 0,max = 100)�̂Ƃ�return = 100
		/// </summary>
		/// <param name="x">��r�������l</param>
		/// <param name="min"></param>
		/// <param name="max"></param>
		/// <returns></returns>
		float Clamp(float x, float min, float max);

		/// <summary>
		/// value�̕�����Ԃ�
		/// </summary>
		/// <param name="value">�m�F�������l</param>
		/// <returns>����1 or -1</returns>
		int GetSign(float value);

		// �K�E�V�A���t�B���^�̏d�݂��v�Z����֐�
		float GaussFilter(const GE::Math::Vector2& pos, float value);
		// �Q�ƃs�N�Z���ւ̏��ƃK�E�V�A���t�B���^�̏d�݂��v�Z���ăZ�b�g����֐�
		void SetGaussFilterData(const GE::Math::Vector2& size, GE::Math::Vector2& dir, float gaussValue, GaussFilterData* data, int dataSize);

		/// <summary>
		/// �X�N���[���̈�_����L�т郌�C�����擾
		/// </summary>
		/// <param name="pos">�X�N���[�����W�@��)�}�E�X���W�Ȃ�</param>
		/// <param name="outPos">���C�̎n�_���Q�Ɠn���Ŏ擾</param>
		/// <param name="outDir">���C�̕������Q�Ɠn���Ŏ擾</param>
		/// <param name="view">�r���[�s��</param>
		/// <param name="proj">�v���W�F�N�V�����s��</param>
		/// <param name="viewPort">�r���[�|�[�g�s��</param>
		void GetScreenToRay(const Vector2& pos,Vector3* outPos, Vector3* outDir,const Matrix4x4& view,const Matrix4x4& proj,const Matrix4x4& viewPort);
	}
}
