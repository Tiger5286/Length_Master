#include "GlobalFuncs.h"
#include <vector>
#include <Windows.h>

namespace
{
    // qがp-rのバウンディングボックス内にあるか(共線ケースの区間重なり判定用)
    bool OnSegmentBB(const Vector2& p, const Vector2& q, const Vector2& r)
    {
        return (q.x <= (std::max)(p.x, r.x) && q.x >= (std::min)(p.x, r.x) &&
            q.y <= (std::max)(p.y, r.y) && q.y >= (std::min)(p.y, r.y));
    }
}

bool GlobalFuncs::IsHitRectLine(const Vector2& p1, const Vector2& p2, const Rect& rect)
{
    // パターンA: 始点または終点が矩形の中にある場合
    if (IsPointInRect(p1, rect) || IsPointInRect(p2, rect)) {
        return true;
    }

    // 矩形の4つの頂点座標を準備
	RectPos rectPos = rect.GetRectPos();

    // パターンB: 線分が矩形の4つの「辺」のどれかと交差しているかチェック
    // ① 上辺 (topLeft - topRight)
    if (IsHitLineLine(p1, p2, rectPos.lt, rectPos.rt)) return true;
    // ② 下辺 (bottomLeft - bottomRight)
    if (IsHitLineLine(p1, p2, rectPos.lb, rectPos.rb)) return true;
    // ③ 左辺 (topLeft - bottomLeft)
    if (IsHitLineLine(p1, p2, rectPos.lt, rectPos.lb)) return true;
    // ④ 右辺 (topRight - bottomRight)
    if (IsHitLineLine(p1, p2, rectPos.rt, rectPos.rb)) return true;

    return false; // どの辺とも交差せず、内側にもない
}

bool GlobalFuncs::IsHitLineLine(const Vector2& a1, const Vector2& a2, const Vector2& b1, const Vector2& b2)
{
    Vector2 vA = { a2.x - a1.x, a2.y - a1.y };
    Vector2 vB1 = { b1.x - a1.x, b1.y - a1.y };
    Vector2 vB2 = { b2.x - a1.x, b2.y - a1.y };

    float cross1 = vA.Cross(vB1);
    float cross2 = vA.Cross(vB2);

    Vector2 vB = { b2.x - b1.x, b2.y - b1.y };
    Vector2 vA1 = { a1.x - b1.x, a1.y - b1.y };
    Vector2 vA2 = { a2.x - b1.x, a2.y - b1.y };

    float cross3 = vB.Cross(vA1);
    float cross4 = vB.Cross(vA2);

    // 通常の「またぎ合っている」交差ケース(厳密に符号が逆)
    if (cross1 * cross2 < 0.0f && cross3 * cross4 < 0.0f)
        return true;

    // 特殊ケース:端点が相手の線上に乗っている/共線になっている場合は
    // 外積だけでなく実際の区間の重なり(バウンディングボックス)も確認する
    const float eps = 1e-4f;
    if (fabsf(cross1) < eps && OnSegmentBB(a1, b1, a2)) return true;
    if (fabsf(cross2) < eps && OnSegmentBB(a1, b2, a2)) return true;
    if (fabsf(cross3) < eps && OnSegmentBB(b1, a1, b2)) return true;
    if (fabsf(cross4) < eps && OnSegmentBB(b1, a2, b2)) return true;

    return false;
}
bool GlobalFuncs::IsPointInRect(const Vector2& point, const Rect& rect)
{
    return (point.x >= rect.x && point.x <= rect.x + rect.w &&
		    point.y >= rect.y && point.y <= rect.y + rect.h);
}

Vector2 GlobalFuncs::GetNearestCorner(const Vector2& p1, const Vector2& p2, const Rect& rect)
{
    const float offset = 3.0f;
    RectPos rp = rect.GetRectPos();

    // オフセット適用済みの4角(埋まり防止)
    Vector2 cLT = { rp.lt.x - offset, rp.lt.y - offset };
    Vector2 cRT = { rp.rt.x + offset, rp.rt.y - offset };
    Vector2 cLB = { rp.lb.x - offset, rp.lb.y + offset };
    Vector2 cRB = { rp.rb.x + offset, rp.rb.y + offset };

    // 各辺と、その両端に対応するオフセット済みの角
    struct EdgeInfo { Vector2 a, b; Vector2 cornerA, cornerB; };
    EdgeInfo edges[4] = {
        { rp.lt, rp.rt, cLT, cRT }, // 上辺
        { rp.lb, rp.rb, cLB, cRB }, // 下辺
        { rp.lt, rp.lb, cLT, cLB }, // 左辺
        { rp.rt, rp.rb, cRT, cRB }, // 右辺
    };

    // 交差した辺の角(端点)を集める
    std::vector<Vector2> hitCorners;
    for (auto& e : edges)
    {
        if (IsHitLineLine(p1, p2, e.a, e.b))
        {
            hitCorners.push_back(e.cornerA);
            hitCorners.push_back(e.cornerB);
        }
    }

    // 交差した辺が2本の通常ケース：2本の辺が共有する角を探す
    if (hitCorners.size() == 4)
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 2; j < 4; ++j)
            {
                if (hitCorners[i] == hitCorners[j])
                {
                    return hitCorners[i]; // ★ 共有している角が正解
                }
            }
        }
    }

    // フォールバック（対辺同士がヒットした等、共有角がない場合）
    // → 従来通り最短距離の角を返す
    Vector2 corners[4] = { cLT, cRT, cLB, cRB };
    int bestIndex = 0;
    float minDistanceSq = 1e9f;
    for (int i = 0; i < 4; ++i)
    {
        float distSq = DistanceToSegmentSq(corners[i], p1, p2);
        if (distSq < minDistanceSq)
        {
            minDistanceSq = distSq;
            bestIndex = i;
        }
    }
    return corners[bestIndex];
}

float GlobalFuncs::DistanceToSegmentSq(const Vector2& p, const Vector2& a, const Vector2& b)
{
    float l2 = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
    if (l2 == 0.0f) return (p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y);

    // 線分上の投影位置（t: 0.0~1.0）
    float t = ((p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y)) / l2;
    if (t < 0.0f) t = 0.0f;
	else if (t > 1.0f) t = 1.0f;

    // 投影点までの最短距離の二乗
    Vector2 projection = { a.x + t * (b.x - a.x), a.y + t * (b.y - a.y) };
    return (p.x - projection.x) * (p.x - projection.x) + (p.y - projection.y) * (p.y - projection.y);
}

bool GlobalFuncs::IsPointInCircle(const Vector2& p, const Vector2& c, const float r)
{
    return (c - p).SquaredLength() <= r * r;
}

std::wstring GlobalFuncs::StringToWString(const std::string& str)
{
    int size = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        nullptr,
        0
    );

    std::wstring result(size - 1, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        result.data(),
        size
    );

    return result;
}

std::string GlobalFuncs::WStringToString(const std::wstring& str)
{
    int size = WideCharToMultiByte(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        nullptr,
        0,
        nullptr,
        nullptr
    );

    std::string result(size - 1, '\0');

    WideCharToMultiByte(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        result.data(),
        size,
        nullptr,
        nullptr
    );

    return result;
}
