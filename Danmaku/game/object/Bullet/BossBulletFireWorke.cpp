#include "BossBulletFireWorke.h"
#include "BulletManager.h"

using namespace DirectX;

BossBulletFireWorke::BossBulletFireWorke(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const float _speed, const XMFLOAT3& _color)
{
	timer = 0;
	pos = _pos;
	moveVec = _moveVec;
	color = _color;
	move = { moveVec.x * _speed , moveVec.y * _speed , moveVec.z * _speed };
}

std::unique_ptr<BossBulletFireWorke> BossBulletFireWorke::Create(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const float _speed, const XMFLOAT3& _color)
{
	// 3Dオブジェクトのインスタンスを生成
	BossBulletFireWorke* instance = new BossBulletFireWorke(_pos, _moveVec, _speed, _color);
	if (instance == nullptr) {
		return nullptr;
	}

	instance->Initialize();

	return std::unique_ptr<BossBulletFireWorke>(instance);
}

void BossBulletFireWorke::Update()
{
	timer++;

	BaseBullet::Update();

	if (timer > 40)
	{
		//弾ける弾の数
		const int bulletNum = 100;
		float addAngle = 360.0f / (bulletNum / 10);
		for (int i = 0; i < bulletNum; i++)
		{
			float nowAngleXY = addAngle * float(i % 10);
			float nowAngleXZ = addAngle * float(i / 10);
			float radiunXY = XMConvertToRadians(nowAngleXY);
			float radiunXZ = XMConvertToRadians(nowAngleXZ);

			BulletManager::SetBossBulletNormal(pos,
				{ cos(radiunXY) * cos(radiunXZ),cos(radiunXY) * sin(radiunXZ),sin(radiunXY) }, 1.0f,
				{ float(i % 10) / 10.0f, float(i / bulletNum), float(i) / 10.0f });
		}
		isAlive = false;
	}
}