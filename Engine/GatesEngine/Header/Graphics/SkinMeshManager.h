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
		// �A�j���[�V�����̖��O
		std::string name;
		// fbxScene�ɓo�^����Ă���v�f�ԍ�
		int index;
		// �A�j���[�V�����̊J�n����
		FbxTime startTime;
		// �A�j���[�V�����̏I������
		FbxTime endTime;
		// 1�t���[���̎���
		FbxTime frameTime;
	};

	struct SkinMeshData
	{
		FbxScene* fbxScene;
		// ���b�V�����
		Mesh* mesh;
		// �{�[��
		std::vector<Bone> bones;
		// �A�j���[�V�����f�[�^
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
