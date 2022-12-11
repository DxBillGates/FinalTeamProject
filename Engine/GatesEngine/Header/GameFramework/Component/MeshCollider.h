#pragma once
#include "Collider.h"
#include "..\..\Graphics\MeshData.h"
#include "..\..\Graphics\VertexData.h"

namespace GE
{
	class MeshCollider : public Collider
	{
	private:
		std::vector<Triangle> meshs;
	public:
		MeshCollider();
		~MeshCollider();
		void Awake() override;
		void SetMesh(MeshData<Vertex_UV_Normal>* meshData);
		bool CheckHit(ICollider* collider,Math::Vector3& hitNormal);
	};
}
