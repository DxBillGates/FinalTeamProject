#include "..\..\..\Header\GameFramework\Collision\CollisionManager.h"
#include "..\..\..\Header\GameFramework\GameObject\GameObject.h"
#include "..\..\..\Header\GameFramework\Component\MeshCollider.h"

#include <array>
#include <cmath>

void GE::CollisionManager::CollisionCheck(std::vector<GameObject*>* firstTagGameObjects, std::vector<GameObject*>* secondTagGameObjects)
{
	for (auto& firstTagGameObject : *firstTagGameObjects)
	{
		for (auto& secondTagGameObjects : *secondTagGameObjects)
		{
			if (firstTagGameObject->IsDestroy() == true || secondTagGameObjects->IsDestroy() == true)continue;

			// 前フレームでヒット判定されていたか
			bool isHitBeforeFrame = firstTagGameObject->CheckBeforeFrameHitObject(secondTagGameObjects);
			bool isHitCurrentFrame = CheckHit(firstTagGameObject->GetCollider(), secondTagGameObjects->GetCollider());

			bool enter, stay, exit;
			enter = stay = exit = false;

			if (isHitCurrentFrame == true)
			{
				// 前フレームも現フレームもヒットしていた場合
				// OnCollision
				if (isHitBeforeFrame == true)
				{
					stay = true;
				}
				// 現フレームのみヒット
				// OnCollisionExit
				else
				{
					enter = true;
				}
			}
			else
			{
				if (isHitBeforeFrame == false)continue;

				// 前フレームまでヒットしていた場合
				// OnCollisionExit
				exit = true;
			}

			if (enter == false && stay == false && exit == false)continue;

			firstTagGameObject->GetCollider()->Hit();
			firstTagGameObject->OnCollision(secondTagGameObjects, enter, stay, exit);
			secondTagGameObjects->GetCollider()->Hit();
			secondTagGameObjects->OnCollision(firstTagGameObject, enter, stay, exit);

		}
	}
}

GE::Math::Vector3 GE::CollisionManager::CheckClosestPoint(const Math::Vector3& point, const Triangle& triangle)
{
	Math::Vector3 center = point;
	Math::Vector3 returnPosition;

	// ポイント間のベクトル
	Math::Vector3 p1_p2, p3_p1;
	p1_p2 = triangle.pos2 - triangle.pos1;
	p3_p1 = triangle.pos1 - triangle.pos3;

	// 各ポイントとコライダーの真ん中間のベクトル
	Math::Vector3 p1_pt, p2_pt, p3_pt;
	p1_pt = center - triangle.pos1;
	p2_pt = center - triangle.pos2;
	p3_pt = center - triangle.pos3;

	float d1, d2, d3, d4, d5, d6;
	d1 = Math::Vector3::Dot(p1_p2, p1_pt);
	d2 = Math::Vector3::Dot(-p3_p1, p1_pt);

	if (d1 <= 0 && d2 <= 0)
	{
		returnPosition = triangle.pos1;
		return returnPosition;
	}

	d3 = Math::Vector3::Dot(p1_p2, p2_pt);
	d4 = Math::Vector3::Dot(-p3_p1, p2_pt);

	if (d3 >= 0 && d4 <= d3)
	{
		returnPosition = triangle.pos2;
		return returnPosition;
	}

	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0 && d1 >= 0 && d3 <= 0)
	{
		float v = d1 / (d1 - d3);
		returnPosition = triangle.pos1 + v * p1_p2;
		return returnPosition;
	}

	d5 = Math::Vector3::Dot(p1_p2, p3_pt);
	d6 = Math::Vector3::Dot(-p3_p1, p3_pt);

	if (d5 >= 0 && d5 <= d6)
	{
		returnPosition = triangle.pos3;
		return returnPosition;
	}

	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0 && d2 >= 0 && d6 <= 0)
	{
		float v = d2 / (d2 - d6);
		returnPosition = triangle.pos1 + v * -p3_p1;
		return returnPosition;
	}

	float va = d3 * d6 - d5 * d4;
	if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0)
	{
		float v = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		returnPosition = triangle.pos1 + v * (triangle.pos2 - triangle.pos1);
		return returnPosition;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	returnPosition = triangle.pos1 + p1_p2 * v + -p3_p1 * w;
	return returnPosition;
}

