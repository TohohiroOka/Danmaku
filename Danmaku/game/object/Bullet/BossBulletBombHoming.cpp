#include "BossBulletBombHoming.h"
#include "BulletManager.h"
#include "GameHelper.h"

using namespace DirectX;

BossBulletBombHoming::BossBulletBombHoming(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const XMFLOAT3& _color)
{
	pos = _pos;
	moveVec = _moveVec;
	const float speed = 10.0f;
	move = { moveVec.x * speed , moveVec.y * speed , moveVec.z * speed };
	timer = 0;
	maxTimer = int(Randomfloat(40)) + 20;
	color = _color;
}

std::unique_ptr<BossBulletBombHoming> BossBulletBombHoming::Create(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const XMFLOAT3& _color)
{
	// 3Dオブジェクトのインスタンスを生成
	BossBulletBombHoming* instance = new BossBulletBombHoming(_pos, _moveVec, _color);
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<BossBulletBombHoming>(instance);
}

void BossBulletBombHoming::Update()
{
	timer++;

	BaseBullet::Update();

	if (timer > maxTimer)
	{
		//弾ける弾の数
		const int bulletNum = 25;
		float speed = 10.0f;
		for (int i = 0; i < bulletNum; i++)
		{
			BulletManager::SetBossBulletBombHoming1(pos, speed,
				{ float(i % 10) / 10.0f, float(i / bulletNum), float(i) / 10.0f });
		}
		isAlive = false;
	}
}