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
	GE::Math::Vector2 currentWindowSizeDiff = GE::Window::GetDiffWindowSize();
	SpriteInfo result;
	result.transform.position = { pos.x * currentWindowSizeDiff.x,pos.y * currentWindowSizeDiff.y,0 };
	result.transform.scale = { scale.x * currentWindowSizeDiff.x,scale.y * currentWindowSizeDiff.y,0 };
	result.color = color;
	result.textureName = textureName;
	return result;
}

ScreenUIManager::SpriteInfo ScreenUIManager::Set(GE::Math::Vector3 pos, GE::Math::Vector3 scale, GE::Color color, std::string textureName, GE::Math::Vector2 texSize, GE::Math::Vector2 clipSize)
{
	GE::Math::Vector2 currentWindowSizeDiff = GE::Window::GetDiffWindowSize();
	SpriteInfo result;
	result.transform.position = { pos.x * currentWindowSizeDiff.x,pos.y * currentWindowSizeDiff.y,0 };
	result.transform.scale = { scale.x * currentWindowSizeDiff.x,scale.y * currentWindowSizeDiff.y,0 };
	result.color = color;
	result.textureName = textureName;
	result.texSize = texSize;
	result.clipSize = clipSize;
	return result;
}
void ScreenUIManager::TitleMenuActive(bool isActive)
{
	object["title_start"].isDraw = isActive;
	object["title_option"].isDraw = isActive;
	object["title_exit"].isDraw = isActive;
}

void ScreenUIManager::OptionMenuActive(bool isActive)
{
	object["bgm_num"].isDraw = isActive;
	object["se_num"].isDraw = isActive;
	object["bgm_info"].isDraw = isActive;
	object["se_info"].isDraw = isActive;
	object["option_back"].isDraw = isActive;
	object["option_right"].isDraw = isActive;
	object["option_left"].isDraw = isActive;
}
float ScreenUIManager::SetLerp(std::string name, float lerpTime, float deltaTime)
{
	if (object[name].lerpCount < lerpTime)
	{
		object[name].lerpCount += deltaTime;
		return object[name].lerpCount / lerpTime;
	}
	else
	{
		return 1.0f;
	}
}
void ScreenUIManager::NormalModeStart()
{
	winSize = GE::Window::GetWindowSize();
	center = winSize / 2.0f;
	a = 0.0f;

	//�������̍��W��ł���������ł��ꂻ��
#pragma region �I�v�V����
	object["bgm_num"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["se_num"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });
	object["bgm_info"] = Set({}, { 200, 100, 0 }, GE::Color::White(), "bgm_info_tex");
	object["se_info"] = Set({}, { 100,100,0 }, GE::Color::White(), "se_info_tex");
	object["option_back"] = Set({}, { 150, 100, 0 }, GE::Color::White(), "texture_back");
	object["option_right"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_next");
	object["option_right"].transform.rotation = GE::Math::Quaternion::Euler({ 0, 0, 180 });
	object["option_left"] = Set({}, { 100,100,0 }, GE::Color::White(), "texture_next");
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

	object["crash"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), { 400,100,0 }, GE::Color::White(), "crash_info_tex");
	object["lockon_info"] = Set(GE::Math::Vector3(217, 600, 0.0f), GE::Math::Vector3(600, 64, 0) * 0.6f, GE::Color::White(), "lockon_info_tex");
	object["dash_info"] = Set(GE::Math::Vector3(168, 670, 0.0f), GE::Math::Vector3(256, 64, 0) * 1.1f, GE::Color::White(), "control_info_1_tex", { 512,384 }, { 512, 128 });
	object["dash_info"].pivotPos = 2;
	object["go_tree"] = Set(GE::Math::Vector3(center.x, center.y - 150.f, 0.f), GE::Math::Vector3(256, 64, 0) * 0.6f, GE::Color::White(), "control_info_2_tex");
	object["push_b"] = Set(GE::Math::Vector3(winSize.x - 300.f, winSize.y - 100.f, 0.f), GE::Math::Vector3(256, 64, 0) * 0.6f, GE::Color::White(), "push_b_tex");
	object["push_b"].isDraw = true;
	//�J�ڂ̒l������
	for (auto o : object)
	{
		o.second.lerpCount = 0.0f;
	}
}

