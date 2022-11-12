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
		// key : �^�O
		std::map<std::string, std::vector<GameObject*>> gameObjects;

		GUI::Hierarchy hierarchyGui;
		GUI::Inspector inspectorGui;

		std::map<std::string, std::vector<GameObject*>> destroyGameObjects;
	private:
		void Destroy();
	public:
		GameObjectManager();
		~GameObjectManager();
		void Awake();
		void Start();
		void Update(float deltaTime);
		void Draw();
		void LateDraw();

		std::map<std::string, std::vector<GameObject*>>* GetManager();
		GameObject* AddGameObject(GameObject* newGameObject);
		GameObject* FindGameObject(const std::string& name);
		GameObject* FindGameObjectWithTag(const std::string& name, const std::string& tag);

		void DestroyGameObject(const std::string& name);
		void DestroyGameObject(GameObject* gameObject);

		//// �S�I�u�W�F�N�g�ƃ��C�L���X�g
		//bool Raycast(const Math::Vector3& pos, const Math::Vector3& dir,float length = 1000000,float* hitLength = nullptr);

		// �w�肵���^�O���������I�u�W�F�N�g�ƃ��C�L���X�g
		bool Raycast(const Math::Vector3& pos, const Math::Vector3& dir, const std::string& tag, float length = 1000000, float* hitLenght = nullptr);

	};
}