#include "MiniMapViewer.h"
#include <cmath>

MiniMapViewer::MiniMapViewer()
{
}

MiniMapViewer::~MiniMapViewer()
{
}

void MiniMapViewer::Awake()
{
}

void MiniMapViewer::Start()
{
	miniMap.Initialize(gameObject->GetGameObjectManager(), graphicsDevice);
}

void MiniMapViewer::Update(float deltaTime)
{
	if (player == nullptr)return;

	float oldAngle = direction.y;
	direction = player->GetDirection();
	float directionDifference = oldAngle - direction.y;

	miniMap.Update(deltaTime,directionDifference);
}

void MiniMapViewer::LateDraw()
{
	miniMap.Draw();
}

void MiniMapViewer::SetPlayer(PlayerComponent* comp)
{
	player = comp;
}
