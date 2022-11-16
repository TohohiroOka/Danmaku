#pragma once
#include "HeightMap.h"
#include "Object3d.h"

class Ground
{
public:

	Ground() {};
	~Ground() {};

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Ground> Create(const std::string& heightmapFilename,
		const std::string& filename1 = "null", const std::string& filename2 = "null");

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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

	void SetRatio(const float _ratio) {
		ratio = _ratio;
	}

private:

	//マップを覆うキューブ
	std::array<std::unique_ptr<Model>, 2> cubeModel;
	std::array<std::unique_ptr<Object3d> ,2> cubeObject;
	//マップ
	static const float scale;
	std::unique_ptr<HeightMap> object = nullptr;
	float ratio;

public:

	static float GetScale() { return scale; }
};