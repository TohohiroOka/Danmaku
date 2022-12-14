#pragma once
#include "BaseBullet.h"

class BossBulletFireWorke :
    public BaseBullet
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_speed">速度</param>
	/// <param name="_color">色</param>
	BossBulletFireWorke(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, const float _speed, const XMFLOAT3& _color);
	~BossBulletFireWorke() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_color">色</param>
	/// <returns></returns>
	static std::unique_ptr<BossBulletFireWorke> Create(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec,
		const float _speed, const XMFLOAT3& _color);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	//タイマー
	int timer;
};

