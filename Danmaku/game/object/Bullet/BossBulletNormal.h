#pragma once
#include "BaseBullet.h"

class BossBulletNormal : public BaseBullet
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_speed">速度</param>
	/// <param name="_color">色</param>
	BossBulletNormal(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, float _speed, const XMFLOAT3& _color);
	~BossBulletNormal() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_speed">速度</param>
	/// <param name="_color">色</param>
	/// <returns></returns>
	static std::unique_ptr<BossBulletNormal> Create(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec,
		float _speed, const XMFLOAT3& _color);
};