void GE::CollisionManager::AddTagCombination(const std::string& tag1, const std::string& tag2)
{
	bool isFind = false;

	// 一つ目のタグが登録されているか
	isFind = collisionTagCombination.find(tag1) != collisionTagCombination.end() ? true : false;

	// 一つ目のタグが登録されていないならまずそれを登録する
	if (isFind == false)
	{
		collisionTagCombination.insert(std::make_pair(tag1, std::vector<std::string>()));
	}

	collisionTagCombination[tag1].push_back(tag2);
}

void GE::CollisionManager::SetGameObjectManager(std::map<std::string, std::vector<GameObject*>>* pManager)
{
	pGameObjects = pManager;
}

void GE::CollisionManager::Update()
{
	bool isTagFinds = false;

	// タグの組み合わせに登録されているタグがゲームオブジェクトの配列に登録されているかを確認
	for (auto& tagCombination : collisionTagCombination)
	{
		// ゲームオブジェクトの配列にタグが登録されているかを確認する
		isTagFinds = ((*pGameObjects).find(tagCombination.first) != (*pGameObjects).end());

		for (auto& tag : tagCombination.second)
		{
			isTagFinds = ((*pGameObjects).find(tag) != (*pGameObjects).end());
			if (isTagFinds == false)continue;
			CollisionCheck(&(*pGameObjects)[tagCombination.first], &(*pGameObjects)[tag]);
		}
	}
}

bool GE::CollisionManager::CheckHit(ICollider* col1, ICollider* col2)
{
	ColliderType col1Type = col1->GetType();
	ColliderType col2Type = col2->GetType();

	int bitResult = 0;
	bitResult = (int)col2Type | (int)col1Type;
	bool collisionCheck = false;

	switch ((CollisionBitCombination)bitResult)
	{
	case GE::CollisionBitCombination::NONE:
		break;
	case GE::CollisionBitCombination::SPHERE_SPHERE:
		collisionCheck = CheckSphere(col1, col2);
		break;
	case GE::CollisionBitCombination::SPHERE_AABB:
		if (col1Type == ColliderType::SPHERE)collisionCheck = CheckSphereToAABB(col1, col2);
		else collisionCheck = CheckSphereToAABB(col2, col1);
		break;
	case GE::CollisionBitCombination::SPHERE_OBB:
		if (col1Type == ColliderType::SPHERE)collisionCheck = CheckSphereToOBB(col1, col2);
		else collisionCheck = CheckSphereToOBB(col2, col1);
		break;
	case GE::CollisionBitCombination::SPHERE_CAPSULE:
		break;
	case GE::CollisionBitCombination::SPHERE_MESH:
		if (col1Type == ColliderType::SPHERE)collisionCheck = CheckSphereToMesh(col1, col2);
		else collisionCheck = CheckSphereToMesh(col2, col1);
		break;
	case GE::CollisionBitCombination::AABB_AABB:
		collisionCheck = CheckAABB(col1, col2);
		break;
	case GE::CollisionBitCombination::AABB_OBB:
		break;
	case GE::CollisionBitCombination::AABB_CAPSULE:
		break;
	case GE::CollisionBitCombination::OBB_OBB:
		collisionCheck = CheckOBB(col1, col2);
		break;
	case GE::CollisionBitCombination::OBB_CAPSULE:
		break;
	case GE::CollisionBitCombination::CAPSULE_CAPSULE:
		break;
	default:
		break;
	}

	return collisionCheck;
}

bool GE::CollisionManager::CheckSphere(ICollider* col1, ICollider* col2)
{
	Math::Vector3 center1 = col1->GetMatrix().GetPosition();
	Math::Vector3 center2 = col2->GetMatrix().GetPosition();

	Math::Vector3 scale1 = col1->GetBounds().size * col1->GetParent()->scale;
	Math::Vector3 scale2 = col2->GetBounds().size * col2->GetParent()->scale;

	float distance = Math::Vector3::Distance(center1, center2);
	return (distance * distance <= scale1.x * scale2.x);
}

