#include "DxLib.h"
#include "Game.h"
#include <memory>

#include "Scenes/SceneTitle.h"
#include "Scenes/SceneManager.h"
#include "System/SoundManager.h"
#include "System/SaveDataManager.h"

// プログラムは WinMain から始まる
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true); // ウインドウモードで起動

	SetMainWindowText(L"Length!"); // ウインドウのタイトルを設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum); // 画面サイズと色数を設定
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	// マウスカーソルを表示する
	SetMouseDispFlag(true);
	// 描画対象をバックバッファに変更
	SetDrawScreen(DX_SCREEN_BACK);

	AddFontResourceExA("data/fonts/main_font.otf", FR_PRIVATE, NULL);
	GlobalValues::g_mainFontHandle = CreateFontToHandle(L"YDW あおさぎ R", Game::kMainFontSize, -1);

	auto& sdManager = SaveDataManager::GetInstance();
	sdManager.Load();

	auto& soundManager = SoundManager::GetInstance();
	soundManager.LoadResources();

	SceneManager sceneManager;
	sceneManager.ResetScene(std::make_shared<SceneTitle>(sceneManager));

	while (ProcessMessage() != -1)
	{
		LONGLONG start = GetNowHiPerformanceCount(); // フレーム開始時間を取得
		ClearDrawScreen(); // 画面をクリア

		sceneManager.Update();

		soundManager.Update();

		sceneManager.Draw();

		// escキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) || sceneManager.IsExit())
		{
			break;
		}
		ScreenFlip(); // 描画した内容を画面に反映する
		while (GetNowHiPerformanceCount() - start < 16667) {} // 約16.667ミリ秒(1/60秒)待つことで60FPSに固定
	}

	sdManager.Save();

	DeleteFontToHandle(GlobalValues::g_mainFontHandle);
	RemoveFontResourceExA("data/fonts/main_font.otf", FR_PRIVATE, NULL);

	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}