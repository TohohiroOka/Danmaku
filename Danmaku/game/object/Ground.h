#pragma once
#include "HeightMap.h"
#include "Object3d.h"
#include <unordered_map>

class Ground
{
public:

	Ground() {};
	~Ground();

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Ground> Create();

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
	void CDraw();

	/// <summary>
	/// 囲い描画
	/// </summary>
	void CubeDraw();

	/// <summary>
	/// 座標セット
	/// </summary>
	void SetPos(DirectX::XMFLOAT3 pos) {
		object->SetPosition(pos);
	}

	/// <summary>
	/// モデルを変更する 引数が1だった場合は変更終了とみなしマップ指定を変更する
	/// </summary>
	/// <param name="_ratio"></param>
	void SetChangeModel(const float _ratio, const bool _isHitChange = false) {
		ratio = _ratio;
		isChangeMap = true;
		isHitChange = _isHitChange;
	}

private:

	//使用するマップの名前配列
	static const int modelNum = 5;
	std::array<std::string, modelNum> mapName;

	//マップを覆うキューブ
	std::array<std::unique_ptr<Model>, 2> cubeModel;
	std::array<std::unique_ptr<Object3d> ,2> cubeObject;
	//実際に使用するマップモデル
	std::unique_ptr<TerrainModel> useModel;
	//マップモデル 1平面モデル その他変更用モデル
	std::array<std::unique_ptr<TerrainModel>, modelNum + 1> terrainModel;
	//マップ
	static const float scale;
	std::unique_ptr<HeightMap> object = nullptr;

	//マップ変更フラグ
	bool isChangeMap;
	//衝突判定の変更
	bool isHitChange;
	//変化の割合
	float ratio;
	//変更するマップ指定
	std::array<int, 2> ChangeMapKind;

public:

	static float GetScale() { return scale; }
};