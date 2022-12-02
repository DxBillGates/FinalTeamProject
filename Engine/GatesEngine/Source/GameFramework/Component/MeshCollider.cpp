#include "..\..\..\Header\GameFramework\Component\MeshCollider.h"
#include "..\..\..\Header\GameFramework\Collision\CollisionManager.h"

GE::MeshCollider::MeshCollider()
{
}

GE::MeshCollider::~MeshCollider()
{
}

void GE::MeshCollider::Awake()
{
}

void GE::MeshCollider::SetMesh(MeshData<Vertex_UV_Normal>* meshData)
{
	auto vertices = meshData->GetVertices();
	auto indices = meshData->GetIndices();

	Triangle triangle;
	for (int i = 0; i < indices->size(); i += 3)
	{
		triangle.pos1 = (*vertices)[i].point;
		triangle.pos2 = (*vertices)[(UINT64)i + 1].point;
		triangle.pos3 = (*vertices)[(UINT64)i + 2].point;

		meshs.push_back(triangle);
	}
}

bool GE::MeshCollider::CheckHit(ICollider* collider)
{
	for (auto& mesh : meshs)
	{
		if (CollisionManager::CheckSphereToTriangle(collider, mesh))
		{
			return true;
		}
	}

	return false;
}
