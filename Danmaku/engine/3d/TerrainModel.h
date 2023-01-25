#pragma once
#include "Model.h"
#include <DirectXMath.h>

class TerrainModel
{
private: // エイリアス

	// DirectX::を省略
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:

	struct HEIGHT_MAP_INFO {
		int terrainWidth;//幅
		int terrainHeight;//高さ
		std::vector<XMFLOAT3> heightMap;//地形情報
	};

	enum TEXTURE {
		HEIGHT_MAP_TEX,
		GRAPHIC_TEX_1,
		GRAPHIC_TEX_2,
		SIZE,
	};

public:

	enum class FACE_DIRECTION {
		X_PLUS,
		X_MINUS,
		Y_PLUS,//ノーマル
		Y_MINUS,//下向き
		Z_PLUS,
		Z_MINUS,
		SIZE,
	};

public:

	TerrainModel() {};
	~TerrainModel() {};

	/// <summary>
	/// 平面の生成
	/// </summary>
	/// <param name="_direction">向き</param>
	/// <param name="_filename">テクスチャ名1</param>
	/// <param name="_filename2">テクスチャ名2</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<TerrainModel> FlatlandModelCreate(const FACE_DIRECTION _direction,
		const std::string& _filename1 = "null", const std::string& _filename2 = "null");

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_heightmapFilename">heightmap名</param>
	/// <param name="_direction">向き</param>
	/// <param name="_filename">テクスチャ名1</param>
	/// <param name="_filename2">テクスチャ名2</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<TerrainModel> Create(const std::string& _heightmapFilename, const FACE_DIRECTION _direction,
		const std::string& _filename1 = "null", const std::string& _filename2 = "null");

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* _cmdList);

private:

	/// <summary>
	/// 平面生成
	/// </summary>
	/// <param name="_direction">向き</param>
	/// <returns>成功か</returns>
	bool FlatlandMap(const FACE_DIRECTION _direction);

	/// <summary>
	/// ハイトマップ
	/// </summary>
	/// <param name="_direction">向き</param>
	/// <param name="_filename">ファイル名</param>
	/// <returns>成功か</returns>
	bool HeightMapLoad(const FACE_DIRECTION _direction, const std::string& _filename);

	/// <summary
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="_filename1">ファイル名1</param>
	/// <param name="_filename2">ファイル名2</param>
	void LoadTexture(const std::string& _filename1, const std::string& _filename2);

	/// <summary>
	/// モデルの生成
	/// </summary>
	/// <param name="_direction">向き</param>
	void CreaetModel(const FACE_DIRECTION _direction);

private:

	//基礎ファイル
	static const std::string baseDirectory;

private:

	//ハイトマップの情報
	HEIGHT_MAP_INFO hmInfo;
	//テクスチャ情報
	std::array<std::unique_ptr<Texture>, TEXTURE::SIZE> texture;
	// モデル
	Model* model = nullptr;
	//インデックスの大きさ
	int indexNum = 0;
	//頂点数
	int vertNum = 0;
	//当たり判定用頂点情報
	std::vector<Mesh::VERTEX> hitVertices;
	std::vector<unsigned long> hitIndices;

	//モデルの向き
	FACE_DIRECTION direction;

public:

	Model* GetModel() { return model; }
	std::vector<Mesh::VERTEX>& GetHitVertices() { return hitVertices; }
	std::vector<unsigned long>& GetHitIndices() { return hitIndices; }
	void SetMesh(Mesh* _mesh) { model->ResetASetMeshes(_mesh); }
	void SetHitVertices(const Mesh::VERTEX& inVertex, const int arrayNum) {
		hitVertices[arrayNum] = inVertex;
	}
};

