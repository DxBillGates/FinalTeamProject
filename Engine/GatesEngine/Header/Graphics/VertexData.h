#pragma once
#include "..\Util\Math\Vector2.h"
#include "..\Util\Math\Vector3.h"
#include "Color.h"

namespace GE
{
	struct Vertex
	{
		Math::Vector3 point;

		Vertex() : point({}) {}
		Vertex(const Math::Vector3& p) :point(p) {}
	};

	struct Vertex_UV
	{
		Math::Vector3 point;
		Math::Vector2 uv;

		Vertex_UV() : point({}), uv({}) {}
		Vertex_UV(const Math::Vector3& p, const Math::Vector2& u) : point(p), uv(u) {}
	};

	struct Vertex_Normal
	{
		Math::Vector3 point;
		Math::Vector3 normal;

		Vertex_Normal() : point({}), normal({}) {}
		Vertex_Normal(const Math::Vector3& p, const Math::Vector3& n) : point(p), normal(n) {}
	};

	struct Vertex_UV_Normal
	{
		Math::Vector3 point;
		Math::Vector2 uv;
		Math::Vector3 normal;

		Vertex_UV_Normal() : point({}), uv({}), normal({}) {}
		Vertex_UV_Normal(const Math::Vector3& p, const Math::Vector2& u, const Math::Vector3& n) : point(p), uv(u), normal(n) {}
	};

	static const int MAX_BONE_INDEX_COUNT = 4;
	struct Vertex_UV_Normal_Skin
	{
		Math::Vector3 point;
		Math::Vector2 uv;
		Math::Vector3 normal;
		unsigned int boneIndex[MAX_BONE_INDEX_COUNT];
		float boneWeight[MAX_BONE_INDEX_COUNT];

		Vertex_UV_Normal_Skin() : Vertex_UV_Normal_Skin(0,0,0) {}
		Vertex_UV_Normal_Skin(const Math::Vector3& p, const Math::Vector2& u, const Math::Vector3& n) : point(p), uv(u), normal(n),boneIndex(),boneWeight() {}
	};

	struct Vertex_UV_Normal_Color
	{
		Math::Vector3 point;
		Math::Vector2 uv;
		Math::Vector3 normal;
		Color color;
	};

	struct Vertex_Color
	{
		Math::Vector3 pos;
		Color color;
	};
}
