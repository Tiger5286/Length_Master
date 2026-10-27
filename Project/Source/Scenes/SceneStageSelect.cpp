#include "SceneStageSelect.h"
#include "Game.h"
#include "SceneManager.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "System/SoundManager.h"
#include "System/SaveDataManager.h"

namespace
{
	const std::vector<std::wstring> kStageNames = {
		L"Stage 1",
		L"Stage 2",
		L"Stage 3",
		L"Stage 4",
		L"Stage 5",
		L"Stage 6",
		L"Stage 7",
		L"Stage 8",
		L"Stage 9",
		L"Stage 10",
	};

	constexpr int kButtonSize = 180;
	constexpr int kButtonMargin = 40;
}

SceneStageSelect::SceneStageSelect(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	for (int i = 0; i < Game::kStageNum; i++)
	{
		int x = Game::kScreenWidth / 2 - kButtonSize / 2 - (kButtonSize + kButtonMargin) * 2 + (kButtonSize + kButtonMargin) * i;
		int y = 100;
		if (i >= 5)
		{
			x -= Game::kScreenWidth / 2 - kButtonSize / 2 - (kButtonSize + kButtonMargin) * 2.5f + (kButtonSize + kButtonMargin) * 5;
			y += kButtonSize + kButtonMargin;
		}
		Rect buttonRect(x , y, kButtonSize, kButtonSize);
		Button button;
		button.Init([this, i]() {
			m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneMain>(m_sceneManager, i + 1));
			SoundManager::GetInstance().StopSound(L"stageSelectBGM", true);
			SoundManager::GetInstance().PlaySoundGame(L"mainBGM", true, true);
			}, kStageNames[i], buttonRect);
		m_buttons.push_back(button);
	}

	Rect buttonRect(90, Game::kScreenHeight - 90, 150, 50);
	m_backButton.Init([this]() {
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager));
		SoundManager::GetInstance().StopSound(L"stageSelectBGM", true);
		}, L"Back", buttonRect,Button::SoundType::Cancel);

	SoundManager::GetInstance().PlaySoundGame(L"stageSelectBGM", true, true);

	m_medalHandle = LoadGraph(L"data/graphs/medal.png");
}

SceneStageSelect::~SceneStageSelect()
{
	DeleteGraph(m_medalHandle);
}

void SceneStageSelect::Update()
{
	m_bg.Update();
	for (auto& button : m_buttons)
	{
		button.Update();
	}
	m_backButton.Update();
}

void SceneStageSelect::Draw()
{
	m_bg.Draw();
	int clearStage = SaveDataManager::GetInstance().GetData().clearFrags;
	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons[i].Draw();
		bool isClear = clearStage & 1 << (i);
		if (isClear)
		{
			Rect rect = m_buttons[i].GetRect();
			int w, h;
			GetGraphSize(m_medalHandle, &w, &h);
			int x = rect.x + rect.w - w / 2;
			int y = rect.y - h / 2 + 10;
			DrawGraph(x, y, m_medalHandle, true);
		}
	}
	m_backButton.Draw();

#ifdef _DEBUG
	DrawString(0, 0, L"SceneStageSelect", 0xffffff);
#endif
}