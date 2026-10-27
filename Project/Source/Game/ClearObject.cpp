#include "ClearObject.h"
#include "DxLib.h"
#include "Utility/Vector2.h"
#include "GlobalFuncs.h"
#include "Rope.h"
#include "System/SoundManager.h"

namespace
{
	constexpr float kRadius = 10;
}

ClearObject::ClearObject(float clearLength, const Vector2& pos)
{
	m_clearLength = clearLength;
	m_pos = pos;
}

void ClearObject::Update(Rope& rope)
{
	int mouseX, m_mouseY;
	GetMousePoint(&mouseX, &m_mouseY);
	Vector2 mousePos(mouseX, m_mouseY);

	bool isInCircle = GlobalFuncs::IsPointInCircle(mousePos, m_pos, kRadius);

	if (rope.GetLength() <= m_clearLength && rope.IsClicked())
	{
		if (isInCircle)
		{
			m_isClear = true;
		}
	}
	else
	{
		if (isInCircle && !m_isPrevInCircle)
		{
			SoundManager::GetInstance().PlaySoundGame(L"beep");
		}
	}

	m_isPrevInCircle = isInCircle;
}

void ClearObject::Draw()
{
	DrawCircle(m_pos.x, m_pos.y, kRadius, 0xffff00, true);
}