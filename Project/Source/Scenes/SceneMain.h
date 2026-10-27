#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>
#include "Utility/Rect.h"
#include "System/Button.h"
#include "System/Background.h"

class Rope;
class ClearObject;

class SceneMain : public SceneBase
{
public:
	SceneMain(SceneManager& sceneManager,int stage);
	~SceneMain();

	void Update();
	void Draw();

private:
	void LoadStage(int stage);
	
private:
	std::shared_ptr<Rope> m_pRope = nullptr;
	std::shared_ptr<ClearObject> m_pClearObj = nullptr;
	std::vector<Rect> m_obstacles;
	int m_currentStage = 0;
	Button m_backButton;
	Button m_resetButton;
	Vector2 m_startPos;
	Background m_bg;
};

