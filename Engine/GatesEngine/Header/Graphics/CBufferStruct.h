#pragma once
#include "..\Util\Math\Matrix4.h"
#include "..\Util\Math\Vector4.h"
#include "Color.h"

namespace GE
{
	struct ModelInfo
	{
		Math::Matrix4x4 modelMatrix = Math::Matrix4x4::Identity();
	};

	static const int MAX_BONES = 64;
	struct ModelSkinMeshInfo
	{
		Math::Matrix4x4 modelMatrix = Math::Matrix4x4::Identity();
		Math::Matrix4x4 bones[MAX_BONES];
	};

	struct CameraInfo
	{
		Math::Matrix4x4 viewMatrix;
		Math::Matrix4x4 projMatrix;
		Math::Matrix4x4 billboard = Math::Matrix4x4::Identity();
		Math::Matrix4x4 lightMatrix;
		Math::Matrix4x4 invViewMatrix;
		Math::Matrix4x4 invProjMatrix;
		Math::Vector4 cameraPos = { 0 };
		Math::Vector4 cameraDir = { 0 };
	};

	struct Material
	{
		Math::Vector4 ambient = { 0.25f };
		Math::Vector4 diffuse = { 1 };
		Math::Vector4 specular = { 10 };
		Color color = { 1 };
	};

	struct DirectionalLightInfo
	{
		Math::Vector4 worldLightDir = { 0,-1,0,0 };
		Math::Vector4 worldLightColor = { 1 };
	};

	struct TextureAnimationInfo
	{
		// �؂蔲���T�C�Y
		Math::Vector2 clipSize = { 0,0 };
		// �؂蔲���ʒu
		Math::Vector2 pivot;
		// ���e�N�X�`���T�C�Y
		Math::Vector2 textureSize;
	};
}