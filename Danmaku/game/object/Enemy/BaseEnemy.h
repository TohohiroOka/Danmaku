#pragma once
#include "Object3d.h"

class BaseEnemy
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;

	struct MOVE_LIST {
		//移動先座標
		XMFLOAT3 pos;
		//次の移動先候補
		std::vector<int> destination;
	};

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticUpdate(const XMFLOAT3& _playerPos);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 敵の移動方向セット
	/// </summary>
	/// <param name="_pos">プレイヤー座標</param>
	void SetMove(const XMFLOAT3& _pos);

	/// <summary>
	/// 押し戻し有の当たり判定
	/// </summary>
	void MapCollider();

	/// <summary>
	/// delete処理
	/// </summary>
	static void Finalize();

protected:

	//ボスモデル
	static std::array<std::unique_ptr<Model>, 2> bossModel;
	//enemyAモデル
	static std::unique_ptr<Model> enemyAModel;
	//プレイヤー座標
	static XMFLOAT3 playerPos;

protected:

	//オブジェクト
	std::unique_ptr<Object3d> object;
	//生存フラグ
	bool isAlive;
	//HP
	int hp;
	//座標
	XMFLOAT3 pos;
	//移動ベクトル
	XMFLOAT3 moveVec;
	//移動速度
	float speed;
	//大きさ
	float scale;
	//地面についているか
	bool onGround;

public:

	bool GetIsAlive() { return isAlive; }
	XMFLOAT3 GetPosition() { return pos; }
	float GetScale() { return scale; }
	void SetIsAlive(bool _isAlive) { isAlive = _isAlive; }
	void Damage() { 
		hp--;
		isAlive = hp != 0;
	}
};