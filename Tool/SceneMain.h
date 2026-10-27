#pragma once
#include <vector>
#include "Rect.h"
#include "Vector2.h"

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void Update();
	void Draw();

	enum class Object
	{
		Start,
		Clear,
		Obstacles,
		Delete,

		Num
	};

	void WriteBinary();
	void LoadBinary(char* path);

	void WriteCSV();
	void LoadCSV(char* path);

private:
	std::vector<Rect> m_obstacles;
	Vector2 m_startPos;
	Vector2 m_clearPos;

	Object m_selectObj = Object::Start;

	int m_nowMouseInput = 0;
	int m_prevMouseInput = 0;

	Rect newRect;

	char m_nowKeyState[256] = {};
	char m_prevKeyState[256] = {};

	float m_goalLength = 1000.0f;
};

