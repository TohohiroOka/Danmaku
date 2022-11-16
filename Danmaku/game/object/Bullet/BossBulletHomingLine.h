#pragma once
#include "BaseBullet.h"

class BossBulletHomingLine :
	public BaseBullet
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_color">色</param>
	BossBulletHomingLine(const XMFLOAT3& _pos, const XMFLOAT3& _color);
	~BossBulletHomingLine() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_color">色</param>
	/// <returns></returns>
	static std::unique_ptr<BossBulletHomingLine> Create(const XMFLOAT3& _pos, const XMFLOAT3& _color);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	//タイマー
	int timer;
};