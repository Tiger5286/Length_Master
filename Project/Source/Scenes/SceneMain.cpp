#include "SceneMain.h"
#include "DxLib.h"
#include "Game/Rope.h"
#include "Game/ClearObject.h"
#include "SceneManager.h"
#include "SceneClear.h"
#include "SceneStageSelect.h"
#include <cassert>	
#include "Game.h"
#include <fstream>
#include <sstream>
#include "System/SoundManager.h"
#include "System/SaveDataManager.h"

namespace
{
	const Vector2 kRopeStartPos = Vector2(400.0f, 300.0f);

	const std::vector<std::wstring> kStageFileNames = {
		L"data/stage/stage1.csv",
		L"data/stage/stage1.csv",
		L"data/stage/stage2.csv",
		L"data/stage/stage3.csv",
		L"data/stage/stage4.csv",
		L"data/stage/stage5.csv",
		L"data/stage/stage6.csv",
		L"data/stage/stage7.csv",
		L"data/stage/stage8.csv",
		L"data/stage/stage9.csv",
		L"data/stage/stage10.csv",
	};
}

SceneMain::SceneMain(SceneManager& sceneManager,int stage):
	SceneBase(sceneManager)
{
	m_currentStage = stage;
	LoadStage(stage);

	Rect buttonRect(20, Game::kScreenHeight - 70, 150, 50);
	m_backButton.Init([this]() {
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(m_sceneManager));
		SoundManager::GetInstance().StopSound(L"mainBGM", true);
		}, L"Back", buttonRect,Button::SoundType::Cancel);

	buttonRect = Rect(190, Game::kScreenHeight - 70, 150, 50);
	m_resetButton.Init([this]() {
		m_pRope->Reset();
		}, L"Reset", buttonRect);
}

SceneMain::~SceneMain()
{
}

void SceneMain::Update()
{
	m_bg.Update();
	m_pRope->Update(m_obstacles,m_pClearObj->GetClearLength());
	m_pClearObj->Update(*m_pRope);

	if (m_pClearObj->IsClear())
	{
		SaveDataManager::GetInstance().RecordClearStage(m_currentStage);
		m_sceneManager.PushScene(std::make_shared<SceneClear>(m_sceneManager, m_currentStage + 1));
		return;
	}

	m_backButton.Update();
	m_resetButton.Update();
}

