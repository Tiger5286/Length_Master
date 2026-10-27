#include "Fade.h"
#include "DxLib.h"
#include "Game.h"

void Fade::Update()
{
	switch (m_state)
	{
	case State::FadeIn:
		m_fadeFrame++;
		if (m_fadeFrame > kFadeFrame)
		{
			m_fadeFrame = kFadeFrame;
			m_state = State::None;
		}
		break;
	case State::FadeOut:
		m_fadeFrame--;
		if (m_fadeFrame < 0)
		{
			m_fadeFrame = 0;
			m_state = State::None;
		}
		break;
	}
}

void Fade::Draw()
{
	float rate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeFrame);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate * 255);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}