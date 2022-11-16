#include "BossBulletSnake.h"
#include "GameHelper.h"
using namespace DirectX;

BossBulletSnake::BossBulletSnake(const XMFLOAT3& _pos, const XMFLOAT3& _color)
{
	timer = 0;
	divideNum = int(Randomfloat(50)) + 10;
	pos = _pos;
	move = { 0,0,0 };
	moveVec = { 0,0,0 };
	angle = float((Randomfloat(360 * 2) - 360));
	velocity = 2.0f;
	color = _color;
}

std::unique_ptr<BossBulletSnake> BossBulletSnake::Create(const XMFLOAT3& _pos, const XMFLOAT3& _color)
{
	// 3Dオブジェクトのインスタンスを生成
	BossBulletSnake* instance = new BossBulletSnake(_pos, _color);
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<BossBulletSnake>(instance);
}

void BossBulletSnake::Update()
{
	timer++;

	if (timer % divideNum == 0)
	{
		velocity *= -1.0f;
	}

	float speed = 5.0f;
	angle += velocity;
	angle = float(int(angle) % 360);

	float radiun = XMConvertToRadians(angle);
	move.x = speed * cosf(radiun);
	move.z = speed * sinf(radiun);

	BaseBullet::Update();
}