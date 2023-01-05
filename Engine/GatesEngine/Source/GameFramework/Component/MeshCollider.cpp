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
	boxCollider.SetGameObject(this->GetGameObject());
	boxCollider.SetGraphicsDevice(graphicsDevice);
	boxCollider.SetTransform(gameObject->GetTransform());
}

void GE::MeshCollider::Start()
{
	Math::Vector3 center = boxCollider.GetBounds().center;
	Math::Vector3 size = boxCollider.GetBounds().size;
	//center = Math::Matrix4x4::Transform(center, transform->GetMatrix());
	//size = Math::Matrix4x4::Transform(size, transform->GetMatrix());
	boxCollider.SetCenter(center);
	boxCollider.SetSize(size);
}

void GE::MeshCollider::Update(float deltaTime)
{
	boxCollider.Update(deltaTime);
}

void GE::MeshCollider::Draw()
{
	boxCollider.Draw();
}

void GE::MeshCollider::SetMesh(MeshData<Vertex_UV_Normal>* meshData)
{
	auto vertices = meshData->GetVertices();
	auto indices = meshData->GetIndices();

	Math::Vector3 minPos, maxPos;
	
	minPos = vertices[0][0].point;
	maxPos = vertices[0][0].point;
	auto CalculateEdgePosition = [&minPos, &maxPos](const Math::Vector3& point)
	{
		for (int i = 0; i < 3; ++i)
		{
			// 小さい場所の更新
			minPos.value[i] = (point.value[i] <= minPos.value[i]) ? point.value[i] : minPos.value[i];
			// 大きい場所の更新
			maxPos.value[i] = (point.value[i] >= maxPos.value[i]) ? point.value[i] : maxPos.value[i];
		}
	};

	Triangle triangle;
	for (int i = 0; i < indices->size(); i += 3)
	{
		triangle.pos1 = (*vertices)[(*indices)[i]].point;
		triangle.pos2 = (*vertices)[(*indices)[(UINT64)i + 1]].point;
		triangle.pos3 = (*vertices)[(*indices)[(UINT64)i + 2]].point;

		Math::Vector3 p1p2 = triangle.pos2 - triangle.pos1;
		Math::Vector3 p1p3 = triangle.pos3 - triangle.pos1;
		triangle.normal = Math::Vector3::Cross(p1p2, p1p3).Normalize();

		CalculateEdgePosition(triangle.pos1);
		CalculateEdgePosition(triangle.pos2);
		CalculateEdgePosition(triangle.pos3);

		meshs.push_back(triangle);
	}

	// meshColliderですべての判定をすると重くなるためそのメッシュを覆えるBoxCollider用の各種値を計算
	Math::Vector3 center, size;
	Math::Vector3 vector = (minPos + maxPos);
	center = vector / 2;
	size = maxPos - minPos;
	size = { fabsf(size.x),fabsf(size.y),fabsf(size.z) };

	boxCollider.SetCenter(center);
	boxCollider.SetSize(size);
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

GE::ICollider* GE::MeshCollider::GetBoxCollider()
{
	return &boxCollider;
}
