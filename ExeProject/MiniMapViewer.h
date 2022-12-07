#pragma once
#include "MiniMap.h"
#include <GatesEngine/Header/GameFramework/Component/Component.h>

class MiniMapViewer : public GE::Component
{
protected:
	MiniMap miniMap;
public:
	MiniMapViewer();
	~MiniMapViewer();
	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void LateDraw() override;
};