void ScreenUIManager::DashModeStart()
{
	object["combo"] = Set(GE::Math::Vector3(1500, winSize.y / 2 - 220.0f, 0.0f), { 256,128,0 }, GE::Color::White(), "combo_tex");
	object["comboNum"] = Set(GE::Math::Vector3(1300, winSize.y / 2 - 220.0f, 0.0f), { 128,128,0 }, GE::Color::White(), "texture_Number", { 320,64 }, { 32,64 });

	//�J�ڂ̒l������
	for (auto o : object)
	{
		o.second.lerpCount = 0.0f;
	}
}
void ScreenUIManager::NormalModeUpdate(float deltaTime)
{
	const float f = 144.0f / (1.0f / deltaTime);
	const float addCount = 0.1f * f;
	//Title
	TitleMenuActive(false);

	object["crash"].isDraw = false;
	object["gyro_shake"].isDraw = false;
	object["control_info_2"].isDraw = true;
	object["lockon_info"].isDraw = false;
	object["dash_info"].isDraw = false;
	object["go_tree"].isDraw = false;

	//�^�C�����~�b�g
	object["time_minutes"].isDraw = true;
	object["time_minutes"].pivotPos = TimeLimit::GetInstance()->GetMinutes();
	object["time_tenSeconds"].isDraw = true;
	object["time_tenSeconds"].pivotPos = TimeLimit::GetInstance()->GetTenSeconds();
	object["time_oneSeconds"].isDraw = true;
	object["time_oneSeconds"].pivotPos = TimeLimit::GetInstance()->GetOneSeconds();
	object["time_symbol"].isDraw = true;
	if (PlayerComponent::isGoTree)
	{
		object["go_tree"].isDraw = true;
	}
	switch (PlayerComponent::statas)
	{
	case PlayerComponent::PlayerStatas::CRASH:
		object["crash"].isDraw = true;
		break;
	case PlayerComponent::PlayerStatas::TITLE:
		object["title_name"].isDraw = true;

		//�^�C�����~�b�g�`�悵�Ȃ�
		object["time_minutes"].isDraw = false;
		object["time_symbol"].isDraw = false;
		object["time_tenSeconds"].isDraw = false;
		object["time_oneSeconds"].isDraw = false;
		break;
	case PlayerComponent::PlayerStatas::TITLE_MENU:
		//�^�C�����~�b�g�`�悵�Ȃ�
		object["time_minutes"].isDraw = false;
		object["time_symbol"].isDraw = false;
		object["time_tenSeconds"].isDraw = false;
		object["time_oneSeconds"].isDraw = false;
		//�^�C�g���`��
		TitleMenuActive(true);
		//������t�F�[�h�C��
		object["title_start"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y, 0.0f), SetLerp("title_start", 5.0f, addCount));
		object["title_option"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 100, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 100, 0.0f), SetLerp("title_option", 6.0f, addCount));
		object["title_exit"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 200, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 200, 0.0f), SetLerp("title_exit", 7.0f, addCount));
		break;
	case PlayerComponent::PlayerStatas::STAY_TREE:
		object["title_name"].isDraw = false;
		object["push_b"].isDraw = false;

		break;
	case PlayerComponent::PlayerStatas::LOCKON_SHOOT:

		break;
	case PlayerComponent::PlayerStatas::MOVE:

		object["lockon_info"].isDraw = true;
		object["dash_info"].isDraw = true;

		break;
	}
