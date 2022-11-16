#include "BulletManager.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "ParticleManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include "GameHelper.h"

std::list<std::unique_ptr<PlayerBullet>> BulletManager::playerBullet;
std::list<std::unique_ptr<BaseBullet>> BulletManager::bossBullet;
DirectX::XMFLOAT3 BulletManager::playerPos;

BulletManager::~BulletManager()
{
	//固定砲台の弾
	bossBullet.clear();
	std::list<std::unique_ptr<BaseBullet>>().swap(bossBullet);

	BaseBullet::Finalize();
}

std::unique_ptr<BulletManager> BulletManager::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	BulletManager* instance = new BulletManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<BulletManager>(instance);
}

void BulletManager::Initialize()
{
	playerPos = { 0,0,0 };
	BaseBullet::StaticInitialize();
}

void BulletManager::Update(const XMFLOAT3& _playerPos)
{
	BaseBullet::StaticUpdate();

	playerPos = _playerPos;

	for (auto& i : bossBullet)
	{
		if (!i->GetIsAlive()) { continue; }
		i->Update();
	}
	for (auto& i : playerBullet)
	{
		i->Update();
	}

	//停止した弾を随時消去
	{
		for (auto it = bossBullet.begin(); it != bossBullet.end();)
		{
			if (!it->get()->GetIsAlive())
			{
				// 削除された要素の次を指すイテレータが返される。
				it = bossBullet.erase(it);
			}
			// 要素削除をしない場合に、イテレータを進める
			else {
				++it;
			}
		}
		for (auto it = playerBullet.begin(); it != playerBullet.end();)
		{
			if (!it->get()->GetIsAlive())
			{
				// 削除された要素の次を指すイテレータが返される。
				it = playerBullet.erase(it);
			}
			// 要素削除をしない場合に、イテレータを進める
			else {
				++it;
			}
		}
	}
}

void BulletManager::Draw()
{
	BaseBullet::Draw();
}

void BulletManager::EffectDraw()
{
	BaseBullet::EffectDraw();
}

void BulletManager::Reset()
{
	bossBullet.clear();
}

bool BulletManager::CheckEnemyBulletToPlayerCollision()
{
	//衝突用に座標と半径の大きさを借りる
	playerPos.y += 10.0f;
	float playerSize = 5;

	bool isHit = false;

	//弾とプレイヤーが衝突状態でないなら抜ける
	for (auto& i : bossBullet)
	{
		if (!i->GetIsAlive()) { continue; }

		//プレイヤーと当たっていたら消す
		if (Collision::CheckCircle2Circle(
			i->GetPosition(), i->GetScale(), playerPos, playerSize)) {
			isHit = true;
			i->SetIsAlive(false);
		}
	}
	return isHit;
}

bool BulletManager::CheckPlayerBulletToEnemyCollision(const XMFLOAT3& _pos, float _scale)
{
	XMFLOAT3 enemyPos = _pos;
	bool hit = false;

	for (auto& itr : playerBullet)
	{
		if (!itr->GetIsAlive()) { continue; }

		//弾情報
		XMFLOAT3 bulletPos = itr->GetPosition();
		float bulletSize = itr->GetScale();
		XMFLOAT3 vec = itr->GetMoveVec();

		//移動方向のレイ情報
		Ray ray;
		ray.start = { bulletPos.x,bulletPos.y,bulletPos.z,0 };
		ray.dir = { vec.x,vec.y,vec.z,0 };

		//エネミーのcollider
		Sphere sphere;
		sphere.center = { enemyPos.x,enemyPos.y,enemyPos.z,0 };
		sphere.radius = _scale * 3.0f;
		if (Collision::CheckRay2Sphere(ray, sphere))
		{
			float x = powf(enemyPos.x - bulletPos.x, 2);
			float y = powf(enemyPos.y - bulletPos.y, 2);
			float z = powf(enemyPos.z - bulletPos.z, 2);

			float eneTobul1 = sqrt(x + y + z);
			XMFLOAT3 beforepos = itr->GetMove();
			x = powf(bulletPos.x - beforepos.x - bulletPos.x, 2);
			y = powf(bulletPos.y - beforepos.y - bulletPos.y, 2);
			z = powf(bulletPos.z - beforepos.z - bulletPos.z, 2);

			float eneTobul2 = sqrt(x + y + z);

			if (eneTobul1 < eneTobul2)
			{
				hit = true;
				itr->SetIsAlive(false);
			}
		}
	}

	return hit;
}

