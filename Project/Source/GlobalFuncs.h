#pragma once
#include "Utility/Vector2.h"
#include "Utility/Rect.h"
#include <string>

class GlobalFuncs
{
public:
	// 線分と矩形の当たり判定
	static bool IsHitRectLine(const Vector2& p1, const Vector2& p2, const Rect& rect);
	// 線分同士の交差判定
	static bool IsHitLineLine(const Vector2& a1, const Vector2& a2, const Vector2& b1, const Vector2& b2);
	// 点が矩形の中にあるか判定
	static bool IsPointInRect(const Vector2& point, const Rect& rect);
	// 線分に最も近い矩形の角を返す
	static Vector2 GetNearestCorner(const Vector2& p1, const Vector2& p2, const Rect& rect);

	// 点と線分の距離の二乗を返す
	static float DistanceToSegmentSq(const Vector2& p, const Vector2& a, const Vector2& b);
	// 点と円の当たり判定をする
	static bool IsPointInCircle(const Vector2& p, const Vector2& c, const float r);

	// stringをwstringに変換する
	static std::wstring StringToWString(const std::string& str);
	// wstringをstringに変換する
	static std::string WStringToString(const std::wstring& str);
};