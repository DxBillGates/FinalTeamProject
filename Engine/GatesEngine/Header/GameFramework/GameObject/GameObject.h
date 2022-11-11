#pragma once
#include "..\..\Util\Math\Transform.h"
#include "..\..\Graphics\IGraphicsDeviceDx12.h"
#include "..\..\Graphics\Color.h"

#include <vector>
#include <string>

namespace GE
{
	class Component;
	class ICollider;
	class Collider;
	class GameObjectManager;
	class GameObject
	{
	private:
		GameObjectManager* gameObjectManager;

		GameObject* parent;
		std::vector<Component*> components;
		Transform transform;
		bool enabled;
		bool drawAxisEnabled;

		std::string name;
		std::string tag;

		static IGraphicsDeviceDx12* graphicsDevice;

		ICollider* firstCollider;

		Color materialColor;
	public:
		GameObject(const std::string& name = "unknown", const std::string& tag = "none");
		~GameObject();
		void Awake();
		void Start();
		void Update(float deltaTime);
		void Draw();
		void LateDraw();
		void OnCollision(GameObject* other);
		void OnCollision(ICollider* hitCollider);

		Transform* GetTransform();
		bool IsEnabled();
		const std::string& GetName();
		const std::string& GetTag();

		// 一番最初に追加されたコライダーを返す
		ICollider* GetCollider();
		Color GetColor();

		void SetGameObjectManager(GameObjectManager* manager);
		void SetParent(GameObject* object);
		void SetName(const std::string& name);
		void SetTag(const std::string & tag);
		void SetEnabled(bool flag);
		void SetDrawAxisEnabled(bool flag);
		static void SetGraphicsDevice(IGraphicsDeviceDx12* gDevice);
		static bool IsSetGraphicsDevice();
		void SetColor(const Color& color);

		template<typename T>
		T* AddComponent();

		template<typename T>
		T* GetComponent();

		GameObjectManager* GetGameObjectManager();
		std::vector<Component*>* GetComponents();
	};


	template<typename T>
	inline T* GameObject::AddComponent()
	{
		T* t = new T();
		//Componentクラスから派生したものかチェック
		Component* pComponent = static_cast<Component*>(t);
		if (pComponent != nullptr)
		{
			components.push_back(pComponent);
			return t;
		}
		return nullptr;
	}

	template<typename T>
	inline T* GameObject::GetComponent()
	{
		for (auto& component : components)
		{
			T* t = dynamic_cast<T*>(component);
			if (t == nullptr)continue;

			return t;
		}

		return nullptr;
	}
}