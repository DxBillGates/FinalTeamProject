#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics\Window.h>
#include <GatesEngine/Header/GUI\GUIManager.h>

#include"UIObject.h"
#include"FieldObjectManager.h"

UIObject* UIObject::GetInstance()
{
	static UIObject instance;
	return &instance;
}

UIObject::Object UIObject::AddObject(std::string tag, GE::Math::Vector3 positon, GE::Math::Vector3 scale, GE::Color color, std::string textureName)
{
	Object result;
	result.tag = tag;
	result.transform.position = positon;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	return result;
}


void UIObject::Start()
{
	GE::Utility::Printf("TestTreeComponent Start()\n");
	normalUI.push_back(
		AddObject("test", FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 1000, 0), { 500 }, GE::Color(1, 1, 1, 1), "texture_Chick")
	);
	/*AnimationUI.push_back(
		AddObject("test", FieldObjectManager::GetInstance()->StartPosition + GE::Math::Vector3(0, 1000, 0), { 500 }, GE::Color(1, 1, 1, 1), "texture_Number")
	);*/

}

void UIObject::Update(float deltaTime)
{
	//常にカメラに正面を向ける
	for (int i = 0; i < normalUI.size(); i++)
	{
		GE::Math::Vector3 dir = cameraPosition - normalUI[i].transform.position;
		dir.y = 0.0f;
		normalUI[i].transform.rotation = GE::Math::Quaternion::LookDirection(dir.Normalize()) * GE::Math::Quaternion::Euler({ 90, 0, 180 });
	}

	//for (int i = 0; i < AnimationUI.size(); i++)
	//{
	//	GE::Math::Vector3 dir = cameraPosition - AnimationUI[i].transform.position;
	//	dir.y = 0.0f;
	//	AnimationUI[i].transform.rotation = LookDirection(dir.Normalize()) * GE::Math::Quaternion::Euler({ 90, 0, 180 });
	//}


	//	pivotPosX = StartTree::collectCount;
	//	texSizeX = 320;//画像サイズ
	//	clipSizeX = 32;
	//	
}

void UIObject::Draw(GE::IGraphicsDeviceDx12* graphicsDevice)
{

	GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
	GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

	graphicsDevice->SetShader("DefaultMeshWithTextureShader");


	for (auto o : normalUI)
	{
		GE::Math::Matrix4x4 modelMatrix = o.transform.GetMatrix();
		GE::Material material;
		material.color = o.color;
		renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get(o.textureName)->GetSRVNumber() });

		// アニメーションの情報
		GE::TextureAnimationInfo textureAnimationInfo;
		// 画像の元サイズ
		GE::Math::Vector2 textureSize = { 1,1 };
		textureAnimationInfo.textureSize = textureSize;
		// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
		// textureSizeと一緒にすると切り抜かれずに描画される
		textureAnimationInfo.clipSize = { 1,1 };
		// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
		textureAnimationInfo.pivot = { 0,0 };
		renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });

		graphicsDevice->DrawMesh("Plane");
	}
	//for (auto a : AnimationUI)
	{
		// アニメーションの情報
		//GE::TextureAnimationInfo textureAnimationInfo;

		//// 画像の元サイズ
		//textureAnimationInfo.textureSize = { texSizeX,64 };
		//// 元画像のサイズからどうやって切り抜くか　例) 元サイズが100*100で半分だけ表示したいなら{50,100}にする
		//// textureSizeと一緒にすると切り抜かれずに描画される
		//textureAnimationInfo.clipSize = { clipSizeX,64 };
		//// 切り抜く際の左上座標 例) {0,0}なら元画像の左上 texture->GetSize()なら右下になる
		//textureAnimationInfo.pivot = { pivotPosX,0 };

		//GE::Math::Matrix4x4 modelMatrix = AnimationUI[0].transform.GetMatrix();
		//GE::Material material;
		//material.color = AnimationUI[0].color;
		//renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
		//renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2,&material,sizeof(GE::Material)) });
		//renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
		//renderQueue->AddSetShaderResource({5,graphicsDevice->GetTextureManager()->Get(AnimationUI[0].textureName)->GetSRVNumber() });

		//graphicsDevice->DrawMesh("Plane");
	}
}

void UIObject::OnGui()
{
	//ImGui::InputFloat3("Rotation_Euler", rotation_euler.value);
}

void UIObject::UnLoad()
{
	normalUI.clear();
}