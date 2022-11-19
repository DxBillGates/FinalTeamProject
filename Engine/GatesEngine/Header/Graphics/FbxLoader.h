#pragma once
#include "..\Util\Math\Vector3.h"
#include "..\Util\Math\Matrix4.h"
#include "..\..\Header\Graphics\MeshData.h"
#include "..\..\Header\Graphics\VertexData.h"
#include "..\..\Header\Graphics\Mesh.h"
#include "..\..\Header\Graphics\IGraphicsDeviceDx12.h"

#include <fbxsdk.h>
#include <string>
#include <vector>

namespace GE
{
	struct SkinMeshData
	{
		// メッシュ情報
		Mesh* mesh;
		// アニメーションデータ

	};

	class FbxLoader
	{
	private:
		struct Node
		{
			std::string name;
			Math::Vector3 scaling;
			Math::Vector3 rotation;
			Math::Vector3 translation;
			Math::Matrix4x4 transform;
			Math::Matrix4x4 globalTransform;

			Node* parentNode;

			Node()
				: name(std::string()),scaling(Math::Vector3()),rotation(Math::Vector3()),translation(Math::Vector3())
				, transform(Math::Matrix4x4()),globalTransform(Math::Matrix4x4()),parentNode(nullptr)
			{
			}
		};
	private:
		static FbxManager* fbxManager;
		static FbxImporter* fbxImporter;
		static const std::string BASE_DIRECTORY;

		static MeshData<Vertex_UV_Normal_Skin>* currentLoadModelMeshData;

		static bool isInitialize;
	private:
		static void ParseNodeRecursive(FbxNode* fbxNode,std::vector<Node>& nodes,Node* parentNode = nullptr);
		static void ParseMesh(FbxNode* fbxNode);
		static void ParseMeshVertices(FbxMesh* fbxMesh);
		static void ParseMeshFaces(FbxMesh* fbxMesh);
		static void ParseMaterial(FbxNode* fbxNode);
		static void LoadTexture(const std::string& fullpath);
	public:
		static const SkinMeshData& Load(const std::string& modelName,IGraphicsDeviceDx12* graphicsDevice);

		static void Initialize();
		static void Finalize();
	};
}