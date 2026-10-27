#include "SceneClear.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "SceneMain.h"
#include "System/SoundManager.h"
#include "AllClearScene.h"
#include "System/SaveDataManager.h"

namespace
{
	constexpr int kButtonW = 280;
	constexpr int kButtonH = 50;
	constexpr int kButtonMargin = 30;
}

SceneClear::SceneClear(SceneManager& sceneManager,int nextStage) :
	SceneBase(sceneManager)
{
	m_nextStage = nextStage;

	Rect backToTitleRect(Game::kScreenWidth / 2 - kButtonW - kButtonMargin, Game::kScreenHeight / 2 + 100, kButtonW, kButtonH);
	m_backButton.Init([this]() {
		Back();
		SoundManager::GetInstance().StopSound(L"mainBGM", true);
		}, L"Back", backToTitleRect,Button::SoundType::Cancel);

	Rect restartRect(Game::kScreenWidth / 2 + kButtonMargin, Game::kScreenHeight / 2 + 100, kButtonW, kButtonH);
	m_restartButton.Init([this]() {
		Restart();
		}, L"Retry", restartRect);

	if (m_nextStage <= Game::kStageNum)
	{
		Rect nextStageRect(Game::kScreenWidth / 2 - 350 / 2, Game::kScreenHeight / 2, 350, 80);
		m_nextStageButton.Init([this]() {
			GoNextStage();
			}, L"Next Stage", nextStageRect);
	}
	else
	{
		m_nextStageButton.Init([]() {}, L"", Rect(-100, 0, 0, 0)); // –³Œø‰»
		int clearStage = SaveDataManager::GetInstance().GetData().clearFrags;
		if (clearStage == 0b1111111111)
		{
			Rect nextStageRect(Game::kScreenWidth / 2 - 350 / 2, Game::kScreenHeight / 2, 350, 80);
			m_nextStageButton.Init([this]() {
				GoNextStage(true);
				}, L"Next", nextStageRect);
		}
	}

	SoundManager::GetInstance().PlaySoundGame(L"clearJingle");
}

void SceneClear::Update()
{
	m_backButton.Update();
	m_nextStageButton.Update();
	m_restartButton.Update();
}

void SceneClear::Draw()
{
	// ”¼“§–¾‚Ì”wŒi‚ð•`‰æ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(Game::kScreenWidth / 6 * 1, Game::kScreenHeight / 6 * 1 - Game::kUISpaceHeight / 2,
		Game::kScreenWidth / 6 * 5, Game::kScreenHeight / 6 * 5 - Game::kUISpaceHeight / 2,
		0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ƒ^ƒCƒgƒ‹‚ð•`‰æ
	std::wstring title = L"Stage Clear!";
	int w = GetDrawStringWidthToHandle(title.c_str(), title.size(), GlobalValues::g_mainFontHandle);
	DrawStringToHandle(Game::kScreenWidth / 2 - w / 2,
		Game::kScreenHeight / 2 - 200, title.c_str(), 0xffffff, GlobalValues::g_mainFontHandle);

	// ƒ{ƒ^ƒ“‚ð•`‰æ
	m_backButton.Draw();
	m_nextStageButton.Draw();
	m_restartButton.Draw();
}

void SceneClear::Back()
{
	m_sceneManager.ResetSceneWithFade(std::make_shared<SceneStageSelect>(m_sceneManager));
}

void SceneClear::GoNextStage(bool isAllClear)
{
	SoundManager::GetInstance().StopSound(L"mainBGM", true);
	if (isAllClear)
	{
		m_sceneManager.ResetSceneWithFade(std::make_shared<AllClearScene>(m_sceneManager));
	}
	else
	{
		m_sceneManager.ResetSceneWithFade(std::make_shared<SceneMain>(m_sceneManager, m_nextStage));
	}
}

void SceneClear::Restart()
{
	m_sceneManager.ResetSceneWithFade(std::make_shared<SceneMain>(m_sceneManager, m_nextStage - 1));
}
