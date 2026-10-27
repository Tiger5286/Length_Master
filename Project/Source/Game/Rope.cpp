#include "Rope.h"
#include "GlobalFuncs.h"
#include "Game.h"
#include "System/SoundManager.h"

namespace
{
	// ロープの色と太さ
	//constexpr unsigned int kRopeColor = 0x00ffff;
	constexpr int kRopeThickness = 5;

	constexpr int kSEMargin = 5;
}

Rope::Rope(const Vector2& startPos)
{
	m_anchors.push_back(startPos);
}

Rope::~Rope()
{
}

void Rope::Update(std::vector<Rect>& obstacles,float maxLength)
{
	m_maxLength = maxLength;

	// マウス入力を取得
	m_prevMouseInput = m_nowMouseInput;
	m_nowMouseInput = GetMouseInput();

	// マウスの位置を取得
	m_prevMousePos = m_mousePos;
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	m_mousePos = Vector2(mouseX, mouseY);

	if (m_isClicked)
	{
		// クリックしたら離す
		if ((m_nowMouseInput & MOUSE_INPUT_1) && !(m_prevMouseInput & MOUSE_INPUT_1))
		{
			m_isClicked = false;
		}
		// カーソルが障害物に当たったら離す
		for (const auto& rect : obstacles)
		{
			if (GlobalFuncs::IsHitRectLine(m_prevMousePos,m_mousePos,rect))
			{
				m_isClicked = false;
				break;
			}
		}
		// カーソルが画面外に行ったら離す
		Rect screenRect = Rect(0, 0, Game::kScreenWidth, Game::kScreenHeight - Game::kUISpaceHeight);
		if (!GlobalFuncs::IsHitRectLine(m_prevMousePos, m_mousePos, screenRect))
		{
			m_isClicked = false;
		}
		// マウスが動いていたら効果音を鳴らす
		if ((m_mousePos - m_prevMousePos).SquaredLength() > 0.0f && 
			GetLength() < m_maxLength)
		{
			m_seFrame++;
			if (m_seFrame % kSEMargin == kSEMargin - 1)
			{
				SoundManager::GetInstance().PlaySoundGame(L"extendRope");
			}
		}
		else
		{
			m_seFrame = 0;
		}
		Stretch(obstacles);
	}
	else
	{
		// 最終端をクリックしたら持つ
		if ((m_nowMouseInput & MOUSE_INPUT_1) && !(m_prevMouseInput & MOUSE_INPUT_1))
		{
			if (GlobalFuncs::IsPointInCircle(m_mousePos, m_anchors.back(), kRopeThickness * 2))
			{
				m_isClicked = true;
			}
		}
	}
}

void Rope::Draw()
{
	unsigned int color = 0x00ffff;
	if (GetLength() > m_maxLength) color = 0xff0000;

	for (auto& anchor : m_anchors)
	{
		// 最後のアンカーでなければ、次のアンカーまで線を引く
		if (&anchor != &m_anchors.back())
		{
			Vector2 nextAnchorPos = *(std::next(&anchor));
			DrawGlowLine(anchor, nextAnchorPos, color);
			
		}
		else if (m_isClicked)	// 最後のアンカーの場合、かつ掴んでいる場合
		{
			DrawGlowLine(anchor, m_mousePos, color);
		}
		DrawCircle(anchor.x, anchor.y, kRopeThickness / 2 - 1, color, true);
	}

	// 開始位置と最終位置に大きめの丸を描画
	Vector2 anchor = m_anchors[0];
	DrawCircle(anchor.x, anchor.y, kRopeThickness * 2, color, true);
	if (m_isClicked)
	{
		DrawCircle(m_mousePos.x, m_mousePos.y, kRopeThickness * 2, color, true);
	}
	else
	{
		anchor = m_anchors[m_anchors.size() - 1];
		DrawCircle(anchor.x, anchor.y, kRopeThickness * 2, color, true);
	}
}