bool GE::CollisionManager::CheckAABB(ICollider* col1, ICollider* col2)
{
	Math::Vector3 min1, min2, max1, max2;
	min1 = col1->GetBounds().min;
	max1 = col1->GetBounds().max;
	min2 = col2->GetBounds().min;
	max2 = col2->GetBounds().max;

	if (min1.x > max2.x)return false;
	if (max1.x < min2.x)return false;
	if (min1.y > max2.y)return false;
	if (max1.y < min2.y)return false;
	if (min1.z > max2.z)return false;
	if (max1.z < min2.z)return false;
	return true;
}

bool GE::CollisionManager::CheckOBB(ICollider* col1, ICollider* col2)
{
	const Bounds& bound1 = col1->GetBounds();
	const Bounds& bound2 = col2->GetBounds();
	const Math::Axis& axis1 = col1->GetAxis();
	const Math::Axis& axis2 = col2->GetAxis();


	Math::Vector3 center1 = col1->GetMatrix().GetPosition();
	Math::Vector3 center2 = col2->GetMatrix().GetPosition();
	const Math::Vector3 intervalVec = center2 - center1;


	Math::Vector3 scale1 = bound1.size * col1->GetParent()->scale / 2;
	Math::Vector3 scale2 = bound2.size * col2->GetParent()->scale / 2;

	Math::Vector3 ae1 = axis1.x * scale1.x;
	Math::Vector3 ae2 = axis1.y * scale1.y;
	Math::Vector3 ae3 = axis1.z * scale1.z;
	Math::Vector3 be1 = axis2.x * scale2.x;
	Math::Vector3 be2 = axis2.y * scale2.y;
	Math::Vector3 be3 = axis2.z * scale2.z;

	auto LengthSegmentSeparateAxis = [](const Math::Vector3& sep, const Math::Vector3& e1, const Math::Vector3& e2, const Math::Vector3& e3 = 0)
	{
		float r1 = std::fabs(Math::Vector3::Dot(sep, e1));
		float r2 = std::fabs(Math::Vector3::Dot(sep, e2));
		float r3 = (e3.Length() != 0) ? std::fabs(Math::Vector3::Dot(sep, e3)) : 0;
		return r1 + r2 + r3;
	};

	// 分離軸 Ae1
	float rA = ae1.Length();
	float rB = LengthSegmentSeparateAxis(axis1.x, be1, be2, be3);
	float l = std::fabs(Math::Vector3::Dot(intervalVec, axis1.x));
	if (l > rA + rB)return false;

	// 分離軸 Ae2
	rA = ae2.Length();
	rB = LengthSegmentSeparateAxis(axis1.y, be1, be2, be3);
	l = std::fabs(Math::Vector3::Dot(intervalVec, axis1.y));
	if (l > rA + rB)return false;

	// 分離軸 Ae3
	rA = ae3.Length();
	rB = LengthSegmentSeparateAxis(axis1.z, be1, be2, be3);
	l = std::fabs(Math::Vector3::Dot(intervalVec, axis1.z));
	if (l > rA + rB)return false;

	// 分離軸 Be1
	rA = LengthSegmentSeparateAxis(axis2.x, ae1, ae2, ae3);
	rB = be1.Length();
	l = std::fabs(Math::Vector3::Dot(intervalVec, axis2.x));
	if (l > rA + rB)return false;

	// 分離軸 Be2
	rA = LengthSegmentSeparateAxis(axis2.y, ae1, ae2, ae3);
	rB = be2.Length();
	l = std::fabs(Math::Vector3::Dot(intervalVec, axis2.y));
	if (l > rA + rB)return false;

	// 分離軸 Be3
	rA = LengthSegmentSeparateAxis(axis2.z, ae1, ae2, ae3);
	rB = be3.Length();
	l = std::fabs(Math::Vector3::Dot(intervalVec, axis2.z));
	if (l > rA + rB)return false;

	// 分離軸 C11
	Math::Vector3 cross = Math::Vector3::Cross(axis1.x, axis2.x);
	rA = LengthSegmentSeparateAxis(cross, ae2, ae3);
	rB = LengthSegmentSeparateAxis(cross, be2, be3);
	l = std::fabs(Math::Vector3::Dot(intervalVec, cross));
	if (l > rA + rB)return false;

	// 分離軸 C12
	cross = Math::Vector3::Cross(axis1.x, axis2.y);
	rA = LengthSegmentSeparateAxis(cross, ae2, ae3);
	rB = LengthSegmentSeparateAxis(cross, be1, be3);
	l = std::fabs(Math::Vector3::Dot(intervalVec, cross));
	if (l > rA + rB)return false;

	// 分離軸 C13
	cross = Math::Vector3::Cross(axis1.x, axis2.z);
	rA = LengthSegmentSeparateAxis(cross, ae2, ae3);
	rB = LengthSegmentSeparateAxis(cross, be1, be2);
	l = std::fabs(Math::Vector3::Dot(intervalVec, cross));
	if (l > rA + rB)return false;

	// 分離軸 C21
	cross = Math::Vector3::Cross(axis1.y, axis2.x);
	rA = LengthSegmentSeparateAxis(cross, ae1, ae3);
	rB = LengthSegmentSeparateAxis(cross, be2, be3);
	l = std::fabs(Math::Vector3::Dot(intervalVec, cross));
	if (l > rA + rB)return false;

	// 分離軸 C22
	cross = Math::Vector3::Cross(axis1.y, axis2.y);
	rA = LengthSegmentSeparateAxis(cross, ae1, ae3);
	rB = LengthSegmentSeparateAxis(cross, be1, be3);
	l = std::fabs(Math::Vector3::Dot(intervalVec, cross));
	if (l > rA + rB)return false;

	// 分離軸 C23
	cross = Math::Vector3::Cross(axis1.y, axis2.z);
	rA = LengthSegmentSeparateAxis(cross, ae1, ae3);
	rB = LengthSegmentSeparateAxis(cross, be1, be2);
	l = std::fabs(Math::Vector3::Dot(intervalVec, cross));
	if (l > rA + rB)return false;

	// 分離軸 C31
	cross = Math::Vector3::Cross(axis1.z, axis2.x);
	rA = LengthSegmentSeparateAxis(cross, ae1, ae2);
	rB = LengthSegmentSeparateAxis(cross, be2, be3);
	l = std::fabs(Math::Vector3::Dot(intervalVec, cross));
	if (l > rA + rB)return false;

	// 分離軸 C32
	cross = Math::Vector3::Cross(axis1.z, axis2.y);
	rA = LengthSegmentSeparateAxis(cross, ae1, ae2);
	rB = LengthSegmentSeparateAxis(cross, be1, be3);
	l = std::fabs(Math::Vector3::Dot(intervalVec, cross));
	if (l > rA + rB)return false;

	// 分離軸 C33
	cross = Math::Vector3::Cross(axis1.z, axis2.z);
	rA = LengthSegmentSeparateAxis(cross, ae1, ae2);
	rB = LengthSegmentSeparateAxis(cross, be1, be2);
	l = std::fabs(Math::Vector3::Dot(intervalVec, cross));
	if (l > rA + rB)return false;

	return true;
}

