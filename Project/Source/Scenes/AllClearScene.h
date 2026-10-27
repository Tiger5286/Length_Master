#pragma once
#include "SceneBase.h"
#include "System/Button.h"
#include "System/Background.h"

class AllClearScene :
    public SceneBase
{
public:
    AllClearScene(SceneManager& sceneManager);
    ~AllClearScene() override;

    void Update() override;
    void Draw() override;

private:
    int m_titleHandle[2] = { -1,-1 };
    Button m_backButton;
    Background m_bg;
};

