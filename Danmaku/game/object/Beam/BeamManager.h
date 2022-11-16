#pragma once
#include "LatticeBeam.h"

class BeamManager
{
private:

	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:

	BeamManager() {};
	~BeamManager();

	/// <summary>
	/// 生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<BeamManager> Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 初期化
	/// </summary>
	void Reset();

	/// <summary>
	/// 弾と地形、エネミーとの当たり判定
	/// </summary>
	/// <param name="_pos">プレイヤー座標</param>
	/// <param name="_scale">大きさ</param>
	bool CheckPlayerBulletToEnemyCollision(const XMFLOAT3& _pos, float _scale);

private:

	//プレイヤーの弾
	static std::list<std::unique_ptr<BaseBeam>> beam;

public:

	//ビームセット
	static void SetLatticeBeam(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const float _speed, const XMFLOAT3& _color) {
		beam.emplace_back(LatticeBeam::Create(_pos, _moveVec, _speed, _color));
	}
};

