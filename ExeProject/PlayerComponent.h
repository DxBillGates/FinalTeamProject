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

		bool stop_move;	//σΜβ~ΖΔΆfobOp
		float dir;		//Η]·ιJΜpx
		float cameraDistance;//JΖPlayerΜ£
		
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