bool GE::CollisionManager::CheckSphereToTriangle(ICollider* collider, const Triangle& triangle)
{
	Math::Vector3 closestPoint = CheckClosestPoint(collider->GetMatrix().GetPosition(), triangle);

	Math::Vector3 v = closestPoint - collider->GetMatrix().GetPosition();
	float distance = Math::Vector3::Dot(v, v);

	if (distance >= collider->GetBounds().size.x * collider->GetBounds().size.x)return false;

	return true;
}

bool GE::CollisionManager::CheckSphereToAABB(ICollider* sphere, ICollider* box)
{
	const Bounds& sphereBounds = sphere->GetBounds();
	const Bounds& boxBounds = box->GetBounds();

	Math::Vector3 sphereCenter = sphere->GetMatrix().GetPosition();
	Math::Vector3 localColliderRadius = sphereBounds.size;
	Math::Vector3 parentScale = sphere->GetParent()->scale;
	Math::Vector3 worldColliderSize = (localColliderRadius * parentScale) / 2;

	float radius = (worldColliderSize.x + worldColliderSize.y + worldColliderSize.z) / 3;

	float length = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (sphereCenter.value[i] < boxBounds.min.value[i])
		{
			length += (sphereCenter.value[i] - boxBounds.min.value[i]) * (sphereCenter.value[i] - boxBounds.min.value[i]);
		}
		if (sphereCenter.value[i] > boxBounds.max.value[i])
		{
			length += (sphereCenter.value[i] - boxBounds.max.value[i]) * (sphereCenter.value[i] - boxBounds.max.value[i]);
		}
	}

	if (length < radius * radius)
	{
		return true;
	}

	return false;
}