void SceneMain::Draw()
{
	// ”wŒi‚ð•`‰æ
	m_bg.Draw();

	// áŠQ•¨‚ð•`‰æ
	for(auto& obstacle : m_obstacles)
	{
		obstacle.Draw(0x222222, true);
	}

	// ƒNƒŠƒAˆÊ’u‚Æƒ[ƒv‚ð•`‰æ
	m_pClearObj->Draw();
	m_pRope->Draw();

	// ƒ`ƒ…[ƒgƒŠƒAƒ‹ƒeƒLƒXƒg‚ð•`‰æ
	if (m_currentStage == 1)
	{
		if (m_pRope->GetAnchorNum() == 1)
		{
			DrawStringToHandle(m_startPos.x - 17, m_startPos.y - Game::kMainFontSize - 17, L"«Click here", 0xffffff, GlobalValues::g_mainFontHandle);
		}
		else
		{
			std::wstring str = L"Extend here«";
			int w = GetDrawStringWidthToHandle(str.c_str(), str.size(), GlobalValues::g_mainFontHandle);
			DrawStringToHandle(m_pClearObj->GetPos().x - w + 17, m_pClearObj->GetPos().y - Game::kMainFontSize - 17, str.c_str(), 0xffffff, GlobalValues::g_mainFontHandle);
		}
	}
	else if (m_currentStage == 2)
	{
		DrawStringToHandle(Game::kScreenWidth / 2 + 40,
			Game::kScreenHeight / 2 - Game::kMainFontSize / 2 - Game::kUISpaceHeight,
			L"©Don't touch this", 0xffffff, GlobalValues::g_mainFontHandle);
	}

	// ƒXƒe[ƒW–¼‚ð•`‰æ
	DrawFormatStringToHandle(30, 30, 0xffffff, GlobalValues::g_mainFontHandle, L"Stage %d", m_currentStage);

	// ‰º‚ÌUI—pƒXƒy[ƒX‚ð•`‰æ
	DrawBox(0, Game::kScreenHeight - Game::kUISpaceHeight, Game::kScreenWidth, Game::kScreenHeight, 0x222222, true);

	// ¶‰º‚Ìƒ{ƒ^ƒ“‚ð‘‚¢‚½
	m_backButton.Draw();
	m_resetButton.Draw();

	// Œ»Ý‚Ì’·‚³ / ’·‚³§ŒÀ@‚ÌUI‚ð•`‰æ
	float y = Game::kScreenHeight - Game::kUISpaceHeight / 2 - Game::kMainFontSize / 2;
	unsigned int color = 0xffffff;
	if (m_pRope->GetLength() > m_pClearObj->GetClearLength()) color = 0xff0000;
	std::wstring str = L"/";
	int w = GetDrawStringWidthToHandle(str.c_str(), str.size(), GlobalValues::g_mainFontHandle);
	DrawStringToHandle(Game::kScreenWidth / 2 - w / 2 - 130, y, str.c_str(), color, GlobalValues::g_mainFontHandle);
	str = L"%.1f";
	DrawFormatStringToHandle(Game::kScreenWidth / 2 + w / 2 + 10 - 130, y, color, GlobalValues::g_mainFontHandle, str.c_str(), m_pClearObj->GetClearLength());
	int w2 = GetDrawFormatStringWidthToHandle(GlobalValues::g_mainFontHandle, str.c_str(), m_pRope->GetLength());
	DrawFormatStringToHandle(Game::kScreenWidth / 2 - w / 2 - 10 - w2 - 130, y, color, GlobalValues::g_mainFontHandle, str.c_str(), m_pRope->GetLength());

	// ’·‚³ƒQ[ƒW‚ð•`‰æ
	float rate = m_pRope->GetLength() / m_pClearObj->GetClearLength();
	if (rate > 1.0f) rate = 1.0f;
	// ƒQ[ƒW‚Ì”wŒi
	DrawBox(Game::kScreenWidth / 2 + 50, Game::kScreenHeight - 70,
		Game::kScreenWidth / 2 + 50 + 250, Game::kScreenHeight - 20,
		0x444444, true);
	// ƒQ[ƒW–{‘Ì
	color = 0x00ffff;
	if (rate >= 1.0f) color = 0xff0000;
	DrawBox(Game::kScreenWidth / 2 + 50, Game::kScreenHeight - 70,
		Game::kScreenWidth / 2 + 50 + (250 * rate), Game::kScreenHeight - 20,
		color, true);
	// ˜g
	DrawBox(Game::kScreenWidth / 2 + 50, Game::kScreenHeight - 70,
		Game::kScreenWidth / 2 + 50 + 250, Game::kScreenHeight - 20,
		0xffffff, false, 5);

	// Žc‚è’·‚³‚ð•`‰æ
	str = L"remain %.1f";
	DrawFormatStringToHandle(Game::kScreenWidth / 2 + 50 + 250 + 20, y, color, GlobalValues::g_mainFontHandle, str.c_str(), m_pClearObj->GetClearLength() - m_pRope->GetLength());

#ifdef _DEBUG
	DrawString(0,0,L"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff,L"Stage: %d", m_currentStage);
#endif
}

void SceneMain::LoadStage(int stage)
{
	std::vector<float> datas;
	std::ifstream ifs(kStageFileNames[stage]);

	std::string line;
	if (std::getline(ifs, line))
	{
		std::stringstream ss(line);
		std::string cell;
		while (std::getline(ss, cell, ','))
		{
			if (!cell.empty())
			{
				datas.push_back(std::stof(cell));
			}
		}
	}

	Vector2 pos(datas[0], datas[1]);
	m_startPos = pos;
	m_pRope = std::make_shared<Rope>(pos);
	pos = { datas[2],datas[3] };
	float clearLength = datas[4];
	m_pClearObj = std::make_shared<ClearObject>(clearLength, pos);

	m_obstacles.clear();
	int rectNum = (datas.size() - 5) / 4;
	for (int i = 0; i < rectNum; i++)
	{
		Rect rect;
		rect.x = datas[(5 + i * 4) + 0];
		rect.y = datas[(5 + i * 4) + 1];
		rect.w = datas[(5 + i * 4) + 2];
		rect.h = datas[(5 + i * 4) + 3];
		m_obstacles.push_back(rect);
	}
}