#pragma once
#include "MiniMap.h"
#include "PlayerComponent.h"
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class MiniMapViewer : public GE::Component
{
protected:
	MiniMap miniMap;
	PlayerComponent* player;
	GE::Math::Vector3 direction;
public:
	MiniMapViewer();
	~MiniMapViewer();
	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void LateDraw() override;
	void SetPlayer(PlayerComponent* comp);
};
