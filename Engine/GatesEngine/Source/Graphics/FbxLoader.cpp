#include "..\..\Header\Graphics\FbxLoader.h"
#include "..\..\Header\Util\Math\Math.h"
#include "..\..\Header\Util\Utility.h"
#include <assert.h>

FbxManager* GE::FbxLoader::fbxManager = nullptr;
FbxImporter* GE::FbxLoader::fbxImporter = nullptr;
bool GE::FbxLoader::isInitialize = false;
const std::string GE::FbxLoader::BASE_DIRECTORY = "Resources/Model/fbxModel/";
GE::MeshData<GE::Vertex_UV_Normal_Skin>* GE::FbxLoader::currentLoadModelMeshData = nullptr;

void GE::FbxLoader::ParseNodeRecursive(FbxNode* fbxNode, std::vector<Node>& nodes, Node* parentNode)
{
	std::string nodeName = fbxNode->GetName();

	nodes.emplace_back();
	Node& node = nodes.back();

	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	node.scaling = { (float)scaling[0],(float)scaling[1] ,(float)scaling[2] };
	node.rotation = { (float)rotation[0],(float)rotation[1] ,(float)rotation[2] };
	// ラジアンに変換
	node.rotation = { Math::ConvertToRadian(node.rotation.x),Math::ConvertToRadian(node.rotation.y) ,Math::ConvertToRadian(node.rotation.z) };
	node.translation = { (float)translation[0],(float)translation[1] ,(float)translation[2] };

	// ローカル変形行列作成
	Math::Matrix4x4 matScaling, matRotation, matTranslation;
	matScaling = Math::Matrix4x4::Scale(node.scaling);
	matRotation = Math::Matrix4x4::RotationZXY(node.rotation);
	matTranslation = Math::Matrix4x4::Translate(node.translation);
	node.transform = matScaling * matRotation * matTranslation;
	// 親ノードの行列を反映
	node.globalTransform = node.transform;
	if (parentNode)
	{
		node.globalTransform *= parentNode->transform;
	}

	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
	if (fbxNodeAttribute)
	{
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			ParseMesh(fbxNode);
		}
	}

	// 子ノードに対して再帰
	for (int i = 0; i < fbxNode->GetChildCount(); ++i)
	{
		ParseNodeRecursive(fbxNode->GetChild(i), nodes, &node);
	}
}

void GE::FbxLoader::ParseMesh(FbxNode* fbxNode)
{
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	ParseMeshVertices(fbxMesh);
	ParseMeshFaces(fbxMesh);
	ParseMaterial(fbxNode);
}

void GE::FbxLoader::ParseMeshVertices(FbxMesh* fbxMesh)
{
	auto& vertices = *currentLoadModelMeshData->GetVertices();

	const int CONTROL_POINTS_COUNT = fbxMesh->GetControlPointsCount();
	vertices.resize(CONTROL_POINTS_COUNT, Vertex_UV_Normal_Skin());

	FbxVector4* pCoord = fbxMesh->GetControlPoints();

	for (int i = 0; i < CONTROL_POINTS_COUNT; ++i)
	{
		Vertex_UV_Normal_Skin& vertex = vertices[i];
		vertex.point = { (float)pCoord[i][0],(float)pCoord[i][1] ,(float)pCoord[i][2] };
	}
}

void GE::FbxLoader::ParseMeshFaces(FbxMesh* fbxMesh)
{
	auto& vertices = *currentLoadModelMeshData->GetVertices();
	auto& indices = *currentLoadModelMeshData->GetIndices();

	const int POLYGON_COUNT = fbxMesh->GetPolygonCount();
	const int TEXTURE_UV_COUNT = fbxMesh->GetTextureUVCount();

	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	for (int i = 0; i < POLYGON_COUNT; ++i)
	{
		const int POLYGON_SIZE = fbxMesh->GetPolygonSize(i);

		for (int j = 0; j < POLYGON_SIZE; ++j)
		{
			int index = fbxMesh->GetPolygonVertex(i, j);

			Vertex_UV_Normal_Skin& vertex = vertices[index];

			// 法線取得
			FbxVector4 normal;
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertex.normal = { (float)normal[0],(float)normal[1] ,(float)normal[2] };
			}

			// uv取得
			if (TEXTURE_UV_COUNT > 0)
			{
				FbxVector2 uvs;
				bool lUmmappedUV;

				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUmmappedUV))
				{
					vertex.uv = { (float)uvs[0],(float)uvs[1] };
				}
			}

			// 頂点数3まで
			if (j < 3)
			{
				indices.push_back(index);
			}
			// それ以上
			else
			{
				int indicesSize = (int)indices.size();
				int index0 = indices[(int64_t)indicesSize - 3];
				int index2 = indices[(int64_t)indicesSize - 1];
				int index3 = index;

				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}
}

void GE::FbxLoader::ParseMaterial(FbxNode* fbxNode)
{
}

void GE::FbxLoader::LoadTexture(const std::string& fullpath)
{
}

const GE::SkinMeshData& GE::FbxLoader::Load(const std::string& modelName, IGraphicsDeviceDx12* graphicsDevice)
{
	const std::string FILE_NAME = modelName + ".fbx";
	const std::string FULLPATH = BASE_DIRECTORY + FILE_NAME;

	if (fbxImporter->Initialize(FULLPATH.c_str(), -1, fbxManager->GetIOSettings()) == false)
	{
		std::string error = FILE_NAME + "はロードに失敗しました。";
		Utility::Printf(error.c_str());
	}

	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

	fbxImporter->Import(fbxScene);

	std::vector<Node> nodes;
	nodes.reserve(fbxScene->GetNodeCount());

	MeshData<Vertex_UV_Normal_Skin> meshData;
	currentLoadModelMeshData = &meshData;

	ParseNodeRecursive(fbxScene->GetRootNode(), nodes);

	SkinMeshData skinMeshData;
	skinMeshData.mesh = new Mesh();
	skinMeshData.mesh->Create(graphicsDevice->GetDevice(), graphicsDevice->GetCmdList(), meshData);

	fbxScene->Destroy();

	return skinMeshData;
}

void GE::FbxLoader::Initialize()
{
	if (isInitialize == true)return;

	fbxManager = FbxManager::Create();

	FbxIOSettings* ioSetting = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ioSetting);

	fbxImporter = FbxImporter::Create(fbxManager, "");

	isInitialize = true;
}

void GE::FbxLoader::Finalize()
{
	if (isInitialize == false)return;

	fbxImporter->Destroy();
	fbxManager->Destroy();
}
