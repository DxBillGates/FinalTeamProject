#include "VolumetricCloud.h"
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h>

void VolumetricCloud::Start()
{
}

void VolumetricCloud::Update(float deltaTime)
{
}

void VolumetricCloud::Draw()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("volumetricCloudShader");
	GE::Math::Matrix4x4 modelMatrix = transform->GetMatrix();

	GE::Material material;
	material.color = GE::Color::White();

	GE::CameraInfo cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });

	graphicsDevice->DrawMesh("Cube");
}

void VolumetricCloud::LateDraw()
{
}
