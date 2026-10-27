#pragma once
#include "Vector2.h"
#include <array>

// 矩形の角の座標を格納する構造体
struct RectPos
{
	Vector2 lt; // 左上
	Vector2 rt; // 右上
	Vector2 lb; // 左下
	Vector2 rb; // 右下
};

// 矩形を表すクラス
class Rect
{
public:
	Rect();
	Rect(float x, float y, float w, float h);
	~Rect() = default;

	// 矩形を描画する
	void Draw(unsigned int color, bool isFill, int lineThickness = 1) const;

	// 矩形の角の座標を取得する
	RectPos GetRectPos() const;

	bool CheckPointInRect(const Vector2& point) const;

	float x, y, w, h;
};

