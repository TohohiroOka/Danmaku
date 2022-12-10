#pragma once
#include "DirectXCommon.h"
#include "SceneManager.h"
#include "DirectInput.h"
#include "XInputManager.h"
#include "FrameRateKeep.h"
#include "CubeMap.h"

//ポストエフェクト系
#include "MainRenderTarget.h"
#include "SubRenderTarget.h"
#include "MainPostEffect.h"
#include "ShrinkBuffer.h"

#include <sstream>
#include <iomanip>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

class WindowApp;

class MainEngine
{
private:

	struct RENDER_INFO {
		//書き込むレンダーターゲットの名前
		std::string renderName;
		//書き込むレンダーターゲット
		std::unique_ptr<SubRenderTarget> renderTarget;
	};

public:

	MainEngine() = default;
	~MainEngine();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <returns>ゲームを続けるか</returns>
	bool Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// フレームレートキープ
	/// </summary>
	void FrameControl();

private:

	//DirectXCommonのインスタンス
	std::unique_ptr<DirectXCommon> dXCommon = nullptr;
	//Inputのインスタンス
	DirectInput* input = nullptr;
	//XInputManager
	XInputManager* xinput = nullptr;
	//GameSceneのインスタンス
	std::unique_ptr<SceneManager> scene = nullptr;
	//Fps固定用クラスのインスタンス
	std::unique_ptr<FrameRateKeep> fps = nullptr;
	//キューブマップ
	//std::unique_ptr<CubeMap> cubemap;
	//数字表示デバッグ用
	wchar_t str[256] = {};

	///ポストエフェクト系
	//レンダーターゲットを取得する用
	std::unique_ptr<MainRenderTarget> mainRenderTarget;
	//ブルーム
	RENDER_INFO bloom;
	//ブルーム描画
	std::unique_ptr<BasePostEffect> bloomDraw;
	//縮小バッファ
	std::array<RENDER_INFO, 5> R_shrinkBuffer;
	//ポストエフェクト用テクスチャ描画
	std::array<std::unique_ptr<ShrinkBuffer>, 5> D_shrinkBuffer;
	//ポストエフェクトを合わせる用
	std::unique_ptr<MainPostEffect> mainDraw;

};