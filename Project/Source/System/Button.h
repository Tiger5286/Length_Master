#pragma once
#include "Utility/Rect.h"
#include <functional>
#include <string>
#include "DxLib.h"
#include "GlobalFuncs.h"

class Button
{
public:
	enum class SoundType
	{
		Decision,
		Cancel
	};

public:
	Button() = default;
	~Button() = default;

	void Init(std::function<void()> func, const std::wstring& text, Rect rect,SoundType soundType = SoundType::Decision);
	void Update();
	void Draw();

	Rect GetRect() const { return m_rect; }

private:
	Rect m_rect;
	std::function<void()> m_func;
	std::wstring m_text;

	int m_offset = 0;
	unsigned int m_color = 0x555555;
	unsigned int m_frameColor = 0xffffff;

	bool m_isPrevInRect = false;
	int m_prevMouseInput = 0;

	SoundType m_soundType = SoundType::Decision;
};