#include "WindowApp.h"
#include "DirectXCommon.h"
#include "MainEngine.h"
#include "SafeDelete.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ウィンドウサイズ
	const int window_width = 1280;// 横幅
	const int window_height = 720;// 縦幅

	//ゲーム名
	const wchar_t* gameName = L"ダンマク";

	//ウィンドウ初期化
	WindowApp* winApp = new WindowApp();
	winApp->Initialize(window_width, window_height, gameName);

	//DirectXCommonのインスタンス
	DirectXCommon* dXCommon = DirectXCommon::Create();

	//全体の初期化
	MainEngine* engine = new MainEngine();
	engine->Initialize(dXCommon->GetDevice());

	while (true)
	{
		if (engine->Update()|| winApp->Update()) { break; }

		engine->PostEffectDraw(dXCommon->GetCmdList());

		//描画前設定
		dXCommon->PreDraw();

		engine->Draw(dXCommon->GetCmdList());

		//コマンド実行
		dXCommon->PostDraw();

		//フレームレート管理
		engine->FrameControl();
	}

	//登録解除
	safe_delete(engine);
	safe_delete(dXCommon);
	winApp->Release();

	return 0;
}