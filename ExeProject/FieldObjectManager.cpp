#include "FieldObjectManager.h"
#include "FieldObject.h"
#include "FieldTree.h"
#include <GatesEngine/Header/GameFramework/Component/SphereCollider.h>
#include <GatesEngine/Header/GameFramework/Component/MeshCollider.h>
#include <GatesEngine/Header/GameFramework/Component/BoxCollider.h>
#include <GatesEngine/Header/Graphics/MeshCreater.h>

#include<fstream>
#include<sstream>
#include<cassert>

GE::Math::Vector3 FieldObjectManager::FieldObjectManager::StartPosition;


FieldObjectManager* FieldObjectManager::GetInstance()
{
	static FieldObjectManager instance;
	return &instance;
}

void FieldObjectManager::LoadModels()
{
	LoadModel("mountain1");
	LoadModel("mountain2");
	LoadModel("mountain3");
	LoadModel("mountain4");
	LoadModel("mountain5");
}

void FieldObjectManager::Start(GE::GameObjectManager* gameObjectManager)
{
	this->gameObjectManager = gameObjectManager;

	auto meshManager = graphicsDevice->GetMeshManager();
	auto device = graphicsDevice->GetDevice();
	auto cmdList = graphicsDevice->GetCmdList();


	//スタートの止まり木
	{
		GE::MeshData<GE::Vertex_UV_Normal> startTreeModel;
		mesh = new GE::Mesh();
		GE::MeshCreater::LoadObjModelData("Resources/Model/tree2", startTreeModel);
		mesh->Create(device, cmdList, startTreeModel);
		meshManager->Add(mesh, "startTree");

		auto* object = gameObjectManager->AddGameObject(new GE::GameObject("StartTree", "StartTree"));
		auto* sampleComponent = object->AddComponent<StartTree>();
		auto* collider = object->AddComponent < GE::MeshCollider >();
		collider->SetMesh(&startTreeModel);
		startTree = object;
	}

	AddGroundModel("mountain1");
	AddGroundModel("mountain2");
	AddGroundModel("mountain3");
	AddGroundModel("mountain4");
	AddGroundModel("mountain5");
	//壁仮
	{
		auto* object = gameObjectManager->AddGameObject(new GE::GameObject("skydome", "skydome"));
		auto* sampleComponent = object->AddComponent<FieldObject>();
		object->GetComponent<FieldObject>()->modelName = "Skydome";
		object->GetComponent<FieldObject>()->shaderName = "CelestialSphereShader";
		object->GetComponent<FieldObject>()->textureName = "sky_Windows";
		object->SetColor(GE::Color(1.f, 1.f, 1.f, 1));
		object->GetTransform()->scale = { 400 };
		object->GetTransform()->rotation = GE::Math::Quaternion::Euler(GE::Math::Vector3(0, 0, 180));
		skydome = object;
	}
	//巣
	{
		auto* object = gameObjectManager->AddGameObject(new GE::GameObject("nest", "nest"));
		auto* sampleComponent = object->AddComponent<FieldObject>();
		object->GetComponent<FieldObject>()->modelName = "Nest";
		object->GetComponent<FieldObject>()->shaderName = "DefaultMeshShader";
		object->SetColor(GE::Color(0.8f, 0.6f, 0.6f, 1.f));
		object->GetTransform()->scale = { 200,300,200 };
		auto* collider = object->AddComponent < GE::SphereCollider >();
		collider->SetCenter({ 0,0,0 });
		collider->SetSize({ 10 });
		nest = object;
	}
	//通常の木
	{
		for (int i = 0; i < 7; ++i)
		{
			auto* object = gameObjectManager->AddGameObject(new GE::GameObject("fieldtree", "fieldtree"));
			auto* sampleComponent = object->AddComponent<FieldTree>();
			object->GetTransform()->position = {};
			object->GetTransform()->scale = { 400 };
			fieldTree.push_back(object);
			auto* collider = object->AddComponent < GE::BoxCollider >();
			collider->SetSize(GE::Math::Vector3(1.5f, 15, 1.5f));
			FieldObjectDeBugTransform::GetInstance()->AddTarget(object);
		}
	}
	//フィールドの草
	{
		for (int i = 0; i < 7; ++i)
		{
			auto* object = gameObjectManager->AddGameObject(new GE::GameObject("leaf", "leaf"));
			auto* sampleComponent = object->AddComponent<FieldObject>();
			object->GetTransform()->position = {};
			object->GetTransform()->scale = { 100 };
			object->GetComponent<FieldObject>()->modelName = "Ground_Leaf1";
			sampleComponent->shaderName = "DefaultMeshWithTextureAndAdsCompositiongShader";
			object->GetComponent<FieldObject>()->textureName = "leafTex1";
			fieldLeaf.push_back(object);
			FieldObjectDeBugTransform::GetInstance()->AddTarget(object);
		}
	}
	//
	{
		auto* object = gameObjectManager->AddGameObject(new GE::GameObject("tile", "tile"));
		auto* sampleComponent = object->AddComponent<FieldObject>();
		object->GetComponent<FieldObject>()->modelName = "Plane";
		sampleComponent->shaderName = "DefaultMeshWithShadowShader";
		object->GetTransform()->scale = { 100000,1,100000 };
		object->GetTransform()->position = { 0,-30.f,0 };
		object->SetColor(GE::Color(0.2f, 0.5f, 0.2f, 1.0f));
		auto* collider = object->AddComponent < GE::BoxCollider >();
		collider->SetSize(GE::Math::Vector3(100000, 1, 100000));

	}
}
void FieldObjectManager::AddGroundModel(std::string fileName)
{
	auto* object = gameObjectManager->AddGameObject(new GE::GameObject("ground", "ground"));
	auto* sampleComponent = object->AddComponent<FieldObject>();
	auto* collider = object->AddComponent < GE::MeshCollider >();
	collider->SetMesh(&groundModels[fileName]);
	object->GetComponent<FieldObject>()->modelName = fileName;
	sampleComponent->shaderName = "DefaultMeshWithShadowShader";
	object->GetComponent<FieldObject>()->shaderName = "DefaultMeshWithTextureAndAdsCompositiongShader";
	object->GetComponent<FieldObject>()->textureName = "groundTex1";
	object->GetTransform()->position = { 1000,0,-15000 };
	object->GetTransform()->scale = { 2000 };
	object->GetTransform()->rotation = GE::Math::Quaternion(GE::Math::Vector3(0, 1, 0), -5.0f);
}

