#pragma once
#include "Vector3.h"
#include "PlayerBullet.h"
#include "BossBulletNormal.h"
#include "BossBulletCircle.h"
#include "BossBulletFireWorke.h"
#include "BossBulletSnake.h"
#include "BossBulletHomingLine.h"
#include "BossBulletBombHoming.h"

class BulletManager
{
private:

	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:

	BulletManager() {};
	~BulletManager();

	/// <summary>
	/// 生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<BulletManager> Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="_pos">プレイヤー座標</param>
	void Update(const XMFLOAT3& _playerPos);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void EffectDraw();

	/// <summary>
	/// 初期化
	/// </summary>
	void Reset();
	
	/// <summary>
	/// 弾と地形、プレイヤーとの当たり判定
	/// </summary>
	bool CheckEnemyBulletToPlayerCollision();

	/// <summary>
	/// 弾と地形、エネミーとの当たり判定
	/// </summary>
	/// <param name="_pos">プレイヤー座標</param>
	/// <param name="_scale">大きさ</param>
	bool CheckPlayerBulletToEnemyCollision(const XMFLOAT3& _pos, float _scale);

private:

	//プレイヤーの弾
	static std::list<std::unique_ptr<PlayerBullet>> playerBullet;
	//弾配列
	static std::list<std::unique_ptr<BaseBullet>> bossBullet;
	//プレイヤー座標
	static XMFLOAT3 playerPos;

public:

	//弾セット
	static void SetPlayerBullet(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec,const float _speed, const XMFLOAT3& _color) {
		playerBullet.emplace_back(PlayerBullet::Create(_pos, _moveVec, _speed, _color));
	}

	//弾セット
	static void SetEnemyBullet(const XMFLOAT3& _pos, const float _speed, const XMFLOAT3& _color);

	//通常弾セット
	static void SetBossBulletNormal(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec,
		const float _speed, const XMFLOAT3& _color) {
		bossBullet.emplace_back(BossBulletNormal::Create(_pos, _moveVec, _speed, _color));
	}

	//円状の弾セット
	static void SetBossBulletCircle(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const float _speed) {
		bossBullet.emplace_back(BossBulletCircle::Create(_pos, _moveVec, _speed));
	}

	//花火弾セット
	static void SetBossBulletFireWorke(const XMFLOAT3& _pos, const float _speed, const XMFLOAT3& _color);

	//ホーミング弾セット
	static void SetBossBulletHoming(const XMFLOAT3& _pos, const float _speed, const XMFLOAT3& _color);

	//平面状のウネウネ弾セット
	static void SetBossBulletSnake(const XMFLOAT3& _pos, const XMFLOAT3& _color) {
		bossBullet.emplace_back(BossBulletSnake::Create(_pos, _color));
	}

	//弾セット
	static void SetBossBulletHomingLine(XMFLOAT3& _pos, const float speed, const XMFLOAT3& _color, const int _progress);

	//弾セット
	static void SetBossBulletHomingLine1(XMFLOAT3& _pos, const float speed, const XMFLOAT3& _color);

	//弾セット
	static void SetBossBulletBombHoming(XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const XMFLOAT3& _color) {
		bossBullet.emplace_back(BossBulletBombHoming::Create(_pos, _moveVec, _color));
	}

	//弾セット
	static void SetBossBulletBombHoming1(const XMFLOAT3& _pos, const float speed, const XMFLOAT3& _color);

};