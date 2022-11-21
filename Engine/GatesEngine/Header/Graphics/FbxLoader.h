#pragma once
#include "..\..\Header\Graphics\SkinMeshManager.h"
#include "..\..\Header\Graphics\IGraphicsDeviceDx12.h"

namespace GE
{
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
		static SkinMeshData* currentLoadModelData;

		static bool isInitialize;
	private:
		static void ParseNodeRecursive(FbxNode* fbxNode,std::vector<Node>& nodes,Node* parentNode = nullptr);
		static void ParseMesh(FbxNode* fbxNode);
		static void ParseMeshVertices(FbxMesh* fbxMesh);
		static void ParseMeshFaces(FbxMesh* fbxMesh);
		static void ParseMaterial(FbxNode* fbxNode);
		static void ParseAnimation(FbxScene* fbxScene, int animationCount);
		static void ParseSkin(FbxMesh* fbxMesh,std::vector<Bone>& bones);
		static void LoadTexture(const std::string& fullpath);
	public:
		static Mesh* Load(const std::string& modelName,IGraphicsDeviceDx12* graphicsDevice);

		static void Initialize();
		static void Finalize();
	};
}