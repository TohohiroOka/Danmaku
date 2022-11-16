#pragma once
#include "InterfaceScene.h"
#include "Player.h"
#include "Ground.h"
#include "TouchableObject.h"
#include <array>

#include "BulletManager.h"
#include "EnemyManager.h"
#include "BeamManager.h"
#include "BossA.h"

#include "UiManager.h"

class Boss1 : public InterfaceScene
{
private:

	enum class SCENE {
		SET,//準備
		SET_FROM_PLAY,//準備からゲームプレイまでの移行
		PLAY,//ゲームプレイ
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

	//シーン
	SCENE scene;
	//カメラの回転
	float cameraAngle;
	//カメラの高さ
	const float cameraY = 8.0f;
	//タイマー
	int timer;
	//カメラの一番奥
	float cameraBack;

	//プレイヤー
	std::unique_ptr<Player> player;

	//地形
	std::unique_ptr<Ground> ground;

	//弾
	std::unique_ptr<BulletManager> bullet;
	
	//ビーム
	std::unique_ptr<BeamManager> beam;

	//敵
	std::unique_ptr<EnemyManager> enemy;

	//ボス
	std::unique_ptr<BossA> boss;

	//UI
	std::unique_ptr<UiManager> ui;
};