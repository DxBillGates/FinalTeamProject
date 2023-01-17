#include <GatesEngine/Header/Util/Utility.h>
#include <GatesEngine/Header/Util/Random.h>
#include <GatesEngine/Header/Graphics\Window.h>
#include<fstream>
#include<sstream>
#include<cassert>

#include"ScreenUI.h"
#include"PlayerComponent.h"
#include"Option.h"
#include"Title.h"
#include"TimeLimit.h"



ScreenUIManager* ScreenUIManager::GetInstance()
{
	static ScreenUIManager instance;
	return &instance;
}

ScreenUIManager::SpriteInfo ScreenUIManager::Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName)
{
	SpriteInfo result;
	result.transform.position = pos;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	return result;
}

ScreenUIManager::SpriteInfo ScreenUIManager::Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName, GE::Math::Vector2 texSize, GE::Math::Vector2 clipSize)
{
	SpriteInfo result;
	result.transform.position = pos;
	result.transform.scale = scale;
	result.color = color;
	result.textureName = textureName;
	result.texSize = texSize;
	result.clipSize = clipSize;
	return result;
}
void ScreenUIManager::Start()
{
	GE::Math::Vector2 winSize = GE::Window::GetWindowSize();
	GE::Math::Vector2 center = winSize / 2.0f;

	//�������̍��W��ł���������ł��ꂻ��

#pragma region �I�v�V����
	object["option_bgm"] = Set(GE::Math::Vector3(center.x, center.y, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["option_se"] = Set(GE::Math::Vector3(center.x, center.y + 100, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["option_back"] = Set(GE::Math::Vector3(center.x, center.y + 200, 0.0f), { 150,100,0 }, GE::Color::White(), "texture_back");
	object["option_right"] = Set(GE::Math::Vector3(center.x + 200, center.y + 200, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_next");
	object["option_right"].transform.rotation = GE::Math::Quaternion::Euler({ 0, 0, 180 });
	object["option_left"] = Set(GE::Math::Vector3(center.x - 200, center.y + 200, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_next");
#pragma endregion

#pragma region �^�C�g��
	object["title_start"] = Set(GE::Math::Vector3(winSize.x - 300, center.y, 0.0f), { 300,100,0 }, GE::Color::White(), "texture_start");
	object["title_option"] = Set(GE::Math::Vector3(winSize.x - 300, center.y + 100, 0.0f), { 300,100,0 }, GE::Color::White(), "texture_option");
	object["title_exit"] = Set(GE::Math::Vector3(winSize.x - 300, center.y + 200, 0.0f), { 300,100,0 }, GE::Color::White(), "texture_exit");
	object["title_name"] = Set(GE::Math::Vector3(1500, winSize.y / 2 - 220.0f, 0.0f), { 1319 / 2.0f,642 / 2.0f,0 }, GE::Color::White(), "texture_title");
#pragma endregion

#pragma region �^�C�����~�b�g
	object["time_minutes"] = Set(GE::Math::Vector3(80, 850, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["time_symbol"] = Set(GE::Math::Vector3(160, 850, 0.0f), { 50,100,0 }, GE::Color::White(), "texture_symbol", { 64,64 }, { 32,64 });
	object["time_tenSeconds"] = Set(GE::Math::Vector3(240, 850, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["time_oneSeconds"] = Set(GE::Math::Vector3(340, 850, 0.0f), { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
#pragma endregion
	//Crash���̐���
	object["crash"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), { 400,100,0 }, GE::Color::White(), "crash_info_tex");
	//�W���C�R���U���Ă�A�j���[�V����
	object["gyro_shake"] = Set(object["crash"].transform.position - GE::Math::Vector3(300, 0, 0), { 150,150,0 }, GE::Color::White(), "gyro_shake_tex", { 896,128 }, { 128,128 });
}
void ScreenUIManager::Update(float deltaTime)
{
	object["crash"].isDraw = false;
	object["gyro_shake"].isDraw = false;
	//�^�C�����~�b�g
	object["time_minutes"].isDraw = true;
	object["time_minutes"].pivotPos = TimeLimit::GetInstance()->GetMinutes();
	object["time_tenSeconds"].isDraw = true;
	object["time_tenSeconds"].pivotPos = TimeLimit::GetInstance()->GetTenSeconds();
	object["time_oneSeconds"].isDraw = true;
	object["time_oneSeconds"].pivotPos = TimeLimit::GetInstance()->GetOneSeconds();
	object["time_symbol"].isDraw = true;

	switch (PlayerComponent::statas)
	{
	case PlayerComponent::PlayerStatas::CRASH:
		object["crash"].isDraw = true;
		object["gyro_shake"].isDraw = true;
		break;
	case PlayerComponent::PlayerStatas::TITLE:

		//�^�C�����~�b�g
		object["time_minutes"].isDraw = false;
		object["time_symbol"].isDraw = false;
		object["time_tenSeconds"].isDraw = false;
		object["time_oneSeconds"].isDraw = false;
		break;
	}

	const float f = 144.0f / (1.0f / deltaTime);

	a += 0.05f * f;
	int b = GE::Math::Lerp(0, 30, sin(a));
	object["gyro_shake"].pivotPos = abs(b / 7);

#pragma region �I�v�V������
	if (Title::GetInstance()->GetSelect(Title::States::option))
	{
		object["option_bgm"].isDraw = true;
		object["option_bgm"].pivotPos.x = OptionData::BGM_vol;
		object["option_bgm"].color = GE::Color::White();
		if (Option::select == Option::Select::BGM_VOL) { object["option_bgm"].color = GE::Color::Red(); }
		object["option_se"].isDraw = true;
		object["option_se"].pivotPos.x = OptionData::SE_vol;
		object["option_se"].color = GE::Color::White();
		if (Option::select == Option::Select::SE_VOL) { object["option_se"].color = GE::Color::Red(); }
		object["option_back"].isDraw = true;
		if (Option::select == Option::Select::Back)
		{
			object["option_back"].color = GE::Color::Red();
			object["option_right"].isDraw = false;
			object["option_left"].isDraw = false;
		}
		else
		{
			object["option_back"].color = GE::Color::White();
			object["option_right"].isDraw = true;
			object["option_right"].transform.position.y = (GE::Window::GetWindowSize().y / 2) + (int)Option::select * 100;
			object["option_left"].isDraw = true;
			object["option_left"].transform.position.y = (GE::Window::GetWindowSize().y / 2) + (int)Option::select * 100;
		}
	}
	else
	{
		object["option_bgm"].isDraw = false;
		object["option_se"].isDraw = false;
		object["option_back"].isDraw = false;
		object["option_right"].isDraw = false;
		object["option_left"].isDraw = false;
	}
#pragma endregion

#pragma region �^�C�g��
	if (!Title::GetInstance()->GetDecid())
	{
		object["title_start"].isDraw = true;
		object["title_start"].color = GE::Color::White();
		object["title_option"].isDraw = true;
		object["title_option"].color = GE::Color::White();
		object["title_exit"].isDraw = true;
		object["title_exit"].color = GE::Color::White();
		object["title_name"].isDraw = true;

		switch (Title::GetInstance()->states)
		{
		case Title::States::start:
			object["title_start"].color = GE::Color::Red();
			break;
		case Title::States::option:
			object["title_option"].color = GE::Color::Red();
			break;
		case Title::States::exit:
			object["title_exit"].color = GE::Color::Red();
			break;
		default:
			break;
		}
	}
	else
	{
		object["title_start"].isDraw = false;
		object["title_option"].isDraw = false;
		object["title_exit"].isDraw = false;
		object["title_name"].isDraw = false;
	}
#pragma endregion

}

void ScreenUIManager::DrawSprite(GE::IGraphicsDeviceDx12* graphicsDevice)
{
	for (auto o : object)
	{
		if (o.second.isDraw)
		{
			GE::ICBufferAllocater* cbufferAllocater = graphicsDevice->GetCBufferAllocater();
			GE::RenderQueue* renderQueue = graphicsDevice->GetRenderQueue();

			// �����_�[�L���[��2d�p�ɐ؂�ւ���
			graphicsDevice->SetCurrentRenderQueue(false);

			// �e�N�X�`���`��p�̃V�F�[�_�[���Z�b�g
			graphicsDevice->SetShader("DefaultSpriteWithTextureShader");

			GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ 0 });
			modelMatrix = o.second.transform.GetMatrix();

			// �摜�̐F�ς����肷��悤
			GE::Material material;
			material.color = o.second.color;

			// 2d�p�̃J������� ��{�I�ɔ����Ȃ���ok
			GE::CameraInfo cameraInfo;
			cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
			cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

			// �A�j���[�V�����̏��
			GE::TextureAnimationInfo textureAnimationInfo;

			// �摜�̌��T�C�Y
			textureAnimationInfo.textureSize = o.second.texSize;
			// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
			// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
			textureAnimationInfo.clipSize = o.second.clipSize;
			// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
			textureAnimationInfo.pivot = o.second.pivotPos;

			renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
			renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
			renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
			renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
			renderQueue->AddSetShaderResource({ 5,graphicsDevice->GetTextureManager()->Get(o.second.textureName)->GetSRVNumber() });
			graphicsDevice->DrawMesh("2DPlane");
		}
	}
}


void ScreenUIManager::LoadPosition(const std::string& filename)
{
	std::vector<LoadObj> ft;

	std::ifstream file;
	//�t�@�C�����J��
	file.open(filename);
	//�t�@�C�����J���Ȃ�������Ƃ܂�
	if (file.fail()) {
		assert(0);
	}

	// 1�s���ǂݍ���
	std::string line;
	while (getline(file, line)) {

		// 1�s���̕�����
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		

	}
	file.close();
}

void ScreenUIManager::SaveCurrentPosition(const std::string& filename)
{
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	//���e������
	writing_file.clear();
	if (!writing_file.is_open()) { assert(0); }

	writing_file.close();
}

void ScreenUIManager::UnLoad()
{
	object.clear();
}

