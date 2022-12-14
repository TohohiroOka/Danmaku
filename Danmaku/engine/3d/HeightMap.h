#pragma once
#include "InterfaceObject3d.h"
#include "TerrainModel.h"

class BaseCollider;
class Camera;
class LightGroup;

class HeightMap : public InterfaceObject3d
{
private: // エイリアス

	struct OBJECT_INFO
	{
		float ratio;
	};

public://メンバ関数

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_model">モデル</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<HeightMap> Create(TerrainModel* _model);

private://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// モデルセット
	/// </summary>
	/// <param name="_model">モデル</param>
	void SetModel(TerrainModel* _model) {
		model = _model;
	}

public:

	HeightMap() {};
	~HeightMap();

	/// <summary>
	/// 追加の定数バッファの更新
	/// </summary>
	void AddConstBufferUpdate(const float _ratio = 1.0f);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// モデルの変換
	/// </summary>
	/// <param name="_beforeModel">変更前</param>
	/// <param name="afterModel">変更後</param>
	/// <param name="_ratio">変換割合</param>
	void SetChangeModel(TerrainModel* _beforeModel, TerrainModel* afterModel, const float _ratio = 1.0f);

private:

	//定数バッファ
	ComPtr<ID3D12Resource> constBufferOData;
	// モデル
	TerrainModel* model = nullptr;

public:

	Model* GetModel() { return model->GetModel(); }
	std::vector<Mesh::VERTEX>& GetHitVertices() { return model->GetHitVertices(); }
	std::vector<unsigned long>& GetHitIndices() { return model->GetHitIndices(); }
};