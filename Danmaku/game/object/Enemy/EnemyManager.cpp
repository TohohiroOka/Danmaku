#include "EnemyManager.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "ParticleManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "GameHelper.h"

using namespace DirectX;

std::list<std::unique_ptr<BaseEnemy>> EnemyManager::enemy;
XMFLOAT3 EnemyManager::playerPos;

EnemyManager::~EnemyManager()
{
	//固定砲台の弾
	enemy.clear();
	std::list<std::unique_ptr<BaseEnemy>>().swap(enemy);

	BaseEnemy::Finalize();
}

std::unique_ptr<EnemyManager> EnemyManager::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	EnemyManager* instance = new EnemyManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<EnemyManager>(instance);
}

void EnemyManager::Initialize()
{
	BaseEnemy::StaticInitialize();
}

void EnemyManager::Update(const XMFLOAT3& _playerPos)
{
	playerPos = _playerPos;

	BaseEnemy::StaticUpdate(playerPos);

	for (auto& i : enemy)
	{
		i->Update();
	}

	//倒された敵を随時消去
	for (auto it = enemy.begin(); it != enemy.end();)
	{
		if (!it->get()->GetIsAlive())
		{
			// 削除された要素の次を指すイテレータが返される。
			it = enemy.erase(it);
		}
		// 要素削除をしない場合に、イテレータを進める
		else {
			++it;
		}
	}
}

void EnemyManager::Draw()
{
	for (auto& i : enemy)
	{
		i->Draw();
	}
}

void EnemyManager::Reset()
{
	enemy.clear();
}

bool EnemyManager::CheckCollision()
{
	//衝突用に座標と半径の大きさを借りる
	XMFLOAT3 _playerPos = playerPos;
	_playerPos.y += 10.0f;
	float playerSize = 5;

	bool isHit = false;

	//プレイヤーが衝突状態でないなら抜ける
	for (auto& i : enemy)
	{
		if (!i->GetIsAlive()) { continue; }

		//プレイヤーと当たっていたら押し戻し
		if (Collision::CheckCircle2Circle(
			i->GetPosition(), i->GetScale(), _playerPos, playerSize)) {
		}
	}

	return isHit;
}