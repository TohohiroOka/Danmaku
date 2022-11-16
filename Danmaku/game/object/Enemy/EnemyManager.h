#pragma once
#include "TutorialEnemy.h"
#include "EnemyA.h"

class EnemyManager
{
private:

	using XMFLOAT3 = DirectX::XMFLOAT3;

public:

	EnemyManager() {};
	~EnemyManager();

	/// <summary>
	/// 生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<EnemyManager> Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="_player">プレイヤー座標</param>
	void Update(const XMFLOAT3& _playerPos);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 初期化
	/// </summary>
	void Reset();

	/// <summary>
	/// 弾と地形、プレイヤーとの当たり判定
	/// </summary>
	bool CheckCollision();

private:

	//敵A
	static std::list<std::unique_ptr<BaseEnemy>> enemy;
	//プレイヤー座標
	static XMFLOAT3 playerPos;

public:

	static void SetEnemyAMoveList(const XMFLOAT3& _pos, const std::vector<int> _destination) {
		EnemyA::SetMoveList(_pos, _destination);
	}

	static void SetTutorialEnemy(const XMFLOAT3& _pos) {
		enemy.emplace_back(TutorialEnemy::Create(_pos));
	}

	static void SetEnemyA(const XMFLOAT3& _pos, const int _destination) {
		enemy.emplace_back(EnemyA::Create(_pos, _destination));
	}

	std::list<std::unique_ptr<BaseEnemy>>::iterator& GetEnemyA() {
		return enemy.begin();
	}
};