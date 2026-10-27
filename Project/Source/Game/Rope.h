#pragma once
#include "Dxlib.h"
#include "Utility/Vector2.h"
#include <vector>
#include "System/Button.h"

class Rect;

class Rope
{
public:
	Rope(const Vector2& startPos);
	~Rope();

	void Update(std::vector<Rect>& obstacles,float maxLength);
	void Draw();

	float GetLength() const;
	Vector2 GetPos() const { return m_anchors.front(); }
	int GetAnchorNum() const;
	bool IsClicked() const { return m_isClicked; }

	void Reset();

private:
	void Stretch(std::vector<Rect>& obstacles);

	void DrawGlowLine(const Vector2& p1, const Vector2& p2, unsigned int color);

private:
	std::vector<Vector2> m_anchors;	// ロープが引っかかる位置のリスト(一つ目の要素はロープの始点)
	Vector2 m_prevMousePos;
	Vector2 m_mousePos;
	float m_maxLength = 0.0f;
	bool m_isClicked = false;

	int m_nowMouseInput = 0;
	int m_prevMouseInput = 0;

	int m_seFrame = 0;
};