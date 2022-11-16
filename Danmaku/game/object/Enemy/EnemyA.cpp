#include "EnemyA.h"
#include "BulletManager.h"
#include "GameHelper.h"
#include "Easing.h"

using namespace DirectX;

std::vector<BaseEnemy::MOVE_LIST> EnemyA::moveList;

EnemyA::EnemyA(const XMFLOAT3& _pos, const int _destination)
{
	//タイマー
	timer = 0;
	//HP
	hp = 10;
	//地面についているか
	onGround = false;
	//速度
	speed = 3.0f;
	//回転角
	angle = 0;
	//オブジェクトの生成
	object = Object3d::Create(enemyAModel.get());
	//座標セット
	pos = _pos;
	object->SetPosition(pos);

	//大きさセット
	scale = 10.0f;
	object->SetScale({ scale ,scale ,scale });

	destinationNumber = _destination;
	moveVec = { 0,0,0 };
	isMove = true;
}

std::unique_ptr<EnemyA> EnemyA::Create(const XMFLOAT3& _pos, const int _destination)
{
	// 3Dオブジェクトのインスタンスを生成
	EnemyA* instance = new EnemyA(_pos, _destination);
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<EnemyA>(instance);
}

void EnemyA::Update()
{
	timer++;
	const float maxTimer = 80.0f;
	//角度
	XMFLOAT2 objAngle = {};
	objAngle.x = GetAngle({ pos.x,pos.z }, { playerPos.x, playerPos.z });//y軸
	objAngle.y = GetAngle({ pos.x,pos.y }, { playerPos.x, playerPos.y });//z軸
	object->SetRotation({ 0,objAngle.x, objAngle.y });

	//弾の発射
	if (timer % 150 == 0)
	{
		BulletManager::SetEnemyBullet(pos, 10.0f, { 0.8f,0.2f, 0.8f });
	}

	if (isMove)
	{
		moveTimer++;
		float ratio = float(moveTimer) / maxTimer;
		int nextNum = moveList[destinationNumber].destination[0];
		pos.x = Easing::Lerp(moveList[destinationNumber].pos.x, moveList[nextNum].pos.x, ratio);
		pos.z = Easing::Lerp(moveList[destinationNumber].pos.z, moveList[nextNum].pos.z, ratio);

		if (ratio >= 1.0f) {
			destinationNumber = moveList[destinationNumber].destination[0];
			moveTimer = 0;
		}
	}

	BaseEnemy::Update();
}