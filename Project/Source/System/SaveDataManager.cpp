#include "SaveDataManager.h"
#include "DxLib.h"
#include <fstream>

namespace
{
	constexpr const wchar_t* kFileName = L"data/saveData.dat";
}

SaveDataManager& SaveDataManager::GetInstance()
{
	static SaveDataManager instance;
	return instance;
}

void SaveDataManager::RecordClearStage(int clearStage)
{
	// クリアしたステージをビットで記録する
	m_data.clearFrags |= (1 << (clearStage - 1));
}

void SaveDataManager::Load()
{
	int handle = FileRead_open(kFileName);
	if (handle == -1) return;
	FileRead_read(&m_data, sizeof(m_data), handle);
	FileRead_close(handle);
}

void SaveDataManager::Save()
{
	std::ofstream file(kFileName, std::ios::binary);
	file.write(reinterpret_cast<const char*>(&m_data), sizeof(m_data));
	file.close();
}

SaveDataManager::SaveDataManager()
{
}

SaveDataManager::~SaveDataManager()
{
	Save();
}
