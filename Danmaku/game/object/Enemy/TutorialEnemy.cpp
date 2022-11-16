#include "TutorialEnemy.h"

using namespace DirectX;

TutorialEnemy::TutorialEnemy(const XMFLOAT3& _pos)
{
	//HP
	hp = 10;
	//地面についているか
	onGround = false;
	//速度
	speed = 0.0f;
	//オブジェクトの生成
	object = Object3d::Create(enemyAModel.get());
	//座標セット
	pos = _pos;
	object->SetPosition(pos);

	//大きさセット
	scale = 10.0f;
	object->SetScale({ scale ,scale ,scale });

	moveVec = { 0,0,0 };
}

std::unique_ptr<TutorialEnemy> TutorialEnemy::Create(const XMFLOAT3& _pos)
{
	// 3Dオブジェクトのインスタンスを生成
	TutorialEnemy* instance = new TutorialEnemy(_pos);
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<TutorialEnemy>(instance);
}