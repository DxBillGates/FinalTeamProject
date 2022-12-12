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
	type = ColliderType::MESH;
}

void GE::MeshCollider::SetMesh(MeshData<Vertex_UV_Normal>* meshData)
{
	auto vertices = meshData->GetVertices();
	auto indices = meshData->GetIndices();

	Triangle triangle;
	for (int i = 0; i < indices->size(); i += 3)
	{
		triangle.pos1 = (*vertices)[(*indices)[i]].point;
		triangle.pos2 = (*vertices)[(*indices)[(UINT64)i + 1]].point;
		triangle.pos3 = (*vertices)[(*indices)[(UINT64)i + 2]].point;

		Math::Vector3 p1p2 = triangle.pos2 - triangle.pos1;
		Math::Vector3 p1p3 = triangle.pos3 - triangle.pos1;
		triangle.normal = Math::Vector3::Cross(p1p2, p1p3).Normalize();

		meshs.push_back(triangle);
	}
}

bool GE::MeshCollider::CheckHit(ICollider* collider, Math::Vector3& hitNormal)
{
	Math::Matrix4x4 matrix = gameObject->GetTransform()->GetMatrix();

	for (auto& mesh : meshs)
	{
		Triangle triangle = mesh;
		triangle.pos1 = Math::Matrix4x4::Transform(mesh.pos1, matrix);
		triangle.pos2 = Math::Matrix4x4::Transform(mesh.pos2, matrix);
		triangle.pos3 = Math::Matrix4x4::Transform(mesh.pos3, matrix);

		Math::Vector3 p1p2 = triangle.pos2 - triangle.pos1;
		Math::Vector3 p1p3 = triangle.pos3 - triangle.pos1;
		triangle.normal = Math::Vector3::Cross(p1p2, p1p3).Normalize();

		if (CollisionManager::CheckSphereToTriangle(collider, triangle))
		{
			hitNormal = triangle.normal;
			return true;
		}
	}

	return false;
}
