#include "SceneManager.h"
#include "SceneBase.h"
#include "DxLib.h"

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> pNewScene)
{
	// シーンが空なら新しいシーンを追加する
	if (m_pScenes.empty())
	{
		m_pScenes.push_back(pNewScene);
	}
	else	// シーンが空でないなら最後のシーンを新しいシーンにする
	{
		m_pScenes.back() = pNewScene;
	}
}

void SceneManager::ChangeSceneWithFade(std::shared_ptr<SceneBase> pNewScene)
{
	m_pNextScene = pNewScene;
	m_nextChangeScene = NextChangeScene::Change;
	m_fade.StartFadeIn();
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> pNewScene)
{
	m_pScenes.push_back(pNewScene);
}

void SceneManager::PopScene()
{
	if (m_pScenes.size() > 1)
	{
		m_pScenes.pop_back();
	}
}

void SceneManager::PopSceneWithFade()
{
	m_nextChangeScene = NextChangeScene::Pop;
	m_fade.StartFadeIn();
}

void SceneManager::ResetScene(std::shared_ptr<SceneBase> pNewScene)
{
	m_pScenes.clear();
	m_pScenes.push_back(pNewScene);
}

void SceneManager::ResetSceneWithFade(std::shared_ptr<SceneBase> pNewScene)
{
	m_pNextScene = pNewScene;
	m_nextChangeScene = NextChangeScene::Reset;
	m_fade.StartFadeIn();
}

void SceneManager::Update()
{
	m_pScenes.back()->Update();

	m_fade.Update();

	if (m_nextChangeScene != NextChangeScene::None && m_fade.GetState() == Fade::State::None)
	{
		switch (m_nextChangeScene)
		{
		case NextChangeScene::Change:
			ChangeScene(m_pNextScene);
			break;
		case NextChangeScene::Reset:
			ResetScene(m_pNextScene);
			break;
		case NextChangeScene::Pop:
			PopScene();
			break;
		}

		m_pNextScene = nullptr;
		m_nextChangeScene = NextChangeScene::None;

		m_fade.StartFadeOut();
	}
}

void SceneManager::Draw()
{
	for (auto& scene : m_pScenes)
	{
		scene->Draw();
	}
	m_fade.Draw();
}

void SceneManager::RequestExit()
{
	m_isExit = true;
}
