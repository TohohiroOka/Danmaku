#pragma once
#include "HeightMap.h"
#include "Object3d.h"
#include <unordered_map>

class TerrainBox
{
private:// エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	//周りの箱
	struct CUBE_INFO {
		//モデル
		std::array<std::unique_ptr<Model>, 2> model;
		//描画設定
		std::array<std::unique_ptr<Object3d>, 2> object;
	};

	//面の状態
	enum class SURFACE_STATE {
		STOP,
		CHANGE_ATTACK,
		CHANGE_DEFENSE,
		ATTACK,
		DEFENSE,
	};

	//地形情報
	struct TERRAIN_INFO {
		//地形情報
		std::unique_ptr<TerrainModel> model;
		//描画設定
		std::unique_ptr<HeightMap> object;
		//反対の面番号
		int reverseSurface;
		//変更地形番号
		std::array<int,2> changeModelNum;
		//面の状態
		SURFACE_STATE state;
		//衝突判定の変更
		bool isHitChange;
		//攻撃方向
		XMFLOAT3 attackDirection;
	};

	enum class PHASE {
		ATTACK,
		CHANGE_TERRAIN,
	};

public:

	TerrainBox() {};
	~TerrainBox();

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<TerrainBox> Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 判定描画
	/// </summary>
	void HitDraw();

private:
	
	/// <summary>
	/// 衝突判定セット
	/// </summary>
	void SetCollider(TERRAIN_INFO& _surface);

	/// <summary>
	/// 変更面のセット
	/// </summary>
	void SetChangeTerrain();

	/// <summary>
	/// 地形の変遷
	/// </summary>
	void ChangeTerrain();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private:

	//フィールドを覆うキューブ数
	static const int cube_num = 2;
	//保持モデル数
	static const int model_num = 6;
	//地形の数
	static const int box_surface_num = 6;
	//地形の大きさ
	static const float box_scale;

	enum MODEL_NUMBER {
		FLAT = 0,//平面番号
		ATTACK = 3,//この番号まで攻撃用地形
		DEFENSE = model_num,//この番号まで防御地形
	};

private:

	//タイマー
	int timer;
	//フィールドを覆うキューブ
	std::array<CUBE_INFO, cube_num> cube;
	//保持モデル
	std::array<std::unique_ptr<TerrainModel>, model_num> terrainModel;
	//地形描画
	std::array<TERRAIN_INFO, box_surface_num> surface;
	//現在の地形の状態
	PHASE phase;

public:

	static float GetScale() { return box_scale; }

};