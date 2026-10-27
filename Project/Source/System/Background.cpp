#include "Background.h"
#include "Game.h"
#include "DxLib.h"

namespace
{
	constexpr int kLineMargin = 100;
	constexpr int kLineThickness = 2;
	constexpr unsigned int kLineColor = 0x888888;
	constexpr float kMoveSpeed = 0.5f;
	constexpr unsigned int kBackColor = 0x555555;
}

void Background::Update()
{
	m_offset += kMoveSpeed;
	if (m_offset >= kLineMargin)
	{
		m_offset = 0;
	}
}

void Background::Draw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBackColor, true);

	for (int i = 0; i < Game::kScreenWidth / kLineMargin + 1; i++)
	{
		int x = m_offset + i * kLineMargin;
		DrawLine(x, 0, x, Game::kScreenHeight, kLineColor, kLineThickness);
	}
	for (int i = 0; i < Game::kScreenHeight / kLineMargin + 1; i++)
	{
		int y = m_offset + i * kLineMargin;
		DrawLine(0, y, Game::kScreenWidth, y, kLineColor, kLineThickness);
	}
}
