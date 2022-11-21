#pragma once
#include "..\Util\Math\Vector3.h"
#include "..\Util\Math\Matrix4.h"
#include "..\..\Header\Graphics\MeshData.h"
#include "..\..\Header\Graphics\VertexData.h"
#include "..\..\Header\Graphics\Mesh.h"
#include "..\..\Header\Util\Manager.h"
#include "..\..\External\FBX\fbxsdk.h"

#include <string>
#include <vector>

namespace GE
{
	struct Bone
	{
		std::string name;
		Math::Matrix4x4 invInitialPose;
		FbxCluster* fbxCluster;

		Bone(const std::string& name)
			: name(name)
			, invInitialPose()
			, fbxCluster(nullptr)
		{}
	};

	struct SkinMeshAnimationData
	{
		FbxAnimStack* animStack;
		// アニメーションの名前
		std::string name;
		// fbxSceneに登録されている要素番号
		int index;
		// アニメーションの開始時間
		FbxTime startTime;
		// アニメーションの終了時間
		FbxTime endTime;
		// 1フレームの時間
		FbxTime frameTime;
	};

	struct SkinMeshData
	{
		FbxScene* fbxScene;
		// メッシュ情報
		Mesh* mesh;
		// ボーン
		std::vector<Bone> bones;
		// アニメーションデータ
		std::vector<SkinMeshAnimationData> animationDatas;

		SkinMeshData() : fbxScene(nullptr), mesh(nullptr) {}
		~SkinMeshData() { fbxScene->Destroy(); }
	};

	class SkinMeshManager
	{
	private:
		Manager<SkinMeshData>* manager;
	private:
		SkinMeshManager();
		SkinMeshManager(const SkinMeshManager&) = delete;
		~SkinMeshManager() = default;
		SkinMeshManager& operator=(const SkinMeshManager&) = delete;
	public:
		static SkinMeshManager* GetInstance();
		void Finalize();
		void Add(SkinMeshData* addSkinMeshData,const std::string& setName);
		SkinMeshData* Get(const std::string& name);
	};
}