#pragma region �I�v�V������
	OptionMenuActive(false);
	if (Title::GetInstance()->GetSelect(Title::States::option))
	{
		TitleMenuActive(false);
		OptionMenuActive(true);
		object["option_right"].transform.position.x = GE::Math::Lerp(winSize.x + 1000, winSize.x - 200, SetLerp("bgm_num", 5.0f, addCount));
		object["option_left"].transform.position.x = GE::Math::Lerp(winSize.x + 1000, winSize.x - 450, SetLerp("bgm_num", 5.0f, addCount));

		object["bgm_num"].pivotPos.x = OptionData::BGM_vol;
		object["bgm_num"].color = GE::Color::White();
		object["bgm_num"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y, 0.0f), SetLerp("bgm_num", 5.0f, addCount));
		object["bgm_info"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 700, center.y, 0.0f), GE::Math::Vector3(winSize.x - 600, center.y, 0.0f), SetLerp("bgm_num", 5.0f, addCount));

		if (Option::select == Option::Select::BGM_VOL) { object["bgm_num"].color = GE::Color::Red(); }
		object["se_num"].pivotPos.x = OptionData::SE_vol;
		object["se_num"].color = GE::Color::White();
		object["se_num"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 100, 0.0f), GE::Math::Vector3(winSize.x - 305, center.y + 100, 0.0f), SetLerp("se_num", 6.0f, addCount));
		object["se_info"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 700, center.y + 100, 0.0f), GE::Math::Vector3(winSize.x - 605, center.y + 100, 0.0f), SetLerp("se_num", 6.0f, addCount));

		if (Option::select == Option::Select::SE_VOL) { object["se_num"].color = GE::Color::Red(); }
		object["option_back"].transform.position = GE::Math::Vector3::Lerp(GE::Math::Vector3(winSize.x + 1000, center.y + 200, 0.0f), GE::Math::Vector3(winSize.x - 300, center.y + 200, 0.0f), SetLerp("option_back", 7.0f, addCount));
		if (Option::select == Option::Select::Back)
		{
			object["option_back"].color = GE::Color::Red();
			object["option_right"].isDraw = false;
			object["option_left"].isDraw = false;
			//�^�C�g�����j���[�̑J�ڏ�����
			object["title_start"].lerpCount = 0.f;
			object["title_option"].lerpCount = 0.f;
			object["title_exit"].lerpCount = 0.f;
			object["title_name"].lerpCount = 0.f;

		}
		else
		{
			object["option_back"].color = GE::Color::White();
			object["option_right"].transform.position.y = (GE::Window::GetWindowSize().y / 2) + (int)Option::select * 100;
			object["option_left"].transform.position.y = (GE::Window::GetWindowSize().y / 2) + (int)Option::select * 100;
		}
	}
	else
	{
		//�I�v�V�������j���[�̑J�ڏ�����
		object["bgm_num"].lerpCount = 0.f;
		object["se_num"].lerpCount = 0.f;
		object["option_back"].lerpCount = 0.f;
		object["option_right"].lerpCount = 0.f;
		object["option_left"].lerpCount = 0.f;
	}
#pragma endregion

#pragma region �^�C�g��
	if (!Title::GetInstance()->GetDecid())
	{
		object["title_start"].color = GE::Color::White();
		object["title_option"].color = GE::Color::White();
		object["title_exit"].color = GE::Color::White();

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
#pragma endregion

}
void ScreenUIManager::DashModeUpdate(float deltaTime)
{
	const float f = 144.0f / (1.0f / deltaTime);

	GE::Math::Vector3 vive = Vivlate(f);
	GE::Math::Vector3 comboPos = GE::Math::Vector3(1500, winSize.y / 2 - 220.0f, 0.0f);
	GE::Math::Vector3 comboNumPos = GE::Math::Vector3(1300, winSize.y / 2 - 220.0f, 0.0f);
	if (PlayerComponent::combo != 0)
	{
		object["combo"].isDraw = true;
		object["comboNum"].isDraw = true;
		object["combo"].transform.position = comboPos + vive;
		object["comboNum"].transform.position = comboNumPos + vive;
		object["comboNum"].pivotPos = PlayerComponent::combo;


	}
	else
	{
		object["combo"].isDraw = false;
		object["comboNum"].isDraw = false;
	}
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

GE::Math::Vector3 ScreenUIManager::Vivlate(float deltaTime)
{
	//�J�����V�F�C�N�̌���
	viveVelocity.x > 0 ? viveVelocity.x -= 1.0f * (int)deltaTime : viveVelocity.x = 0.0f;
	viveVelocity.y > 0 ? viveVelocity.y -= 1.0 * (int)deltaTime : viveVelocity.y = 0.0f;

	//�����_���ňړ���
	GE::Math::Vector2 randVel = GE::Math::Vector2(viveVelocity.x > 0 ? rand() % (int)viveVelocity.x - viveVelocity.x / 2 : 0, viveVelocity.y > 0 ? rand() % (int)viveVelocity.y - viveVelocity.y / 2 : 0);
	//�ړ��ʂ��Z�b�g����
	return GE::Math::Vector3(randVel.x, randVel.y, 0);
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

