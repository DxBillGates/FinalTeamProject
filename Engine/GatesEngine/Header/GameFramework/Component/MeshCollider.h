#pragma once
#include "Collider.h"
#include "..\..\Graphics\MeshData.h"
#include "..\..\Graphics\VertexData.h"
#include "..\..\GameFramework\Component\BoxCollider.h"

namespace GE
{
	class MeshCollider : public Collider
	{
	private:
		std::vector<Triangle> meshs;
		BoxCollider boxCollider;
	public:
		MeshCollider();
		~MeshCollider();
		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;
		void Draw() override;
		void SetMesh(MeshData<Vertex_UV_Normal>* meshData);
		bool CheckHit(ICollider* collider,Math::Vector3& hitNormal);
		ICollider* GetBoxCollider();
	};
}
