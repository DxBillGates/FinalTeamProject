#pragma once
#include "Component.h"
#include "..\..\Graphics\CBufferStruct.h"

namespace GE
{
	class DirectionalLight : public Component
	{
	private:
		Math::Vector3 drawRange;
		Math::Vector3 direction;
		Math::Vector3 up;
		Math::Vector2 angleXY;
		// light���猩���s��
		Math::Matrix4x4 lightMatrix;
		Math::Matrix4x4 projectionMatrix;
		// light * proj �s��
		Math::Matrix4x4 vpMatrix;

		Transform* target;
	private:
		// ���g�̉�]�l��������x�N�g�����擾
		GE::Math::Vector3 CalclateDirection();
		GE::Math::Vector3 CalclateUp();
	public:
		DirectionalLight();
		~DirectionalLight();
		void Start() override;
		void Update(float deltaTime) override;
		void OnGui() override;
		void SetDirectionalLightInfo();
		const Math::Matrix4x4& GetViewMatrix();
		const Math::Matrix4x4& GetProjectionMatrix();
		const Math::Matrix4x4 GetVPMatrix();
		// ���z�̈ʒu����V����uv���v�Z�����l��Ԃ�
		Math::Vector2 GetUV();
		void SetTarget(Transform* transform);
	};
}
