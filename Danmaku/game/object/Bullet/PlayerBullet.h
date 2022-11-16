#pragma once
#include "BaseBullet.h"

class PlayerBullet : public BaseBullet
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_speed">速度</param>
	/// <param name="_color">色</param>
	PlayerBullet(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const float _speed, const XMFLOAT3& _color);
	~PlayerBullet() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_speed">速度</param>
	/// <param name="_color">色</param>
	/// <returns></returns>
	static std::unique_ptr<PlayerBullet> Create(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec,
		const float _speed, const XMFLOAT3& _color);
};