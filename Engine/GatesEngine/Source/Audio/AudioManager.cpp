#include "..\..\Header\Audio\AudioManager.h"

GE::AudioManager::AudioManager()
{
	HRESULT hr;
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	Audio::SetAudioEngine();
}

GE::AudioManager::~AudioManager()
{
	for (auto& tagAudios : audios)
	{
		for (auto& audio : tagAudios.second)
		{
			delete audio;
		}
	}

	for (auto& audioData : backupAudioDatas)
	{
		delete audioData.second;
	}
	audios.clear();
	backupAudioDatas.clear();
}

GE::Audio* GE::AudioManager::AddAudio(Audio * newAudio)
{
	audios[newAudio->GetTag()].emplace_back(newAudio);
	return newAudio;
}

GE::AudioData* GE::AudioManager::AddAudioData(AudioData * newAudioData,const std::string& tag)
{
	backupAudioDatas.insert(std::make_pair(tag, newAudioData));
	return newAudioData;
}

GE::AudioData* GE::AudioManager::GetAudioData(std::string& tag)
{
	return backupAudioDatas.at(tag);
}

GE::Audio * GE::AudioManager::Use(const std::string& tag)
{
	// �����������̃^�O�ŃI�[�f�B�I������ĂȂ�����nullptr��Ԃ�
	if (audios.find(tag) == audios.end())return nullptr;

	int audioCount = (int)audios[tag].size();

	if (audioCount == 1)return audios[tag][0];

	// ���̃^�O�ŕ�������Ă���Ȃ�Đ�����Ă��Ȃ�����Ԃ�
	for (auto& audio : audios[tag])
	{
		if (audio->GetIsPlay() == true)continue;

		// �Đ�����ĂȂ������擾
		return audio;
	}

	return audios[tag][0];
}

GE::Audio* GE::AudioManager::Get(const std::string& tag, int index)
{
	// �����������̃^�O�ŃI�[�f�B�I������ĂȂ�����nullptr��Ԃ�
	if (audios.find(tag) == audios.end())return nullptr;

	int audioCount = (int)audios[tag].size();

	// �g�����Ƃ��Ă���v�f�ԍ����p�ӂ���Ă���ʂ�葽���ꍇ��0�Ԃ�Ԃ�
	if (index >= audioCount)return audios[tag][0];

	return audios[tag][index];
}
