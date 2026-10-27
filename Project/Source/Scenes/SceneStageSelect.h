#pragma once
#include "SceneBase.h"
#include "System/Button.h"
#include <vector>
#include "System/Background.h"

class SceneStageSelect :
    public SceneBase
{
public:
    SceneStageSelect(SceneManager& sceneManager);
    ~SceneStageSelect() override;

    void Update() override;
	void Draw() override;
private:
	std::vector<Button> m_buttons;
    Button m_backButton;
    Background m_bg;
    int m_medalHandle = -1;
};