#pragma once
#include "BaseBullet.h"

class BossBulletCircle : public BaseBullet
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_speed">速度</param>
	BossBulletCircle(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const float _speed);
	~BossBulletCircle() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_speed">速度</param>
	/// <returns></returns>
	static std::unique_ptr<BossBulletCircle> Create(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const float _speed);

	/// <summary>
	/// 色変換
	/// </summary>
	static void ColorChange();

private:

	//色
	static COLOR colorKind;
	//球状のダンマクの色
	static XMFLOAT3 circleColor;

};