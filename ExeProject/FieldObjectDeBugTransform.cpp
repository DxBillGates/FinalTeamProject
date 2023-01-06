#include "FieldObjectDeBugTransform.h"
#include "PlayerComponent.h"
#include <GatesEngine/Header/Graphics\Window.h>
#include<cassert>
FieldObjectDeBugTransform* FieldObjectDeBugTransform::GetInstance()
{
	static FieldObjectDeBugTransform instance;
	return &instance;
}

void FieldObjectDeBugTransform::Update()
{
	if (PlayerComponent::statas != PlayerComponent::PlayerStatas::DEBUG)return;
	//マウスのレイ
	const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	GE::Math::GetScreenToRay(inputDevice->GetMouse()->GetClientMousePos(), &rayPos, &rayDir, cameraInfo.viewMatrix, cameraInfo.projMatrix, GE::Math::Matrix4x4::GetViewportMatrix(GE::Window::GetWindowSize()));
	//モード切替
	if (inputDevice->GetKeyboard()->CheckHitKey(GE::Keys::LCONTROL))
	{
		state = Trans_State::ROTATION;
	}
	else
	{
		state = Trans_State::POSITION;
	}

	//押されているかどうか
	bool LClick = false;
	if (inputDevice->GetMouse()->GetCheckHitButton(GE::MouseButtons::LEFT_CLICK))
	{
		LClick = true;
	}
	else
	{
		for (auto& obj : objects)
		{
			obj.statas = Statas::NONE;
		}
	}

	for (auto& obj : objects)
	{
		obj.pivotTransform[0].position = obj.target->GetTransform()->position + GE::Math::Vector3(obj.target->GetTransform()->scale.x * 3, 0, 0);
		obj.pivotTransform[1].position = obj.target->GetTransform()->position + GE::Math::Vector3(0, obj.target->GetTransform()->scale.x * 10, 0);
		obj.pivotTransform[2].position = obj.target->GetTransform()->position + GE::Math::Vector3(0, 0, obj.target->GetTransform()->scale.x * 3);
		for (int i = 0; i < obj.coll.size(); i++)
		{
			obj.coll[i].SetTransform(&obj.pivotTransform[i]);
			if (!LClick)
			{
				if (GE::CollisionManager::CheckSphereToRay(&obj.coll[i], obj.pivotTransform[i].position, rayPos, rayDir))
				{
					switch (i)
					{
					case 0:
						obj.statas = Statas::DRAG_X;
						obj.pivotTransform[i].scale = { 200 };
						break;

					case 1:
						obj.statas = Statas::DRAG_Y;
						obj.pivotTransform[i].scale = { 200 };
						break;

					case 2:
						obj.statas = Statas::DRAG_Z;
						obj.pivotTransform[i].scale = { 200 };
						break;
					}
				}
				else
				{
					obj.pivotTransform[i].scale = { 100 };
				}
			}
			else
			{
				GE::Math::Vector3 cameraPos = { cameraInfo.cameraPos.x,cameraInfo.cameraPos.y,cameraInfo.cameraPos.z };
				GE::Math::Vector3 pivotDirection = GE::Math::Vector3(obj.target->GetTransform()->position - cameraPos).Normalize();
				
				switch (state)
				{
				case Trans_State::POSITION:
					switch (obj.statas)
					{
					case  Statas::DRAG_X:
						obj.target->GetTransform()->position.x += -inputDevice->GetMouse()->GetMouseMove().x * 3;

						break;

					case  Statas::DRAG_Y:
						obj.target->GetTransform()->position.y += inputDevice->GetMouse()->GetMouseMove().y * 3;
						break;

					case  Statas::DRAG_Z:
						obj.target->GetTransform()->position.z += -inputDevice->GetMouse()->GetMouseMove().x * 3;
						break;
					case Statas::NONE:
						break;
					}
					break;
				case Trans_State::ROTATION:
					switch (obj.statas)
					{
					case  Statas::DRAG_X:
						obj.target->GetTransform()->rotation *= GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), inputDevice->GetMouse()->GetMouseMove().x * 0.01f);

						break;

					case  Statas::DRAG_Y:
						obj.target->GetTransform()->rotation *= GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), inputDevice->GetMouse()->GetMouseMove().x * 0.01f);
						break;

					case  Statas::DRAG_Z:
						obj.target->GetTransform()->rotation *= GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), inputDevice->GetMouse()->GetMouseMove().x * 0.01f);
						break;
					case Statas::NONE:
						break;
					}
					break;
				}
			}
		}
	}
}

void FieldObjectDeBugTransform::SetInputDevice(GE::InputDevice* iDevice)
{
	inputDevice = iDevice;
}

void FieldObjectDeBugTransform::SetGraphicsDevice(GE::IGraphicsDeviceDx12* gDevice)
{
	graphicsDevice = gDevice;
}

void FieldObjectDeBugTransform::AddTarget(GE::GameObject* gameobject)
{
	Object result;
	result.target = gameobject;
	for (int j = 0; j < 3; j++)
	{
		result.pivotTransform[j].scale = { 100 };
		result.coll.push_back({});
		result.coll[j].SetGameObject(result.target);
		result.coll[j].SetGraphicsDevice(graphicsDevice);
		result.coll[j].Awake();
		result.coll[j].SetSize(2);
	}
	objects.push_back(result);
}


void FieldObjectDeBugTransform::Draw()
{
	if (PlayerComponent::statas != PlayerComponent::PlayerStatas::DEBUG)return;

	material[0].color = GE::Color::Red();
	material[0].specular = 0.f;
	material[1].color = GE::Color::Blue();
	material[1].specular = 0.f;
	material[2].color = GE::Color::Green();
	material[2].specular = 0.f;

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	for (auto& obj : objects)
	{

		graphicsDevice->SetShader("DefaultMeshShader");
		for (int i = 0; i < 3; i++)
		{
			GE::Math::Matrix4x4 modelMatrix = obj.pivotTransform[i].GetMatrix();

			renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
			renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material[i],sizeof(GE::Material)) });
			switch (state)
			{
			case Trans_State::POSITION:
				graphicsDevice->DrawMesh("Cube");
				break;
			case Trans_State::ROTATION:
				graphicsDevice->DrawMesh("Sphere");
				break;
			}
		}
	}
}

void FieldObjectDeBugTransform::UnLoad()
{
	objects.clear();
}
