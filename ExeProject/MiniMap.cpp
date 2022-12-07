#include "MiniMap.h"
#include <GatesEngine/Header/Graphics/Window.h>

void MiniMap::Draw2D(const GE::Math::Vector2& drawPosition, const GE::Math::Vector2& drawSize, const GE::Color& drawColor)
{
	GE::ICBufferAllocater* cbufferAllocater = pGraphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = pGraphicsDevice->GetRenderQueue();

	pGraphicsDevice->SetShader("DefaultSpriteShader");

	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ drawSize.x,drawSize.y,1 });
	GE::Math::Vector2 pos = drawPosition;

	modelMatrix *= GE::Math::Matrix4x4::Translate({ pos.x,pos.y,0 });

	GE::Material material;
	material.color = drawColor;

	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,1,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	pGraphicsDevice->DrawMesh("2DPlane");
}

void MiniMap::Initialize(GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* graphicsDevice)
{
	pGameObjectManager = gameObjectManager;
	pGraphicsDevice = graphicsDevice;
}

void MiniMap::Update(float deltaTime)
{
}

void MiniMap::Draw()
{
	if (pGameObjectManager == nullptr)return;
	if (pGraphicsDevice == nullptr)return;

	const float DRAW_PLAYER_SIZE = 32;
	const GE::Color DRAW_PLAYER_COLOR = GE::Color::White();
	const float DRAW_ENEMY_SIZE = 16;
	const GE::Color DRAW_ENEMY_COLOR = GE::Color::Red();

	const GE::Math::Vector2 MINIMAP_OFFSET = { 1746,842 };
	const float MINIMAP_SIZE = 320;
	const float DIVIDE = 100;

	Draw2D(MINIMAP_OFFSET, MINIMAP_SIZE, GE::Color::Black());

	auto gameObjectsManager = pGameObjectManager->GetManager();
	GE::Math::Vector3 drawPlayerPositionOffset = (*gameObjectsManager)["player"][0]->GetTransform()->position;

	Draw2D(MINIMAP_OFFSET, DRAW_PLAYER_SIZE, DRAW_PLAYER_COLOR);

	for (auto& enemy : (*gameObjectsManager)["enemy"])
	{
		GE::Math::Vector3 enemyPosition = enemy->GetTransform()->position - drawPlayerPositionOffset;
		GE::Math::Vector2 drawEnemyPosition = GE::Math::Vector2(enemyPosition.x, -enemyPosition.z) / DIVIDE + MINIMAP_OFFSET;

		GE::Math::Vector3 minPos = { MINIMAP_OFFSET.x + -MINIMAP_SIZE / 2 + DRAW_ENEMY_SIZE / 2,MINIMAP_OFFSET.y + -MINIMAP_SIZE / 2 + DRAW_ENEMY_SIZE / 2,0 };
		GE::Math::Vector3 maxPos = { MINIMAP_OFFSET.x +  MINIMAP_SIZE / 2 - DRAW_ENEMY_SIZE / 2,MINIMAP_OFFSET.y +  MINIMAP_SIZE / 2 - DRAW_ENEMY_SIZE / 2,0 };

		GE::Math::Vector3 fixedDrawPosition = GE::Math::Vector3::Min(minPos, GE::Math::Vector3::Max(maxPos, { drawEnemyPosition.x,drawEnemyPosition.y,0 }));

		Draw2D({ fixedDrawPosition.x,fixedDrawPosition.y }, DRAW_ENEMY_SIZE, DRAW_ENEMY_COLOR);
	}
}
