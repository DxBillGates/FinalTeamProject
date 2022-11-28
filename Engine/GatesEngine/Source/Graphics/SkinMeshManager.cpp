#include "..\..\Header\Graphics\SkinMeshManager.h"

GE::SkinMeshManager::SkinMeshManager()
	: manager(new Manager<SkinMeshData>())
{
}

GE::SkinMeshManager* GE::SkinMeshManager::GetInstance()
{
	static SkinMeshManager instance;
	return &instance;
}

void GE::SkinMeshManager::Finalize()
{
	delete manager;
}

void GE::SkinMeshManager::Add(SkinMeshData* addSkinMeshData, const std::string& setName)
{
	manager->Add(addSkinMeshData, setName);
}

GE::SkinMeshData* GE::SkinMeshManager::Get(const std::string& name)
{
	return manager->Get(name);
}
