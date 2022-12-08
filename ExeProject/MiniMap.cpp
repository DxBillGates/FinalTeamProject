#include "MiniMap.h"
#include <GatesEngine/Header/Graphics/Window.h>

void MiniMap::Draw2D(const Draw2DInfo& drawInfo)
{
	GE::ICBufferAllocater* cbufferAllocater = pGraphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = pGraphicsDevice->GetRenderQueue();

	pGraphicsDevice->SetShader("DefaultSpriteShader");

	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ drawInfo.size.x,drawInfo.size.y,1 });
	GE::Math::Vector2 pos = drawInfo.pos;

	modelMatrix *= GE::Math::Matrix4x4::Translate({ pos.x,pos.y,0 });

	GE::Material material;
	material.color = drawInfo.color;

	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,1,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	pGraphicsDevice->DrawMesh("2DPlane");
}

void MiniMap::Draw2D(const Draw2DInfo& drawInfo, const GE::Math::Vector2& mapOffset, const GE::Math::Vector3& minPos, const GE::Math::Vector3& maxPos)
{
	GE::ICBufferAllocater* cbufferAllocater = pGraphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = pGraphicsDevice->GetRenderQueue();

	pGraphicsDevice->SetShader("DefaultSpriteShader");

	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ drawInfo.size.x,drawInfo.size.y,1 });
	GE::Math::Vector2 pos = drawInfo.pos;
	
	modelMatrix *= GE::Math::Matrix4x4::RotationZ(-angle);
	modelMatrix *= GE::Math::Matrix4x4::Translate({ pos.x - mapOffset.x,pos.y - mapOffset.y,0 });
	modelMatrix *= GE::Math::Matrix4x4::RotationZ(angle);
	modelMatrix *= GE::Math::Matrix4x4::Translate({ mapOffset.x,mapOffset.y,0 });

	GE::Math::Vector3 fixedPos = GE::Math::Vector3::Min(minPos, GE::Math::Vector3::Max(maxPos, { modelMatrix.m[3][0],modelMatrix.m[3][1],0 }));
	modelMatrix.m[3][0] = fixedPos.x;
	modelMatrix.m[3][1] = fixedPos.y;

	GE::Material material;
	material.color = drawInfo.color;

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

void MiniMap::Update(float deltaTime, float direction)
{
	angle += direction;
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

	// map”wŒi•`‰æ
	Draw2DInfo drawInfo = { MINIMAP_OFFSET,MINIMAP_SIZE, GE::Color::Black() };
	Draw2D(drawInfo);

	auto gameObjectsManager = pGameObjectManager->GetManager();
	GE::Math::Vector3 drawPlayerPositionOffset = (*gameObjectsManager)["player"][0]->GetTransform()->position;

	// ƒvƒŒƒCƒ„[•`‰æ
	drawInfo = { MINIMAP_OFFSET, DRAW_PLAYER_SIZE, DRAW_PLAYER_COLOR };
	Draw2D(drawInfo);

	// “G•`‰æ
	for (auto& enemy : (*gameObjectsManager)["enemy"])
	{
		GE::Math::Vector3 enemyPosition = enemy->GetTransform()->position - drawPlayerPositionOffset;
		GE::Math::Vector2 drawEnemyPosition = GE::Math::Vector2(enemyPosition.x, -enemyPosition.z) / DIVIDE + MINIMAP_OFFSET;

		GE::Math::Vector3 minPos = { MINIMAP_OFFSET.x + -MINIMAP_SIZE / 2 + DRAW_ENEMY_SIZE / 2,MINIMAP_OFFSET.y + -MINIMAP_SIZE / 2 + DRAW_ENEMY_SIZE / 2,0 };
		GE::Math::Vector3 maxPos = { MINIMAP_OFFSET.x + MINIMAP_SIZE / 2 - DRAW_ENEMY_SIZE / 2,MINIMAP_OFFSET.y + MINIMAP_SIZE / 2 - DRAW_ENEMY_SIZE / 2,0 };

		GE::Math::Vector3 fixedDrawPosition = GE::Math::Vector3::Min(minPos, GE::Math::Vector3::Max(maxPos, { drawEnemyPosition.x,drawEnemyPosition.y,0 }));

		drawInfo = { { fixedDrawPosition.x,fixedDrawPosition.y }, DRAW_ENEMY_SIZE, DRAW_ENEMY_COLOR};
		Draw2D(drawInfo, MINIMAP_OFFSET,minPos,maxPos);
	}
}
