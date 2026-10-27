#pragma once

// ゲーム全体で使用する定数
namespace Game
{
	// 画面情報
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;
	constexpr int kColorBitNum = 32;
	// ステージ数
	constexpr int kStageNum = 10;
	// 下のUI用のスペースの高さ
	constexpr int kUISpaceHeight = 90;

	// メインフォントのサイズ
	constexpr int kMainFontSize = 32;
}

// ゲーム全体で使用する変数
namespace GlobalValues
{
	// フォントハンドル(main.cppでロード、解放を行う)
	inline int g_mainFontHandle = -1;
}