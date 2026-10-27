#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "Game.h"
#include "System/SoundManager.h"

namespace
{
	const Vector2 kButtonWH = { 300,75 };
}

SceneTitle::SceneTitle(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	Rect buttonRect(Game::kScreenWidth / 2 - kButtonWH.x / 2, Game::kScreenHeight / 2 + 100, kButtonWH.x, kButtonWH.y);
	m_startButton.Init([this]() { 
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(m_sceneManager));
		SoundManager::GetInstance().StopSound(L"titleBGM", true);
		}, L"Start", buttonRect);

	buttonRect.y += kButtonWH.y + 30;
	m_endButton.Init([this]() {
		m_sceneManager.RequestExit();
		SoundManager::GetInstance().StopSound(L"titleBGM", true);
		}, L"End", buttonRect, Button::SoundType::Cancel);

	m_titleHandle = LoadGraph(L"data/graphs/title.png");

	SoundManager::GetInstance().PlaySoundGame(L"titleBGM", true, true);
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(m_titleHandle);
}

void SceneTitle::Update()
{
	m_bg.Update();
	m_startButton.Update();
	m_endButton.Update();
}

void SceneTitle::Draw()
{
	m_bg.Draw();
	m_startButton.Draw();
	m_endButton.Draw();

	int w, h;
	GetGraphSize(m_titleHandle, &w, &h);

	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 - 150, 0.5f, 0.0f, m_titleHandle, true);

#ifdef _DEBUG
	DrawString(0,0,L"SceneTitle",0xffffff);
#endif
}
