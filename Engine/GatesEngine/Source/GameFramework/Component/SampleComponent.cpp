#include "..\..\..\Header\GameFramework\Component\SampleComponent.h"
#include "..\..\..\Header\Graphics\CBufferStruct.h"
#include "..\..\..\Header\Util\Utility.h"
#include "..\..\..\Header\Util\Random.h"
#include "..\..\..\Header\Graphics\Window.h"
#include "..\..\..\Header\GUI\GUIManager.h"
#include "..\..\..\Header\GameFramework\GameObject\GameObjectManager.h"

GE::SampleComponent::SampleComponent()
	: inputDevice(nullptr)
{
}

GE::SampleComponent::~SampleComponent()
{
}

void GE::SampleComponent::Awake()
{
	Utility::Printf("SampleComponent Awake()\n");
}

void GE::SampleComponent::Start()
{
	Utility::Printf("SampleComponent Start()\n");
	inputDevice = InputDevice::GetInstance();
	random = { RandomMaker::GetFloat(-1,1),RandomMaker::GetFloat(-1,1),RandomMaker::GetFloat(-1,1) };
	speed = 1;
	animator = SkinMeshManager::GetInstance()->Get("Bird");
	animator.Initialize();
}

void GE::SampleComponent::Update(float deltaTime)
{
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D1))
	{
		animator.PlayAnimation(0);
	}
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D2))
	{
		animator.PlayAnimation(1);
	}
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D3))
	{
		animator.PlayAnimation(2);
	}
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D4))
	{
		animator.PlayAnimation(3);
	}
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D5))
	{
		animator.PlayAnimation(4);
	}
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::D6))
	{
		animator.PlayAnimation(5);
	}

	animator.Update(deltaTime);
}

void GE::SampleComponent::Draw()
{
	const float DRAW_SIZE = 100;
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSkinMeshShader");

	transform->scale = DRAW_SIZE;
	Math::Matrix4x4 modelMatrix = transform->GetMatrix();
	Material material;
	material.color = Color::White();

	animator.SetAnimationData(graphicsDevice, modelMatrix);

	//renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(Material)) });
	graphicsDevice->DrawMesh("Player");
}

void GE::SampleComponent::LateDraw()
{
	const float SPRITE_SIZE = 100/3;

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultSpriteShader");

	GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ SPRITE_SIZE });
	GE::Math::Vector2 mousePos = inputDevice->GetMouse()->GetClientMousePos();
	//GE::Utility::Printf("%d,%d\n",(int)mousePos.x, (int)mousePos.y);

	modelMatrix *= GE::Math::Matrix4x4::Translate({ mousePos.x,mousePos.y,0 });
	Material material;
	material.color = Color::White();

	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,1,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(Material)) });
	graphicsDevice->DrawMesh("2DPlane");
}

void GE::SampleComponent::OnCollision(GameObject* other)
{
	Utility::Printf("SampleComponent OnCollision(GameObject* other) : hit\n");
}

void GE::SampleComponent::OnCollision(ICollider* hitCollider)
{
	Utility::Printf("SampleComponent OnCollision(ICollider* other) : hit\n");
}

void GE::SampleComponent::OnGui()
{
	float dragSpeed = 0.1f;
	float maxValue = 100;
	ImGui::DragFloat("Speed", &speed, dragSpeed, 0, maxValue);
	ImGui::DragFloat3("RandomVector", random.value, dragSpeed, -1, 1);
	ImGui::DragFloat3("GyroVector", gyro.value, dragSpeed, -1, 1);
}
