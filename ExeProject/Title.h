#pragma once
#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 

//タイトル用テクスチャクラス
class TitleTex :public GE::Component
{
private:
	GE::ITexture* tex;
public:
	GE::Math::Vector3 position = {};
	GE::Math::Vector3 scale = {};
	GE::Math::Quaternion rotation = {};
	std::string tag;
public:
	void Awake();
	void Start()override;
	void Update(float deltaTime) override;
	void LateDraw() override;

	void SetTexture(GE::ITexture* setTex) { tex = setTex; }
};

//タイトルクラス
class Title
{
private:
	GE::InputDevice* inputDevice;

	std::vector<TitleTex*> sprites;
	std::vector<GE::ITexture*> textures;

	bool decided = false;
public:
	//選択項目
	enum States
	{
		start,
		option,
		exit,

		serectNum,
	};
	//選択ステージ
	enum Stage
	{
		stage1,
		stage2,

		stageNum,
	};

	States states;
	Stage stage;

	float alpha = 0.0f;
private:
	//テクスチャ生成
	void Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device);
public:
	static Title* GetInstance();
	Title() = default;
	~Title() = default;
	void operator=(const Title& obj) = delete;
	Title(const Title& obj) = delete;

	//選択
	void Select();

	//初期設定
	void Awake(GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device);
	void Update();

	bool GetDecid() { return decided; }

	/// <summary>
	/// 選択結果
	/// </summary>
	/// <param name="s">指定内容</param>
	/// <returns>指定内容がタイトルで選択されたか</returns>
	bool GetSelect(Title::States s);

	void ClearGameObject();
};

