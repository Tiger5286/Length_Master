#include "Rect.h"

Rect::Rect():
	x(0.0f), y(0.0f), w(0.0f), h(0.0f)
{
}

Rect::Rect(float x, float y, float w, float h):
	x(x), y(y), w(w), h(h)
{
}

void Rect::Draw(unsigned int color, bool isFill, int lineThickness) const
{
	DrawBox(x, y, x + w, y + h, color, isFill, lineThickness);
}

RectPos Rect::GetRectPos() const
{
	RectPos result;
	result.lt = Vector2(x, y);
	result.rt = Vector2(x + w, y);
	result.lb = Vector2(x, y + h);
	result.rb = Vector2(x + w, y + h);
	return result;
}

bool Rect::CheckPointInRect(const Vector2& point) const
{
	return point.x > x && point.x < x + w &&
		   point.y > y && point.y < y + h;
}
