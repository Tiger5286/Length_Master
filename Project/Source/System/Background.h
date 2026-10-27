#pragma once
class Background
{
public:
	Background() = default;
	~Background() = default;

	void Update();
	void Draw();

private:
	float m_offset = 0.0f;
};

