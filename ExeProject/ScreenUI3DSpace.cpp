#include "ScreenUI3DSpace.h"
#include <GatesEngine/Header/Graphics/ICBufferAllocater.h>
#include <GatesEngine/Header/Graphics/Window.h>
#include <GatesEngine/Header/Util/Math/Math.h>
#include <cmath>

GE::IGraphicsDeviceDx12* ScreenUI3DSpace::graphicsDevice = nullptr;

void ScreenUI3DSpace::FixedCalclatedScreenPos()
{
	const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	GE::Math::Vector3 cameraPos = { cameraInfo.cameraPos.x,cameraInfo.cameraPos.y,cameraInfo.cameraPos.z };
	GE::Math::Vector3 cameraDir = { cameraInfo.cameraDir.x,cameraInfo.cameraDir.y,cameraInfo.cameraDir.z };
	GE::Math::Vector3 targetDir = worldPos - cameraPos;
	GE::Math::Vector3 sub = cameraDir - targetDir.Normalize();

	GE::Math::Vector2 windowSize;
	windowSize = GE::Window::GetWindowSize();
	GE::Math::Vector3 windowSize3D = { windowSize.x,windowSize.y ,0 };
	GE::Math::Vector3 halfScale = scale / 2;

	// x、y座標ともにスクリーン中心基準で反転してしまうため修正する
	if (GE::Math::Vector3::Dot(cameraDir, targetDir.Normalize()) < 0)
	{
		// 画面真ん中の座標との差分の二倍分引く
		GE::Math::Vector3 temp = calclatedScreenPos - windowSize3D / 2;
		calclatedScreenPos -= temp * 2;
		if (cameraPos.y > worldPos.y)calclatedScreenPos.y = windowSize3D.y - halfScale.y;
		else calclatedScreenPos.y = halfScale.x;
	}

	// 画面外に表示されてしまう計算結果を修正
	calclatedScreenPos = GE::Math::Vector3::Min({ halfScale.x,halfScale.y,0 }, GE::Math::Vector3::Max(calclatedScreenPos, windowSize3D - halfScale));
}

void ScreenUI3DSpace::CalclateScreenPos()
{
	if (graphicsDevice == nullptr)return;

	GE::Math::Matrix4x4 viewPort, proj, view;
	GE::Math::Vector2 windowPos;
	GE::Math::Vector2 windowSize;
	windowPos = { 0, 0 };
	windowSize = GE::Window::GetWindowSize();

	const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();

	view = cameraInfo.viewMatrix;
	proj = cameraInfo.projMatrix;
	viewPort = GE::Math::Matrix4x4::GetViewportMatrix(windowSize, windowPos);
	calclatedScreenPos = GE::Math::Matrix4x4::Transform(worldPos, view * proj * viewPort);

	FixedCalclatedScreenPos();
}

void ScreenUI3DSpace::SetShader()
{
	graphicsDevice->SetShader("DefaultSpriteShader");
}

void ScreenUI3DSpace::SetShaderResource()
{
}

void ScreenUI3DSpace::AttachScreenPos()
{
	GE::ICBufferAllocater* cbufferAlloc = graphicsDevice->GetCBufferAllocater();
	graphicsDevice->SetCurrentRenderQueue(false);
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();
	GE::Math::Matrix4x4 modelMatrix;
	GE::Math::Vector2 diffWindowSize = GE::Window::GetDiffWindowSize();
	GE::Math::Vector3 temp = scale;
	temp.x *= diffWindowSize.x;
	temp.y *= diffWindowSize.y;
	modelMatrix = GE::Math::Matrix4x4::Scale(temp);
	temp = calclatedScreenPos;
	temp.x *= diffWindowSize.x;
	temp.y *= diffWindowSize.y;
	modelMatrix *= GE::Math::Matrix4x4::Translate(temp);
	renderQueue->AddSetConstantBufferInfo({ 0,cbufferAlloc->BindAndAttachData(0, &modelMatrix, sizeof(modelMatrix)) });
}

void ScreenUI3DSpace::AttachCamera()
{
	GE::ICBufferAllocater* cbufferAlloc = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();
	GE::CameraInfo cameraInfo;
	cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,1,0 }, { 0,0,1 }, { 0,1,0 });
	cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	renderQueue->AddSetConstantBufferInfo({ 1,cbufferAlloc->BindAndAttachData(1, &cameraInfo, sizeof(cameraInfo)) });
}

void ScreenUI3DSpace::AttachMaterial()
{
	GE::ICBufferAllocater* cbufferAlloc = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	GE::Material material;
	material.color = color;

	renderQueue->AddSetConstantBufferInfo({ 2,cbufferAlloc->BindAndAttachData(2, &material, sizeof(material)) });
}

void ScreenUI3DSpace::DrawPlane()
{
	graphicsDevice->DrawMesh("2DPlane");
}

ScreenUI3DSpace::ScreenUI3DSpace()
	: worldPos({ 0 })
	, scale({ 1 })
	, calclatedScreenPos({ 0 })
	, color(GE::Color::White())
{
}

ScreenUI3DSpace::~ScreenUI3DSpace()
{
}

void ScreenUI3DSpace::SetGraphicsDevice(GE::IGraphicsDeviceDx12* pGraphicsDevice)
{
	graphicsDevice = pGraphicsDevice;
}

void ScreenUI3DSpace::Start()
{
	worldPos = { 0 };
	scale = { 1 };
	calclatedScreenPos = { 0 };
	color = { 1 };
}

void ScreenUI3DSpace::Update(float deltaTime)
{
	CalclateScreenPos();
}

void ScreenUI3DSpace::Draw()
{
	if (graphicsDevice == nullptr)return;
	SetShader();
	SetShaderResource();
	AttachScreenPos();
	AttachCamera();
	AttachMaterial();
	DrawPlane();
}

void ScreenUI3DSpace::SetWorldPosition(const GE::Math::Vector3& setPos)
{
	worldPos = setPos;
}

void ScreenUI3DSpace::SetScale(const GE::Math::Vector3& setScale)
{
	scale = setScale;
}

void ScreenUI3DSpace::SetColor(const GE::Color& setColor)
{
	color = setColor;
}
