#include "FieldObjectDeBugTransform.h"
#include "PlayerComponent.h"
#include <GatesEngine/Header/Graphics\Window.h>
#include <GatesEngine/Header/GUI/GUIManager.h>
#include<cassert>
FieldObjectDebugTransform* FieldObjectDebugTransform::GetInstance()
{
	static FieldObjectDebugTransform instance;
	return &instance;
}

void FieldObjectDebugTransform::Update()
{
#ifdef _DEBUG
	GE::Math::Vector2 windowPos,windowSize;
	ImGui::Begin("Game");
	auto pos = ImGui::GetWindowPos();
	auto size = ImGui::GetWindowSize();
	windowPos = { pos.x,pos.y };
	windowSize = { size.x,size.y };
	ImGui::End();
	if (PlayerComponent::statas != PlayerComponent::PlayerStatas::DEBUG)return;
	//マウスのレイ
	const auto& cameraInfo = graphicsDevice->GetMainCamera()->GetCameraInfo();
	GE::Math::GetScreenToRay(inputDevice->GetMouse()->GetClientMousePos(), &rayPos, &rayDir, cameraInfo.viewMatrix, cameraInfo.projMatrix, GE::Math::Matrix4x4::GetViewportMatrix(windowSize,windowPos));
	//モード切替
	if (inputDevice->GetKeyboard()->CheckPressTrigger(GE::Keys::LCONTROL))
	{
		switch (state)
		{
		case Trans_State::POSITION:
			state = Trans_State::ROTATION;
			break;
		case Trans_State::ROTATION:
			state = Trans_State::SCALE;
			break;
		case Trans_State::SCALE:
			state = Trans_State::POSITION;
			break;
		}
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
		obj.pivotTransform[0].position = obj.target->GetTransform()->position + GE::Math::Vector3(obj.target->GetTransform()->scale.x *obj.pivotLength.x, 0, 0);
		obj.pivotTransform[1].position = obj.target->GetTransform()->position + GE::Math::Vector3(0, obj.target->GetTransform()->scale.x * obj.pivotLength.y, 0);
		obj.pivotTransform[2].position = obj.target->GetTransform()->position + GE::Math::Vector3(0, 0, obj.target->GetTransform()->scale.x* obj.pivotLength.z);

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
						obj.target->GetTransform()->rotation *= GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), inputDevice->GetMouse()->GetMouseMove().x * 0.01f);

						break;

					case  Statas::DRAG_Y:
						obj.target->GetTransform()->rotation *= GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), inputDevice->GetMouse()->GetMouseMove().x * 0.01f);
						break;

					case  Statas::DRAG_Z:
						obj.target->GetTransform()->rotation *= GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), inputDevice->GetMouse()->GetMouseMove().x * 0.01f);
						break;
					case Statas::NONE:
						break;
					}
					break;
				case Trans_State::SCALE:
					switch (obj.statas)
					{
					case  Statas::DRAG_X:
						obj.target->GetTransform()->scale.x += -inputDevice->GetMouse()->GetMouseMove().x;
						break;
					case  Statas::DRAG_Y:
						obj.target->GetTransform()->scale.y += inputDevice->GetMouse()->GetMouseMove().y;
						break;
					case  Statas::DRAG_Z:
						obj.target->GetTransform()->scale.z += -inputDevice->GetMouse()->GetMouseMove().x;
						break;
					case Statas::NONE:
						break;
					}
					break;
				}
			}
		}
	}
#endif

}

void FieldObjectDebugTransform::SetInputDevice(GE::InputDevice* iDevice)
{
	inputDevice = iDevice;
}

void FieldObjectDebugTransform::SetGraphicsDevice(GE::IGraphicsDeviceDx12* gDevice)
{
	graphicsDevice = gDevice;
}

void FieldObjectDebugTransform::AddTarget(GE::GameObject* gameobject, GE::Math::Vector3 pivotLength)
{
#ifdef _DEBUG

	Object result;
	result.target = gameobject;
	for (int j = 0; j < 3; j++)
	{
		result.pivotTransform[j].scale = { 100 };
		result.pivotTransform[0].rotation = GE::Math::Quaternion(GE::Math::Vector3(0, 0, 1), -1.57f);
		result.pivotTransform[1].rotation = GE::Math::Quaternion();
		result.pivotTransform[2].rotation = GE::Math::Quaternion(GE::Math::Vector3(1, 0, 0), 1.57f);

		result.coll.push_back({});
		result.coll[j].SetGameObject(result.target);
		result.coll[j].SetGraphicsDevice(graphicsDevice);
		result.coll[j].Awake();
		result.coll[j].SetSize(2);
	}
	result.pivotLength = pivotLength;
	objects.push_back(result);
#endif
}


void FieldObjectDebugTransform::Draw()
{
#ifdef _DEBUG

	if (PlayerComponent::statas != PlayerComponent::PlayerStatas::DEBUG)return;

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	material[0].color = GE::Color::Red();
	material[0].specular = 0.f;
	material[1].color = GE::Color::Blue();
	material[1].specular = 0.f;
	material[2].color = GE::Color::Green();
	material[2].specular = 0.f;

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
				graphicsDevice->DrawMesh("Corn");
				break;
			case Trans_State::ROTATION:
				graphicsDevice->DrawMesh("Sphere");
				break;
			case Trans_State::SCALE:
				graphicsDevice->DrawMesh("Cube");
				break;
			}
		}
	}
#endif
}

void FieldObjectDebugTransform::UnLoad()
{
#ifdef _DEBUG

	objects.clear();
#endif
}