void FieldObjectManager::LoadModel(const std::string& filename)
{
	//地形
	GE::MeshData<GE::Vertex_UV_Normal> model;
	auto meshManager = graphicsDevice->GetMeshManager();
	auto device = graphicsDevice->GetDevice();
	auto cmdList = graphicsDevice->GetCmdList();

	GE::MeshCreater::LoadObjModelData("Resources/Model/_Mountain/" + filename, model);
	mesh = new GE::Mesh();

	mesh->Create(device, cmdList, model);
	meshManager->Add(mesh, filename);

	groundModels[filename] = model;
}

void FieldObjectManager::LoadPosition(const std::string& filename)
{
	std::vector<obj> ft;
	std::vector<obj> fl;
	obj st;
	obj nst;

	std::ifstream file;
	//ファイルを開く
	file.open(filename);
	//ファイルが開けなかったらとまる
	if (file.fail()) {
		assert(0);
	}

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line)) {

		// 1行分の文字列
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "FieldTree") {
			obj result;
			line_stream >> result.pos.x;
			line_stream >> result.pos.y;
			line_stream >> result.pos.z;

			line_stream >> result.rot.x;
			line_stream >> result.rot.y;
			line_stream >> result.rot.z;

			line_stream >> result.scale.x;
			line_stream >> result.scale.y;
			line_stream >> result.scale.z;

			line_stream >> result.col.r;
			line_stream >> result.col.g;
			line_stream >> result.col.b;
			result.col.a = 1.0f;
			ft.emplace_back(result);
		}
		else if (key == "FieldLeaf")
		{
			obj result;
			line_stream >> result.pos.x;
			line_stream >> result.pos.y;
			line_stream >> result.pos.z;

			line_stream >> result.rot.x;
			line_stream >> result.rot.y;
			line_stream >> result.rot.z;

			line_stream >> result.scale.x;
			line_stream >> result.scale.y;
			line_stream >> result.scale.z;

			line_stream >> result.col.r;
			line_stream >> result.col.g;
			line_stream >> result.col.b;
			result.col.a = 1.0f;
			fl.emplace_back(result);
		}
		else if (key == "StartTree")
		{
			obj result;
			line_stream >> result.pos.x;
			line_stream >> result.pos.y;
			line_stream >> result.pos.z;

			line_stream >> result.rot.x;
			line_stream >> result.rot.y;
			line_stream >> result.rot.z;

			line_stream >> result.scale.x;
			line_stream >> result.scale.y;
			line_stream >> result.scale.z;

			line_stream >> result.col.r;
			line_stream >> result.col.g;
			line_stream >> result.col.b;
			result.col.a = 1.0f;
			st = result;
		}
		else if (key == "Nest")
		{
			obj result;
			line_stream >> result.pos.x;
			line_stream >> result.pos.y;
			line_stream >> result.pos.z;

			line_stream >> result.rot.x;
			line_stream >> result.rot.y;
			line_stream >> result.rot.z;

			line_stream >> result.scale.x;
			line_stream >> result.scale.y;
			line_stream >> result.scale.z;

			line_stream >> result.col.r;
			line_stream >> result.col.g;
			line_stream >> result.col.b;
			result.col.a = 1.0f;
			nst = result;
		}
	}
	file.close();
	//ファイルの座標セット
	int index = ft.size() < fieldTree.size() ? ft.size() : fieldTree.size();
	for (int i = 0; i < index; i++)
	{
		fieldTree[i]->GetTransform()->position = ft[i].pos;
		fieldTree[i]->GetTransform()->rotation = GE::Math::Quaternion::Euler(ft[i].rot);
		fieldTree[i]->GetTransform()->scale = ft[i].scale;
	}
	//草
	index = fl.size() < fieldLeaf.size() ? fl.size() : fieldLeaf.size();
	for (int i = 0; i < index; i++)
	{
		fieldLeaf[i]->GetTransform()->position = fl[i].pos;
		fieldLeaf[i]->GetTransform()->rotation = GE::Math::Quaternion::Euler(fl[i].rot);
		fieldLeaf[i]->GetTransform()->scale = fl[i].scale;
	}
	//開始時の木
	startTree->GetTransform()->position = st.pos;
	startTree->GetComponent<StartTree>()->rotation_euler = st.rot;
	startTree->GetTransform()->scale = st.scale;
	//巣
	nest->GetTransform()->position = nst.pos;
	nest->GetTransform()->scale = nst.scale;
	StartPosition = nst.pos;
	skydome->GetTransform()->position = StartPosition;
}

