#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>

	class PlayerComponent : public GE::Component
	{
	private:
		GE::InputDevice* inputDevice;
		GE::Math::Vector3 random;
		float speed;
		GE::Math::Vector3 gyro;

		GE::Math::Vector3 gravity;
		GE::Math::Vector3 move;

		float moveHoldCount;

		bool stop_move;	//滑空の停止と再生＊デバッグ用
		float dir;		//追従するカメラの角度
		float cameraDistance;//カメラとPlayerの距離
		
	public:
		PlayerComponent();

		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;
		void Draw() override;
		void LateDraw() override;
		void OnCollision(GE::GameObject* other) override;
		void OnCollision(GE::ICollider* hitCollider) override;
		void OnGui() override;
	private:
		void Control();
	};