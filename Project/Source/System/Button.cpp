#include "Button.h"
#include "Game.h"
#include "System/SoundManager.h"

void Button::Init(std::function<void()> func, const std::wstring& text, Rect rect,SoundType soundType)
{
	m_func = func;
	m_text = text;
	m_rect = rect;
	m_soundType = soundType;

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	Vector2 mousePos(mouseX, mouseY);

	int mouseInput;
	mouseInput = GetMouseInput();

	m_isPrevInRect = GlobalFuncs::IsPointInRect(mousePos, m_rect);
	m_prevMouseInput = mouseInput;
}

void Button::Update()
{
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	Vector2 mousePos(mouseX, mouseY);

	int mouseInput;
	mouseInput = GetMouseInput();

	m_offset = 0;
	m_color = 0x333333;
	m_frameColor = 0xffffff;

	bool isInRect = GlobalFuncs::IsPointInRect(mousePos, m_rect);
	bool isPressLeftClick = mouseInput & MOUSE_INPUT_1;
	bool isTriggeredLeftClick = (mouseInput & MOUSE_INPUT_1) && !(m_prevMouseInput & MOUSE_INPUT_1);

	if (isInRect)
	{
		m_offset = 5;
		m_frameColor = 0x00ffff;
		if (isPressLeftClick)
		{
			m_color = 0x111111;
			m_offset = 0;
			if (isTriggeredLeftClick)
			{
				m_func();
				if (m_soundType == SoundType::Decision)
				{
					SoundManager::GetInstance().PlaySoundGame(L"decision");
				}
				else if (m_soundType == SoundType::Cancel)
				{
					SoundManager::GetInstance().PlaySoundGame(L"cancel");
				}
			}
		}
	}

	if (isInRect && !m_isPrevInRect)
	{
		SoundManager::GetInstance().PlaySoundGame(L"select");
	}

	m_prevMouseInput = mouseInput;
	m_isPrevInRect = isInRect;
}

void Button::Draw()
{
	DrawBox(m_rect.x - m_offset,
			m_rect.y - m_offset,
			m_rect.x + m_rect.w + m_offset,
			m_rect.y + m_rect.h + m_offset,
		m_color, true);

	DrawBox(m_rect.x - m_offset,
			m_rect.y - m_offset,
			m_rect.x + m_rect.w + m_offset,
			m_rect.y + m_rect.h + m_offset,
		m_frameColor, false, 5);

	int w = GetDrawStringWidthToHandle(m_text.c_str(), m_text.size(),GlobalValues::g_mainFontHandle);
	DrawStringToHandle(m_rect.x + m_rect.w / 2 - w / 2,
					   m_rect.y + m_rect.h / 2 - Game::kMainFontSize / 2,
					   m_text.c_str(), 0xffffff, GlobalValues::g_mainFontHandle);
}