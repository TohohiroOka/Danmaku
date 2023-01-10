#pragma once
#include "Object3d.h"
#include "Vector3.h"

class Player
{
private:

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="cameraAngle">カメラ角度</param>
	/// <returns></returns>
	static std::unique_ptr<Player> Create(const XMFLOAT3& _pos,const XMFLOAT2& _cameraAngle);

private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 押し戻し有の当たり判定
	/// </summary>
	void Collider();

	/// <summary>
	/// 弾のセット
	/// </summary>
	void SetBullet();

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="cameraAngle">カメラ角度</param>
	Player(const XMFLOAT3& _pos, const XMFLOAT2& _cameraAngle);
	~Player() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="cameraAngle">カメラ角度</param>
	void Update(const XMFLOAT2& _cameraAngle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 初期化
	/// </summary>
	void Reset();

	/// <summary>
	/// 地形変更時の移動処理
	/// </summary>
	/// <param name="_ratio"></param>
	void SetMoviePos(const float _ratio);

private:

	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Object3d> object = nullptr;

	XMFLOAT3 position;
	std::array<Vector3, 3> moveVec;
	XMFLOAT3 speed;
	float scale;
	XMFLOAT3 easingPos;
	XMFLOAT3 endEasingPos;

	//描画するか否か
	bool isDraw;

	//移動時のオブジェクトの回転角
	XMFLOAT3 moveObjAngle;

	//カメラ角度
	DirectX::XMFLOAT2 cameraAngle;

	//HP
	const int maxHp = 100;
	int hp;

	//ダメージ演出
	bool isDamageStaging;
	int damageTimer;

	//エネルギー
	const float bulletEnergyMax = 50.0f;
	float bulletEnergy;

	//地形変更時にmove,衝突判定を出来なくする
	bool isMovie;

public:

	/// <summary>
	/// 座標取得
	/// </summary>
	XMFLOAT3 GetPosition() { return object->GetPosition(); }

	/// <summary>
	/// 大きさ取得
	/// </summary>
	XMFLOAT3 GetScale() { return object->GetScale(); }

	void Damage() {
		damageTimer = 0;
		isDamageStaging = true;
		hp--;
	}
	bool GetIsAlive(){
		return hp > 0;
	}

	/// <summary>
	/// 最大hp
	/// </summary>
	/// <returns>最大hp</returns>
	int GetMaxHp() { return  maxHp; }

	/// <summary>
	/// 現hp取得
	/// </summary>
	/// <returns>現hp</returns>
	int GetHp() { return  hp; }

	/// <summary>
	/// bulletEnergy最大取得
	/// </summary>
	/// <returns>bulletEnergy最大値</returns>
	float GetBulletEnergyMax() { return  bulletEnergyMax; }

	/// <summary>
	/// bulletEnergy取得
	/// </summary>
	/// <returns>bulletEnergy</returns>
	float GetBulletEnergy() { return  bulletEnergy; }

	/// <summary>
	/// 地形変更時の初期化
	/// </summary>
	void SetMovie() {
		easingPos = position;
		isMovie = true;
	};

	void EndMovie() {
		isMovie = false;
	}
};