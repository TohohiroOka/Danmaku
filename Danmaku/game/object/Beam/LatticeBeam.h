#pragma once
#include "BaseBeam.h"

class LatticeBeam : public BaseBeam
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_speed">速度</param>
	/// <param name="_color">色</param>
	LatticeBeam(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec, float _speed, const XMFLOAT3& _color);
	~LatticeBeam() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_moveVec">移動量</param>
	/// <param name="_speed">速度</param>
	/// <param name="_color">色</param>
	/// <returns></returns>
	static std::unique_ptr<LatticeBeam> Create(const XMFLOAT3& _pos, const XMFLOAT3& _moveVec,
		float _speed, const XMFLOAT3& _color);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

private:

	int timer;
};