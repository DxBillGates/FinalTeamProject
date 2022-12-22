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
		std::vector<GameObject*> currentFrameHitObjects;
		std::vector<GameObject*> beforeFrameHitObjects;
		Transform transform;
		bool enabled;
		bool drawAxisEnabled;
		bool isDestroy;

		std::string name;
		std::string tag;

		static IGraphicsDeviceDx12* graphicsDevice;

		ICollider* firstCollider;

		Color materialColor;

		Math::Vector3 hitNormal;
	public:
		GameObject(const std::string& name = "unknown", const std::string& tag = "none");
		~GameObject();
		void Awake();
		void Start();
		void Update(float deltaTime);
		void DrawShadow();
		void Draw();
		void LateDraw();
		void OnCollision(GameObject* other,bool enter = false,bool stay = false,bool exit = false);
		void OnCollision(ICollider* hitCollider);
		void OnDestroy();
		void Destroy();

		Transform* GetTransform();
		bool IsEnabled();
		const std::string& GetName();
		const std::string& GetTag();
		bool IsDestroy();

		// 一番最初に追加されたコライダーを返す
		ICollider* GetCollider();
		Color GetColor();
		Math::Vector3 GetHitNormal();

		void SetGameObjectManager(GameObjectManager* manager);
		void SetParent(GameObject* object);
		void SetName(const std::string& name);
		void SetTag(const std::string & tag);
		void SetEnabled(bool flag);
		void SetDrawAxisEnabled(bool flag);
		static void SetGraphicsDevice(IGraphicsDeviceDx12* gDevice);
		static bool IsSetGraphicsDevice();
		void SetColor(const Color& color);
		void SetHitNormal(const Math::Vector3& normal);

		template<typename T>
		T* AddComponent();

		template<typename T>
		T* GetComponent();

		GameObjectManager* GetGameObjectManager();

		// 今のフレームでそいつとヒットしていたか
		bool CheckBeforeFrameHitObject(GameObject* hitObject);

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