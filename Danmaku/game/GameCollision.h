#pragma once

#include "BulletManager.h"
#include "EnemyManager.h"
#include "Collision.h"

/// <summary>
/// ゲームオブジェクト当たり判定クラス
/// </summary>
class GameCollision
{
public:

	/// <summary>
	/// 弾と敵の当たり判定
	/// </summary>
	/// <param name="_bullet">弾</param>
	/// <param name="_enemy">敵</param>
	static bool CheckBulletToEnemy(BaseBullet* _bullet, BaseEnemy* _enemy);
};