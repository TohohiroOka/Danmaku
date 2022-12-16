#pragma once
#include "BaseEnemy.h"
#include <array>

class BossA : public BaseEnemy
{
private:

	//中央時の攻撃
	enum class BULLET_KIND_CENTER
	{
		NONE,//何もなし
		CIRCLE,//円状に出る
		LATTICE_BAEM_SET_X,//格子状ビームセットyz平面
		LATTICE_BAEM_SET_Y,//格子状ビームセットxz平面
		LATTICE_BAEM_SET_Z,//格子状ビームセットxy平面
		FIREWORKE,//花火的なもの
		HOMING,//ホーミング
		SNAKE,//うねうね動く
		SHOCK_WAVE,//衝撃波
		CIRCLE_ROTATE,//円状に出る
		BOMB_HOMING,//爆破してから
		SIZE,//合計
	};

	enum class BULLET_KIND_SPLIT
	{
		NONE,//何もなし
		HOMING_LINE,//追従線(円形に出る)
		SIZE,//合計
	};

	//位置
	enum class POS_STATE
	{
		CENTER,//中央
		SPLIT,//分割
		//ROTATION,//回転
		SIZE,//合計
	};

	//一回分の攻撃情報
	struct BULLET_INFO
	{
		//弾の種類
		int kind;
		//1前フレームの弾の種類
		int oldKind;
		//回転軸
		XMFLOAT3 rota = { 0,0,0 };
		//ビームのセット回数
		int beamSetNum = 0;
		//格子ビームの座標
		std::array<int,2> lattice_beam_pos = { 0,0 };
	};

	//partsごとの情報
	struct PARTS_INFO {
		std::unique_ptr<Object3d> instance;
		//角度ホーミング線用角度
		XMFLOAT2 radiun;
		//HOMING_LINE用の座標
		XMFLOAT3 HOMING_LINEpos;
	};

	static const int lattice_beam_side_num = 10;

public:

	BossA();
	~BossA() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<BossA> Create();

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

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

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
	//中心からの距離
	static const std::array<XMFLOAT3, partsNum> partsSplitPos;

private:

	//周りのオブジェクト
	std::array<PARTS_INFO, partsNum> parts;
	//タイマー
	int timer;
	//一回に出す弾の量
	const int bulletNum = 36;
	//一回分の攻撃情報
	std::array<BULLET_INFO, kindNum> attack;
	//地形変更時にmove,衝突判定を出来なくする
	bool isMovie;
	//ボスの位置
	POS_STATE posState;
	//ボスの前回の位置
	POS_STATE oldPosState;
	//移動タイマー
	int moveTimer;

public:

	int GetMaxHp() { return maxHp; }
	int GetHp() { return hp; }
	float GetHpRatio() { return float(hp) / (maxHp); }
	void SetMovie() {
		isMovie = true;
	};

	void EndMovie() {
		isMovie = false;
	}

};