void FieldObjectManager::SaveCurrentPosition(const std::string& filename)
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//内容初期化
	writing_file.clear();
	if (!writing_file.is_open()) { assert(0); }
	//普通の木
	for (int i = 0; i < fieldTree.size(); i++)
	{
		GE::Math::Vector3 pos = fieldTree[i]->GetTransform()->position;
		GE::Math::Vector3 scale = fieldTree[i]->GetTransform()->scale;
		GE::Math::Vector3 rota = fieldTree[i]->GetTransform()->rotation.EulerAngle();
		GE::Color col = fieldTree[i]->GetColor();

		writing_file << "FieldTree " << pos.x << " " << pos.y << " " << pos.z <<
			" " << rota.x << " " << rota.y << " " << rota.z <<
			" " << scale.x << " " << scale.x << " " << scale.x <<
			" " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
	}
	//普通の木
	for (int i = 0; i < fieldLeaf.size(); i++)
	{
		GE::Math::Vector3 pos = fieldLeaf[i]->GetTransform()->position;
		GE::Math::Vector3 scale = fieldLeaf[i]->GetTransform()->scale;
		GE::Math::Vector3 rota = fieldLeaf[i]->GetTransform()->rotation.EulerAngle();
		GE::Color col = fieldLeaf[i]->GetColor();

		writing_file << "FieldLeaf " << pos.x << " " << pos.y << " " << pos.z <<
			" " << rota.x << " " << rota.y << " " << rota.z <<
			" " << scale.x << " " << scale.x << " " << scale.x <<
			" " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
	}
	//開始時の木
	GE::Math::Vector3 pos = startTree->GetTransform()->position;
	GE::Math::Vector3 scale = startTree->GetTransform()->scale;
	GE::Math::Vector3 rota = startTree->GetTransform()->rotation.EulerAngle();
	GE::Color col = startTree->GetColor();

	writing_file << "StartTree " << pos.x << " " << pos.y << " " << pos.z <<
		" " << rota.x << " " << rota.y << " " << rota.z <<
		" " << scale.x << " " << scale.y << " " << scale.z <<
		" " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;

	//巣
	pos = nest->GetTransform()->position;
	scale = nest->GetTransform()->scale;
	col = startTree->GetColor();

	writing_file << "Nest " << pos.x << " " << pos.y << " " << pos.z <<
		" " << rota.x << " " << rota.y << " " << rota.z <<
		" " << scale.x << " " << scale.y << " " << scale.z <<
		" " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;

	writing_file.close();
}

void FieldObjectManager::OtherUpdate()
{
	FieldObjectDeBugTransform::GetInstance()->Update();
}

void FieldObjectManager::OtherDraw()
{
	startTree->GetComponent<StartTree>()->TreeLeafDraw();

	for (int i = 0; i < fieldTree.size(); i++)
	{
		fieldTree[i]->GetComponent<FieldTree>()->TreeLeafDraw();

	}

	FieldObjectDeBugTransform::GetInstance()->Draw();
}

void FieldObjectManager::UnLoad()
{
	fieldTree.clear();
	fieldLeaf.clear();
	//delete mesh;
}