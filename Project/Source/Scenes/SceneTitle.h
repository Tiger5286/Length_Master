#pragma once
#include "SceneBase.h"
#include "System/Button.h"
#include "System/Background.h"

class SceneTitle :
    public SceneBase
{
public:
    SceneTitle(SceneManager& sceneManager);
    ~SceneTitle() override;

    void Update() override;
	void Draw() override;

private:
	Button m_startButton;
    Button m_endButton;
    int m_titleHandle = -1;
    Background m_bg;
};

