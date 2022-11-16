#pragma once
#include "BaseEnemy.h"

class EnemyA : public BaseEnemy
{
public:

	EnemyA(const XMFLOAT3& _pos, const int _destination);
	~EnemyA() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_destination">次の移動先番号</param>
	/// <returns></returns>
	static std::unique_ptr<EnemyA> Create(const XMFLOAT3& _pos, const int _destination);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	//移動経路
	static std::vector<MOVE_LIST> moveList;

private:

	//タイマー
	int timer;
	//回転角
	float angle;
	//移動フラグ
	bool isMove;
	//移動タイマー
	int moveTimer;
	//現在の移動先番号
	int destinationNumber;

public:
	static void SetMoveList(const XMFLOAT3& _pos, const std::vector<int> _destination) {
		MOVE_LIST add;
		add.pos = _pos;
		add.destination = _destination;
		moveList.emplace_back(add);
	}

};