#pragma once
#include "GameObject.h"
#include "..\..\GUI\Hierarchy.h"
#include "..\..\GUI\Inspector.h"
#include <vector>
#include <unordered_map>

namespace GE
{
	class GameObjectManager
	{
	private:
		// key : タグ
		std::unordered_map<std::string, std::vector<GameObject*>> managedTagGameObjects;
		std::vector<GameObject*> gameObjects;

		GUI::Hierarchy hierarchyGui;
		GUI::Inspector inspectorGui;

		int destroyGameObjectCount;
	private:
		void Destroy();
	public:
		GameObjectManager();
		~GameObjectManager();
		void Awake();
		void Start();
		void Update(float deltaTime);
		void DrawShadow();
		void Draw();
		void LateDraw();

		std::unordered_map<std::string, std::vector<GameObject*>>* GetManager();
		GameObject* AddGameObject(GameObject* newGameObject);
		GameObject* FindGameObject(const std::string& name);
		GameObject* FindGameObjectWithTag(const std::string& name, const std::string& tag);

		void DestroyGameObject(const std::string& name);
		void DestroyGameObject(GameObject* gameObject);

		// 全ゲームオブジェクトを削除する
		void DestroyGameObjects();

		//// 全オブジェクトとレイキャスト
		//bool Raycast(const Math::Vector3& pos, const Math::Vector3& dir,float length = 1000000,float* hitLength = nullptr);

		// 指定したタグを持ったオブジェクトとレイキャスト
		bool Raycast(const Math::Vector3& pos, const Math::Vector3& dir, const std::string& tag, float length = 1000000, float* hitLenght = nullptr);
	};
}