#include<GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Graphics/SkinMeshAnimator.h>

class ClearBird:public GE::Component
{
private:
	GE::Math::Vector3 target;

	GE::SkinMeshAnimator animator;
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	void SetTarget(GE::Math::Vector3 target) { this->target = target; }
};