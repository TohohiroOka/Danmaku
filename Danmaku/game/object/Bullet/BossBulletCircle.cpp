#include "BossBulletCircle.h"

using namespace DirectX;

BossBulletCircle::COLOR BossBulletCircle::colorKind = COLOR::RED;
XMFLOAT3 BossBulletCircle::circleColor = { 0.4f,0.1f,0.4f };

BossBulletCircle::BossBulletCircle(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const float _speed)
{
	pos = _pos;
	moveVec = _moveVec;
	move = { moveVec.x * _speed , moveVec.y * _speed , moveVec.z * _speed };
	color = circleColor;
	ColorChange();
}

std::unique_ptr<BossBulletCircle> BossBulletCircle::Create(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec,
	const float _speed)
{
	// 3Dオブジェクトのインスタンスを生成
	BossBulletCircle* instance = new BossBulletCircle(_pos, _moveVec, _speed);
	if (instance == nullptr) {
		return nullptr;
	}

	instance->Initialize();

	return std::unique_ptr<BossBulletCircle>(instance);
}

void BossBulletCircle::ColorChange()
{
	float variation = 0.005f;
	const XMFLOAT2 minmax = { 0.1f,0.4f };

	if (colorKind==COLOR::RED)
	{
		circleColor.z -= variation;
		if (circleColor.z <= minmax.x)
		{
			circleColor.z = minmax.x;
			colorKind = COLOR::YELLOW;
		}
	}
	else if (colorKind == COLOR::YELLOW)
	{
		circleColor.y += variation;
		if (circleColor.y >= minmax.y)
		{
			circleColor.y = minmax.y;
			colorKind = COLOR::GREEN;
		}
	}
	else if (colorKind == COLOR::GREEN)
	{
		circleColor.x -= variation;
		if (circleColor.x <= minmax.x)
		{
			circleColor.x = minmax.x;
			colorKind = COLOR::LIGHT_BLUE;
		}
	}
	else if (colorKind == COLOR::LIGHT_BLUE)
	{
		circleColor.z += variation;
		if (circleColor.z >= minmax.y)
		{
			circleColor.z = minmax.y;
			colorKind = COLOR::BLUE;
		}
	}
	else if (colorKind == COLOR::BLUE)
	{
		circleColor.y -= variation;
		if (circleColor.y <= minmax.x)
		{
			circleColor.y = minmax.x;
			colorKind = COLOR::PURPLE;
		}
	}
	else if (colorKind == COLOR::PURPLE)
	{
		circleColor.x += variation;
		if (circleColor.x >= minmax.y)
		{
			circleColor.x = minmax.y;
			colorKind = COLOR::RED;
		}
	}
}
