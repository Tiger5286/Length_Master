#pragma once

class SceneManager;

class SceneBase
{
public:
	SceneBase(SceneManager& sceneManager);
	virtual ~SceneBase() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	SceneManager& m_sceneManager;
};

