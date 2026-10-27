#include "AllClearScene.h"
#include <array>
#include "Game.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "System/SoundManager.h"

namespace
{
	constexpr int kButtonW = 200;
	constexpr int kButtonH = 50;
	constexpr int kButtonMargin = 30;
}

AllClearScene::AllClearScene(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	m_titleHandle[0] = LoadGraph(L"data/graphs/congratulations.png");
	m_titleHandle[1] = LoadGraph(L"data/graphs/allStageClear.png");
	
	Rect rect(kButtonMargin, Game::kScreenHeight - kButtonH - kButtonMargin, kButtonW, kButtonH);
	m_backButton.Init([this]() {
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager));
		SoundManager::GetInstance().StopSound(L"allClearBGM", true);
		}, L"Back", rect);

	SoundManager::GetInstance().PlaySoundGame(L"allClearBGM", true, true);
}

AllClearScene::~AllClearScene()
{
	for (int i = 0; i < std::size(m_titleHandle); i++)
	{
		DeleteGraph(m_titleHandle[i]);
	}
}

void AllClearScene::Update()
{
	m_bg.Update();
	m_backButton.Update();
}

void AllClearScene::Draw()
{
	m_bg.Draw();

	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 - 150, 0.5f, 0.0f, m_titleHandle[0], true);
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 + 20, 0.5f, 0.0f, m_titleHandle[1], true);

	m_backButton.Draw();
}
