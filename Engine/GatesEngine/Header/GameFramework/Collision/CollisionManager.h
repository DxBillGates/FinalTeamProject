#pragma once
#include "ICollider.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace GE
{
	class CollisionManager
	{
	private:
		std::unordered_map<std::string, std::vector<std::string>> collisionTagCombination;
		std::unordered_map<std::string, std::vector<GameObject*>>* pGameObjects;
	private:
		void CollisionCheck(std::vector<GameObject*>* firstTagGameObjects, std::vector<GameObject*>* secondTagGameObjects);
		static Math::Vector3 CheckClosestPoint(const Math::Vector3& point, const Triangle& triangle);
	public:
		void AddTagCombination(const std::string& tag1, const std::string& tag2);
		void SetGameObjectManager(std::unordered_map<std::string, std::vector<GameObject*>>* pManager);

		void Update();

		static bool CheckHit(ICollider* col1, ICollider* col2);
		static bool CheckSphere(ICollider* col1, ICollider* col2);
		static bool CheckAABB(ICollider* col1, ICollider* col2);
		static bool CheckOBB(ICollider* col1, ICollider* col2);
		static bool CheckSphereToTriangle(ICollider* collider, const Triangle& triangle);
		static bool CheckSphereToAABB(ICollider* sphere, ICollider* box);
		static bool CheckSphereToOBB(ICollider* sphere, ICollider* box);
		static bool CheckSphereToRay(ICollider* sphere,const Math::Vector3& pos,const Math::Vector3& rayPos, const Math::Vector3& rayDir,Math::Vector3* hitPos = nullptr);
		static bool CheckSphereToMesh(ICollider* sphere, ICollider* mesh);
	};
}