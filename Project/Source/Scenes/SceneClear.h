#pragma once
#include "SceneBase.h"
#include "System/Button.h"

class SceneClear :
    public SceneBase
{
public:
    SceneClear(SceneManager& sceneManager, int nextStage);
    ~SceneClear() override = default;

    void Update() override;
	void Draw() override;

private:
    void Back();
	void GoNextStage(bool isAllClear = false);
    void Restart();

private:
	Button m_nextStageButton;
    Button m_backButton;
    Button m_restartButton;
	int m_nextStage = -1;
};

