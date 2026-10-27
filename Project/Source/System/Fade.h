#pragma once
class Fade
{
public:
	enum class State
	{
		None,
		FadeIn,
		FadeOut
	};

	static constexpr int kFadeFrame = 20;

public:
	Fade() = default;
	~Fade() = default;

	void Update();
	void Draw();

	void StartFadeIn() { m_state = State::FadeIn; }
	void StartFadeOut() { m_state = State::FadeOut; }

	State GetState() { return m_state; }
	int GetFrame() { return m_fadeFrame; }

private:
	int m_fadeFrame = 0;	// 0 : フェードイン完了(画面が見える) / kDefaultFadeFrame : フェードアウト完了(画面が見えない)
	State m_state = State::None;
};

