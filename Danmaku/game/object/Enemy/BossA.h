#pragma once
#include "BaseEnemy.h"
#include <array>

class BossA : public BaseEnemy
{
private:

	enum class BULLET_KIND
	{
		CIRCLE,//円状に出る
		LATTICE_BAEM_SET_X,//格子状ビームセットyz平面
		LATTICE_BAEM_SET_Y,//格子状ビームセットxz平面
		LATTICE_BAEM_SET_Z,//格子状ビームセットxy平面
		FIREWORKE,//花火的なもの
		HOMING,//ホーミング
		CIRCLE_ROTATE,//円状に出る回転あり
		BOMB_HOMING,//爆破してから追従
		SNAKE,//うねうね動く
		HOMING_LINE1,//追従線(円形に出る)
		HOMING_LINE2,//追従線(追従する)
		SHOCK_WAVE,//衝撃波
		SIZE,//合計
	};

	static const int homing_line_num = 6;

	//一回分の攻撃情報
	struct BULLET_INFO
	{
		//弾の種類
		int kind;
		//1前フレームの弾の種類
		int oldKind;
		//HOMING_LINE用の座標
		XMFLOAT3 HOMING_LINEpos[homing_line_num];
		//角度ホーミング線用角度
		XMFLOAT2 radiun[homing_line_num];
		//回転軸
		XMFLOAT3 rota = { 0,0,0 };
		//ビームのセット回数
		int beamSetNum = 0;
		//格子ビームの座標
		std::array<int,2> lattice_beam_pos = { 0,0 };
	};

	static const int lattice_beam_side_num = 15;

public:

	BossA(const XMFLOAT3& _pos, const int _destination);
	~BossA() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_destination">次の移動先番号</param>
	/// <returns></returns>
	static std::unique_ptr<BossA> Create(const XMFLOAT3& _pos, const int _destination);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private:

	//一回にできる攻撃の種類
	static const int kindNum = 1;
	//ボスの最大体力
	const int maxHp = 200;
	//移動経路
	static std::vector<MOVE_LIST> moveList;
	//partsの数
	static const int partsNum = 8;
	//中心からの距離
	static const std::array<XMFLOAT3, partsNum> partsPos;

private:

	//周りのオブジェクト
	std::array<std::unique_ptr<Object3d>, partsNum> parts;
	//タイマー
	int timer;
	//地面についているか
	bool onGround;
	//移動フラグ
	bool isMove;
	//移動タイマー
	int moveTimer;
	//現在の移動先番号
	int destinationNumber;
	//次の移動先番号
	int nextDestinationNumber;
	//一回に出す弾の量
	const int bulletNum = 36;
	//一回分の攻撃情報
	std::array<BULLET_INFO, kindNum> attack;

public:

	int GetMaxHp() { return maxHp; }
	int GetHp() { return hp; }
	static void SetMoveList(const XMFLOAT3& _pos, const std::vector<int> _destination) {
		MOVE_LIST add;
		add.pos = _pos;
		add.destination = _destination;
		moveList.emplace_back(add);
	}

};
