#pragma once
#include "GameObject.h"
#include "..\..\GUI\Hierarchy.h"
#include "..\..\GUI\Inspector.h"
#include <vector>
#include <map>

namespace GE
{
	class GameObjectManager
	{
	private:
		// key : タグ
		std::map<std::string, std::vector<GameObject*>> gameObjects;

		GUI::Hierarchy hierarchyGui;
		GUI::Inspector inspectorGui;
	public:
		GameObjectManager();
		~GameObjectManager();
		void Awake();
		void Start();
		void Update(float deltaTime);
		void Draw();
		void LateDraw();

		GameObject* AddGameObject(GameObject* newGameObject);
		GameObject* FindGameObject(const std::string& name);
		GameObject* FindGameObjectWithTag(const std::string& name, const std::string& tag);

		//// 全オブジェクトとレイキャスト
		//bool Raycast(const Math::Vector3& pos, const Math::Vector3& dir,float length = 1000000,float* hitLength = nullptr);

		// 指定したタグを持ったオブジェクトとレイキャスト
		bool Raycast(const Math::Vector3& pos, const Math::Vector3& dir,const std::string& tag,float length = 1000000,float* hitLenght = nullptr);
	};
}