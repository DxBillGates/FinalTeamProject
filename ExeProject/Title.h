#pragma once
#include"Option.h"

#include <GatesEngine/Header/GameFramework/Component/Component.h>
#include <GatesEngine/Header/Input/InputDevice.h>
#include <GatesEngine/Header/GameFramework/GameObject/GameObjectManager.h> 
#include <GatesEngine/Header/GameFramework/GameObject/GameObject.h> 
#include<GatesEngine/Header/Audio/AudioManager.h>

//タイトルクラス
class Title
{
private:
	GE::InputDevice* inputDevice;
	
	bool decided = false;
	Option option;

public:
	//選択項目
	enum class States
	{
		start,
		endless,
		option,
		exit,

		serectNum,
	};
	//選択ステージ
	enum class Stage
	{
		stage1,
		stage2,

		stageNum,
	};

	States states = States::start;
	Stage stage;
	bool isActive;
	float alpha = 0.0f;
	bool isExit;
private:
	Title() = default;
	~Title() = default;
	void operator=(const Title & obj) = delete;
	Title(const Title & obj) = delete;

	//テクスチャ生成
	void Create(std::string gui_tag, std::string tex_tag, GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device);
public:
	static Title* GetInstance();

	//選択
	void Select();

	//初期設定
	void Awake(GE::GameObjectManager* gameObjectManager, GE::IGraphicsDeviceDx12* device);
	void Update(GE::AudioManager* audioManager);

	bool GetDecid() { return decided; }

	/// <summary>
	/// 選択結果
	/// </summary>
	/// <param name="s">指定内容</param>
	/// <returns>指定内容がタイトルで選択されたか</returns>
	bool GetSelect(Title::States s);

	void Back() { decided = false; }

	void ClearGameObject();
};

