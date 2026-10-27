#pragma once
class SaveDataManager
{
public:
	static SaveDataManager& GetInstance();
	~SaveDataManager();

	struct SaveData
	{
		int clearFrags = 0;
	};

	void RecordClearStage(int clearStage);

	void Load();
	void Save();

	SaveData GetData() const { return m_data; }

private:
	SaveDataManager();
	SaveDataManager(const SaveDataManager&) = delete;
	void operator=(const SaveDataManager&) = delete;

private:

	SaveData m_data;
};