float Rope::GetLength() const
{
	float result = 0.0f;
	if (m_isClicked)
	{
		for (const auto& anchor : m_anchors)
		{
			if (&anchor != &m_anchors.back())
			{
				Vector2 nextAnchorPos = *(std::next(&anchor));
				result += (nextAnchorPos - anchor).Length();
			}
			else
			{
				result += (m_mousePos - anchor).Length();
			}
		}
	}
	else
	{
		for (const auto& anchor : m_anchors)
		{
			if (&anchor != &m_anchors.back())
			{
				Vector2 nextAnchorPos = *(std::next(&anchor));
				result += (nextAnchorPos - anchor).Length();
			}
		}
	}
	return result;
}

int Rope::GetAnchorNum() const
{
	int result = m_anchors.size();
	if (m_isClicked) result++;
	return result;
}

void Rope::Reset()
{
	Vector2 startPos = m_anchors[0];
	m_anchors.clear();
	m_anchors.push_back(startPos);
	m_isClicked = false;
}

void Rope::Stretch(std::vector<Rect>& obstacles)
{
	Vector2 lastAnchorPos = m_anchors.back();
	// 引っかかり判定
	constexpr float kEpsilon = 0.01f;	// 適当な微小値
	// アンカーとマウスがほぼ同じ位置なら判定しない(ゼロ長線分を弾く)
	if ((m_mousePos - lastAnchorPos).SquaredLength() > kEpsilon * kEpsilon)
	{
		for (const auto& rect : obstacles)
		{
			if (GlobalFuncs::IsHitRectLine(lastAnchorPos, m_mousePos, rect))
			{
				// 障害物の最も近い角をアンカーとして追加する
				Vector2 hitCorner = GlobalFuncs::GetNearestCorner(lastAnchorPos, m_mousePos, rect);
				// 最後に追加されたアンカーと同じ位置の角であれば追加しない　
				if (!m_anchors.empty() && m_anchors.back() == hitCorner ||
					GlobalFuncs::IsPointInRect(m_mousePos, rect))		// マウスが矩形の中にあるなら追加しない
				{
					break;
				}
				m_anchors.push_back(hitCorner);
				break;	// 1つの障害物に引っかかったら、他の障害物はチェックしない
			}
		}
	}

	// 外れ判定
	if (m_anchors.size() >= 2)
	{
		Vector2 prevAnchorPos = m_anchors[m_anchors.size() - 2];

		// 手前のアンカーからマウスへ直接視界が通るかチェック
		bool blocked = false;
		for (const auto& rect : obstacles)
		{
			if (GlobalFuncs::IsHitRectLine(prevAnchorPos, m_mousePos, rect))
			{
				blocked = true;
				break;
			}
		}
		// 邪魔する障害物がなければ、最後のアンカーを削除する
		if (!blocked)
		{
			m_anchors.pop_back();
		}
	}
}

void Rope::DrawGlowLine(const Vector2& p1, const Vector2& p2, unsigned int color)
{
	//Vector2 vecOffset = p2 - p1;
	//vecOffset = vecOffset.Rotate(DX_PI_F / 2);
	//vecOffset = vecOffset.Normalized() * kRopeThickness;

	//Vector2 vecOffsetOffset = vecOffset.Rotate(DX_PI_F);
	//vecOffsetOffset.Normalize();

	//for (int i = 0; i < kRopeThickness * 2; i++)
	//{
	//	Vector2 drawP1 = p1 + vecOffset + (vecOffsetOffset * i);
	//	Vector2 drawP2 = p2 + vecOffset + (vecOffsetOffset * i);
	//	SetDrawBlendMode(DX_BLENDMODE_ADD, 255 / kRopeThickness * 2);
	//	DrawLine(drawP1.x, drawP1.y, drawP2.x, drawP2.y, color, kRopeThickness);
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//}

	DrawLine(p1.x, p1.y, p2.x, p2.y, color, kRopeThickness);
}
