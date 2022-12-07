#include "MiniMapViewer.h"

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
	miniMap.Update(deltaTime);
}

void MiniMapViewer::LateDraw()
{
	miniMap.Draw();
}
