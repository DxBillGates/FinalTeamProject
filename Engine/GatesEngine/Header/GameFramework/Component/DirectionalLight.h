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
		// lightから見た行列
		Math::Matrix4x4 lightMatrix;
		Math::Matrix4x4 projectionMatrix;
		// light * proj 行列
		Math::Matrix4x4 vpMatrix;

		Transform* target;
	private:
		// 自身の回転値から方向ベクトルを取得
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
		// 太陽の位置から天球のuvを計算した値を返す
		Math::Vector2 GetUV();
		void SetTarget(Transform* transform);
	};
}