void BulletManager::SetEnemyBullet(const XMFLOAT3& _pos, const float _speed, const XMFLOAT3& _color)
{
	Vector3 moveV = { playerPos.x - _pos.x,playerPos.y - _pos.y,playerPos.z - _pos.z };
	Vector3 moveVn = moveV.normalize();
	bossBullet.emplace_back(BossBulletNormal::Create(_pos, moveVn, _speed, _color));
}

void BulletManager::SetBossBulletHoming(const XMFLOAT3& _pos, const float _speed, const XMFLOAT3& _color)
{
	const int randMax = 50;

	XMFLOAT3 randTargetPos = playerPos;
	randTargetPos.x = randTargetPos.x + Randomfloat(randMax) * float(RandomSign());
	randTargetPos.y = randTargetPos.y + Randomfloat(randMax) * float(RandomSign());
	randTargetPos.z = randTargetPos.z + Randomfloat(randMax) * float(RandomSign());

	Vector3 moveVec = { randTargetPos.x - _pos.x, randTargetPos.y - _pos.y,randTargetPos.z - _pos.z };
	moveVec = moveVec.normalize();

	bossBullet.emplace_back(BossBulletNormal::Create(_pos, moveVec, _speed, _color));
}

void BulletManager::SetBossBulletFireWorke(const XMFLOAT3& _pos, const float _speed, const XMFLOAT3& _color)
{
	const int randMax = 50;

	XMFLOAT3 randTargetPos = playerPos;
	randTargetPos.x = randTargetPos.x + Randomfloat(randMax) * float(RandomSign());
	randTargetPos.y = randTargetPos.y + Randomfloat(randMax) * float(RandomSign());
	randTargetPos.z = randTargetPos.z + Randomfloat(randMax) * float(RandomSign());

	Vector3 moveVec = { randTargetPos.x - _pos.x, randTargetPos.y - _pos.y,randTargetPos.z - _pos.z };
	moveVec = moveVec.normalize();

	bossBullet.emplace_back(BossBulletFireWorke::Create(_pos, moveVec, _speed, _color));
}

void BulletManager::SetBossBulletHomingLine(XMFLOAT3& _pos, const float speed, const XMFLOAT3& _color,const int _progress)
{
	//弾追加
	bossBullet.emplace_back(BossBulletHomingLine::Create(_pos, _color));

	//0ならボス側で移動
	if (_progress == 0) { return; }

	//プレイヤーの周りに行くようにする
	Vector3 randPos = { _pos.x,_pos.y,_pos.z };
	randPos = randPos + Vector3(Randomfloat(100) - 50.0f, Randomfloat(100) - 50.0f, Randomfloat(100) - 50.0f);

	//次の位置を決める
	Vector3 moveVec = { playerPos.x - randPos.x,playerPos.y - randPos.y ,playerPos.z - randPos.z };
	moveVec = moveVec.normalize();
	_pos.x += moveVec.x * speed;
	_pos.y += moveVec.y * speed;
	_pos.z += moveVec.z * speed;
}

void BulletManager::SetBossBulletHomingLine1(XMFLOAT3& _pos, const float speed, const XMFLOAT3& _color)
{
	//次の位置を決める
	Vector3 moveVec = { playerPos.x - _pos.x,playerPos.y - _pos.y ,playerPos.z - _pos.z };
	moveVec = moveVec.normalize();
	//弾追加
	SetBossBulletNormal(_pos, moveVec, speed, _color);
}

void BulletManager::SetBossBulletBombHoming1(const XMFLOAT3& _pos, const float _speed, const XMFLOAT3& _color)
{
	//球状に出す
	XMFLOAT3 rand = { Randomfloat(100) - 50.0f,Randomfloat(100) - 50.0f ,Randomfloat(100) - 50.0f };
	Vector3 moveVec = { rand.x + playerPos.x - _pos.x,rand.y + playerPos.y - _pos.y ,rand.z + playerPos.z - _pos.z };
	moveVec = moveVec.normalize();
	bossBullet.emplace_back(BossBulletNormal::Create(_pos, { moveVec.x,moveVec.y,moveVec.z }, _speed, _color));
}