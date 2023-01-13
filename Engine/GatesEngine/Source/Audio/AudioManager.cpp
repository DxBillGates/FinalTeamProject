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
	// そもそもそのタグでオーディオが作られてないからnullptrを返す
	if (audios.find(tag) == audios.end())return nullptr;

	int audioCount = (int)audios[tag].size();

	if (audioCount == 1)return audios[tag][0];

	// そのタグで複数作られているなら再生されていない音を返す
	for (auto& audio : audios[tag])
	{
		if (audio->GetIsPlay() == true)continue;

		// 再生されてない音を取得
		return audio;
	}

	return audios[tag][0];
}

GE::Audio* GE::AudioManager::Get(const std::string& tag, int index)
{
	// そもそもそのタグでオーディオが作られてないからnullptrを返す
	if (audios.find(tag) == audios.end())return nullptr;

	int audioCount = (int)audios[tag].size();

	// 使おうとしている要素番号が用意されている量より多い場合は0番を返す
	if (index >= audioCount)return audios[tag][0];

	return audios[tag][index];
}