bool GE::CollisionManager::CheckSphereToOBB(ICollider* sphere, ICollider* box)
{
	const Bounds& sphereBounds = sphere->GetBounds();
	const Bounds& boxBounds = box->GetBounds();
	const Math::Axis& boxAxis = box->GetAxis();
	Math::Vector3 boxColliderSize = boxBounds.size * box->GetParent()->scale;

	Math::Vector3 boxCenter = box->GetMatrix().GetPosition();
	Math::Vector3 sphereCenter = sphere->GetMatrix().GetPosition();
	Math::Vector3 localColliderRadius = sphereBounds.size;
	Math::Vector3 parentScale = sphere->GetParent()->scale;
	Math::Vector3 worldColliderSize = (localColliderRadius * parentScale) / 2;

	float radius = (worldColliderSize.x + worldColliderSize.y + worldColliderSize.z) / 3;

	Math::Vector3 vec;
	for (int i = 0; i < 3; ++i)
	{
		Math::Vector3 axis = boxAxis.value[i] * boxColliderSize.value[i];
		float l = axis.Length() / 2;
		if (l <= 0)continue;
		float s = Math::Vector3::Dot((sphereCenter - boxCenter), boxAxis.value[i]) / l;

		s = std::fabs(s);
		if (s > 1)
		{
			vec += (1 - s) * l * boxAxis.value[i];
		}
	}

	float length = vec.Length();
	if (length < radius)
	{
		return true;
	}

	return false;
}

bool GE::CollisionManager::CheckSphereToRay(ICollider* sphere, const Math::Vector3& pos, const Math::Vector3& rayPos, const Math::Vector3& rayDir, Math::Vector3* hitPos)
{
	const Bounds& sphereBounds = sphere->GetBounds();
	Math::Vector3 worldColliderSize = (sphereBounds.size * sphere->GetParent()->scale) / 2;

	float radius = (worldColliderSize.x + worldColliderSize.y + worldColliderSize.z) / 3;

	// レイの始点から球の中心へのベクトル
	Math::Vector3 rayToCenterVector = pos - rayPos;

	// 解の公式に当てはめるための準備
	float a = Math::Vector3::Dot(rayDir, rayDir);
	float b = Math::Vector3::Dot(rayDir, rayToCenterVector);
	float c = Math::Vector3::Dot(rayToCenterVector, rayToCenterVector) - radius * radius;

	float result = b * b - a * c;
	if (result < 0)return false;

	result = std::sqrtf(result);
	float a1 = (b + result) / a;
	float a2 = (b - result) / a;

	// レイの逆方向への計算もされるためレイの逆方向であたっていると判断されることがある
	if (a1 < 0 || a2 < 0)return false;

	// ヒットしていた場所の算出
	if (hitPos == nullptr)return true;

	// 当たり始め
	*hitPos = rayPos + a1 * rayDir;
	// 当たり終わり
	*hitPos = rayPos + a2 * rayDir;

	return true;
}

bool GE::CollisionManager::CheckSphereToMesh(ICollider* sphere, ICollider* mesh)
{
	MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(mesh);
	bool result = false;
	result = meshCollider->CheckHit(sphere);
	return result;
}
