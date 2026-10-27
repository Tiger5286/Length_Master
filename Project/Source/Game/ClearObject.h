#pragma once
#include "Utility/Vector2.h"

class Rope;

class ClearObject
{
public:
	ClearObject(float clearLength, const Vector2& pos);
	~ClearObject() = default;

	void Update(Rope& rope);
	void Draw();

	bool IsClear() const { return m_isClear; }
	float GetClearLength() const { return m_clearLength; }
	Vector2 GetPos() const { return m_pos; }

private:
	Vector2 m_pos;
	float m_clearLength = 0.0f;
	bool m_isClear = false;
	bool m_isPrevInCircle = false;
};

