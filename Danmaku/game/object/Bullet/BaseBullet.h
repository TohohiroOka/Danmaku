#pragma once
#include "InstanceObject.h"
#include "Emitter.h"

class BaseBullet
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;

	enum class COLOR {
		RED,//1,0,0
		YELLOW,//1,1,0
		GREEN,//0,1,0
		LIGHT_BLUE,//0,1,1
		BLUE,//0,0,1
		PURPLE,//1,0,1
	};

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 静的更新
	/// </summary>
	static void StaticUpdate();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	static void Draw();

	static void EffectDraw();

	/// <summary>
	/// delete処理
	/// </summary>
	static void Finalize();

protected:

	//モデル
	static std::unique_ptr<Model> model;
	//オブジェクトの最大値
	static const int object_max_num = 20;
	//オブジェクトの最大値
	static const int effect_max_num = 20 * 10;
	//オブジェクト
	static std::array<std::unique_ptr<InstanceObject>, object_max_num> bullet;
	//現在の使用個数
	static int usingNum;
	//
	static std::array<std::unique_ptr<Emitter>, effect_max_num> effect;
	static std::array<int, effect_max_num> effectNum;

protected:

	//生存フラグ
	bool isAlive;
	//座標
	XMFLOAT3 pos;
	//移動ベクトル
	XMFLOAT3 moveVec;
	//移動速度
	XMFLOAT3 move;
	//大きさ
	float scale;
	//角度
	XMFLOAT3 rotate;
	//色
	XMFLOAT3 color = { 1,1,1 };

public:

	void SetIsAlive(bool _isAlive) { isAlive = _isAlive; }
	bool GetIsAlive() { return isAlive; }
	XMFLOAT3 GetPosition() { return pos; }
	float GetScale() { return scale; }
	XMFLOAT3 GetMoveVec() { return moveVec; }
	XMFLOAT3 GetMove() { return move; }
};

