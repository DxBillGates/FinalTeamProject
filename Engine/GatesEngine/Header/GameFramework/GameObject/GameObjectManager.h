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
		// key : �^�O
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

		// �S�Q�[���I�u�W�F�N�g���폜����
		void DestroyGameObjects();

		//// �S�I�u�W�F�N�g�ƃ��C�L���X�g
		//bool Raycast(const Math::Vector3& pos, const Math::Vector3& dir,float length = 1000000,float* hitLength = nullptr);

		// �w�肵���^�O���������I�u�W�F�N�g�ƃ��C�L���X�g
		bool Raycast(const Math::Vector3& pos, const Math::Vector3& dir, const std::string& tag, float length = 1000000, float* hitLenght = nullptr);
	};
}