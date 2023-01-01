#include "..\..\..\Header\GameFramework\Component\DirectionalLight.h"
#include "..\..\..\Header\GUI\GUIManager.h"

GE::Math::Vector3 GE::DirectionalLight::CalclateDirection()
{
	Math::Vector3 vector = { 0,0,1 };
	Math::Quaternion vectorQuaternion = Math::Quaternion(vector.x, vector.y, vector.z, 0);
	Math::Quaternion temp = transform->rotation * vectorQuaternion * Math::Quaternion::Conjugate(transform->rotation);
	vector = { temp.x,temp.y,temp.z };
	return vector;
}

GE::Math::Vector3 GE::DirectionalLight::CalclateUp()
{
	Math::Vector3 vector = { 0,1,0 };
	Math::Quaternion vectorQuaternion = Math::Quaternion(vector.x, vector.y, vector.z, 0);
	Math::Quaternion temp = transform->rotation * vectorQuaternion * Math::Quaternion::Conjugate(transform->rotation);
	vector = { temp.x,temp.y,temp.z };
	return vector;
}

GE::DirectionalLight::DirectionalLight()
	: drawRange(0)
	, direction(0)
	, up(0)
	, angleXY(0)
	, lightMatrix(GE::Math::Matrix4x4::Identity())
	, projectionMatrix(GE::Math::Matrix4x4::Identity())
	, vpMatrix(GE::Math::Matrix4x4::Identity())
	, target(nullptr)
{
}

GE::DirectionalLight::~DirectionalLight()
{
}

void GE::DirectionalLight::Start()
{
	drawRange = { 20000,20000,30000 };
	direction = { 0,0,1 };
	up = { 0,-1,0 };
	angleXY = { 20,110 };
	transform->rotation = Math::Quaternion::Euler({ angleXY.x,angleXY.y,0 });

	lightMatrix = Math::Matrix4x4::GetViewMatrixLookTo(-direction * drawRange.z, direction, up);
	projectionMatrix = Math::Matrix4x4::GetOrthographMatrix({ drawRange.x,drawRange.y }, 1, drawRange.z);

	vpMatrix = lightMatrix * projectionMatrix;
}

void GE::DirectionalLight::Update(float deltaTime)
{
	const float MAX_RADIUS = 20000;
	direction = CalclateDirection();
	up = CalclateUp();

	Math::Vector3 targetPosition = target->position;
	targetPosition.y = 0;
	Math::Vector3 position = (target) ? targetPosition + -direction * drawRange.z : -direction * drawRange.z;
	//Math::Vector3 tempPosition = -direction * drawRange;
	//position = (tempPosition.Length() > MAX_RADIUS || tempPosition.Length() < MAX_RADIUS) ? position.Normalize() * MAX_RADIUS : position;
	transform->position = position;


	lightMatrix = Math::Matrix4x4::GetViewMatrixLookTo(position, direction, up);
	projectionMatrix = Math::Matrix4x4::GetOrthographMatrix({ drawRange.x,drawRange.y }, 1, drawRange.z * 5);
}

void GE::DirectionalLight::OnGui()
{
	ImGui::DragFloat2("angle", angleXY.value);
	ImGui::DragFloat3("drawRange", drawRange.value);
	ImGui::Text("%.3f,%.3f,%.3f", direction.x, direction.y, direction.z);
	ImGui::Text("%.3f,%.3f,%.3f", up.x, up.y, up.z);
	transform->rotation = Math::Quaternion::Euler({ angleXY.x,angleXY.y,0 });

	Math::Vector2 fixUV;
	fixUV = GetUV();
	ImGui::Text("uv : x %.3f / y %.3f", fixUV.x, fixUV.y);
}

void GE::DirectionalLight::SetDirectionalLightInfo()
{
	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	DirectionalLightInfo directionalLightInfo;
	directionalLightInfo.worldLightDir = direction;

	float height = Math::Lerp(0, 1, transform->position.y / drawRange.z);
	float red = Math::Lerp(0, 1, Math::Easing::EaseOutExpo(height));
	Math::Vector3 rgb = { red + red,red * red,red * red * red };
	if (rgb.x > 1)rgb.x = 1;
	directionalLightInfo.worldLightColor = GE::Math::Vector4(rgb.x, rgb.y, rgb.z, 1);

	renderQueue->AddSetConstantBufferInfo({ 3,cbufferAllocater->BindAndAttachData(3, &directionalLightInfo, sizeof(GE::DirectionalLightInfo)) });
}

const GE::Math::Matrix4x4& GE::DirectionalLight::GetViewMatrix()
{
	return lightMatrix;
}

const GE::Math::Matrix4x4& GE::DirectionalLight::GetProjectionMatrix()
{
	return projectionMatrix;
}

const GE::Math::Matrix4x4 GE::DirectionalLight::GetVPMatrix()
{
	return lightMatrix * projectionMatrix;
}

GE::Math::Vector2 GE::DirectionalLight::GetUV()
{
	Math::Vector2 fixUV;
	fixUV.x = 1 - (atan2f(transform->position.x - target->position.x, transform->position.z - target->position.z) / Math::PI + 1) * 0.5f;
	fixUV.y = 1 - (transform->position.y / drawRange.z + 1) * 0.5f;
	return fixUV;
}

void GE::DirectionalLight::SetTarget(Transform* transform)
{
	target = transform;
}
