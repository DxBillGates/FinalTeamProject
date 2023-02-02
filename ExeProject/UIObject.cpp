#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics\Window.h>
#include <GatesEngine/Header/GUI\GUIManager.h>

#include"UIObject.h"
#include"FieldObjectManager.h"
#include"PlayerComponent.h"

UIObject* UIObject::GetInstance()
{
	static UIObject instance;
	return &instance;
}

UIObject::Object UIObject::AddObject(GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color, std::string textureName)
{
	Object result;
	result.transform.position = positon;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	return result;
}

UIObject::Object UIObject::AddAnimaiotnObject(GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color, std::string textureName,
	GE::Math::Vector2 texSize, GE::Math::Vector2 clipSize, GE::Math::Vector2 pivotPos)
{
	Object result;
	result.transform.position = positon;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	result.texSize = texSize;
	result.clipSize = clipSize;
	result.pivotPos = pivotPos;
	return result;
}


void UIObject::SampleSceneStart()
{
	GE::Utility::Printf("TestTreeComponent Start()\n");
	object["chick"] =
		AddObject(FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 1000, 0), { 500 }, GE::Color(1, 1, 1, 1), "texture_Chick");
	object["colect"] =
		AddAnimaiotnObject(FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 600, 0), { 250 }, GE::Color(1, 1, 1, 1), "texture_Number", { 320,64 }, { 32,64 });
	object["symbol"] = AddAnimaiotnObject({}, { 250 }, GE::Color(1, 1, 1, 1), "texture_symbol", { 64,64 }, { 32,64 }, { 1,0 });
	object["colectMax"] = AddAnimaiotnObject({}, { 250 }, GE::Color(1, 1, 1, 1), "texture_Number", { 320,64 }, { 32,64 }, { 1,0 });
	object["control_info"] = AddObject({}, { 512,1,384 }, GE::Color(1, 1, 1, 1), "control_info_1_tex");

	object["control_info"].isDraw = true;
	object["colect"].isDraw = false;
	object["symbol"].isDraw = false;
	object["colectMax"].isDraw = false;
	object["chick"].isDraw = false;

}

void UIObject::DashModeStart()
{
	object["control_info_2"] = AddObject({}, GE::Math::Vector3(512, 1, 384) * 2, GE::Color(1, 1, 1, 1), "control_info_2_tex");

	object["control_info_2"].isDraw = true;

}

void UIObject::SampleSceneUpdate(float deltaTime)
{
	//常にカメラに正面を向ける
	for (auto& o : object)
	{
		GE::Math::Vector3 dir = cameraPosition - o.second.transform.position;
		dir.y = 0.0f;
		o.second.transform.rotation = GE::Math::Quaternion::LookDirection(dir.Normalize()) * GE::Math::Quaternion::Euler({ 90, 0, 180 });
	}

	//ごり押し
	object["colect"].pivotPos = { (float)StartTree::collectCount,0 };
	object["symbol"].transform.position = object["colect"].transform.position + object["colect"].transform.GetRight() * object["colect"].transform.scale.x;
	object["colectMax"].pivotPos = { (float)StartTree::goalCollect,0 };
	object["colectMax"].transform.position = object["symbol"].transform.position + object["symbol"].transform.GetRight() * object["symbol"].transform.scale.x;
	//説明UI
	GE::Math::Vector3 infoPos = object["colectMax"].transform.GetRight() * GE::Math::Matrix4x4::RotationY(1.2f) * 800;
	object["control_info"].transform.position = object["colectMax"].transform.position + infoPos + GE::Math::Vector3(0, -310, 0);
	object["control_info"].isDraw = false;

	switch (PlayerComponent::statas)
	{
	case PlayerComponent::PlayerStatas::TITLE:
		break;
	case PlayerComponent::PlayerStatas::TITLE_MENU:
		break;
	case PlayerComponent::PlayerStatas::STAY_TREE:
		object["control_info"].isDraw = true;	//説明UI
		object["colect"].isDraw = true;
		object["symbol"].isDraw = true;
		object["colectMax"].isDraw = true;
		object["chick"].isDraw = true;

		break;
	case PlayerComponent::PlayerStatas::MOVE:
		break;
	case PlayerComponent::PlayerStatas::CRASH:
		break;
	}

}

void UIObject::DashModeUpdate(float deltaTime)
{
	//常にカメラに正面を向ける
	for (auto& o : object)
	{
		GE::Math::Vector3 dir = cameraPosition - o.second.transform.position;
		dir.y = 0.0f;
		o.second.transform.rotation = GE::Math::Quaternion::LookDirection(dir.Normalize()) * GE::Math::Quaternion::Euler({ 90, 0, 180 });
	}

	object["control_info_2"].transform.position = FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 700, 0);
	object["control_info_2"].isDraw = false;

	switch (PlayerComponent::statas)
	{
	case PlayerComponent::PlayerStatas::TITLE:
		break;
	case PlayerComponent::PlayerStatas::TITLE_MENU:
		break;
	case PlayerComponent::PlayerStatas::STAY_TREE:
		object["control_info_2"].isDraw = true;	//説明UI
		break;
	}
}

void UIObject::Draw(GE::IGraphicsDeviceDx12* graphicsDevice)
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithTextureShader");


	for (auto& o : object)
	{
		if (o.second.isDraw)
		{
			GE::Math::Matrix4x4 modelMatrix = o.second.transform.GetMatrix();
			GE::Material material;
			material.color = o.second.color;
			renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
			renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
			renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get(o.second.textureName)->GetSRVNumber() });

			// アニメーションの情報
			GE::TextureAnimationInfo textureAnimationInfo;
			// 画像の元サイズ
			textureAnimationInfo.textureSize = o.second.texSize;
			// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
			// textureSizeと一緒にすると切り抜かれずに描画される
			textureAnimationInfo.clipSize = o.second.clipSize;
			// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
			textureAnimationInfo.pivot = o.second.pivotPos;
			renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });

			graphicsDevice->DrawMesh("Plane");
		}
	}
}

void UIObject::OnGui()
{
}

void UIObject::UnLoad()
{
	object.clear();
}
