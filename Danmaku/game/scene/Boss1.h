#pragma once
#include "InterfaceScene.h"
#include <array>

#include "Player.h"
#include "TerrainBox.h"
#include "BulletManager.h"

class Boss1 : public InterfaceScene
{
private:

	struct EASING_POS {
		XMFLOAT3 pos = {};
		XMFLOAT3 s_pos = {};
		XMFLOAT3 e_pos = {};
	};

	enum class SCENE {
		SET,//準備
		SET_FROM_PLAY,//準備からゲームプレイまでの移行
		PLAY,//ゲームプレイ
		PLAY_FROM_MAP_CHANGE,//プレイからマップの変更
		MAP_CHANGE,//マップの変更
		MAP_CHANGE_FROM_PLAY,//マップの変更からプレイ
		HOME,//ホーム画面
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	///	更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 更新
	/// </summary>
	void CameraUpdate(Camera* camera) override;

	/// <summary>
	/// 描画
	/// </summary>
	void DrawNotPostB() override;

	/// <summary>
	///	描画
	/// </summary>
	void Draw() override;
	
	/// <summary>
	/// 描画
	/// </summary>
	void DrawNotPostA() override;

	/// <summary>
	///	解放
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// imguiの表示
	/// </summary>
	void ImguiDraw() override;

private:

	//カメラの座標
	EASING_POS cameraPos;
	//カメラのターゲット
	EASING_POS cameraTarget;
	//カメラの回転
	XMFLOAT2 cameraAngle;
	//初期位置
	XMFLOAT3 initCameraPos;
	XMFLOAT3 initCameraTarget;
	XMFLOAT2 initCameraAngle;
	//タイマー
	int timer;
	//カメラの一番奥
	float cameraBack;
	
	//lightの位置角度
	float lightAngle;

	//シーン
	SCENE scene;

	//地形
	std::unique_ptr<TerrainBox> terrain;

	//弾
	std::unique_ptr<BulletManager> bullet;

	//プレイヤー
	std::unique_ptr<Player> player;

};