#include "TerrainModel.h"
#include <DirectXTex.h>

using namespace DirectX;

const std::string TerrainModel::baseDirectory = "Resources/HeightMap/";

std::unique_ptr<TerrainModel> TerrainModel::FlatlandModelCreate(
	const std::string& _filename1, const std::string& _filename2)
{
	//インスタンスを生成
	TerrainModel* instance = new TerrainModel();
	if (instance == nullptr) {
		return nullptr;
	}

	//テクスチャ読み込み
	instance->LoadTexture(_filename1, _filename2);

	//平面の生成
	instance->FlatlandMap();

	return std::unique_ptr<TerrainModel>(instance);
}

std::unique_ptr<TerrainModel> TerrainModel::Create(const std::string& _heightmapFilename, const std::string& _filename1, const std::string& _filename2)
{
	//インスタンスを生成
	TerrainModel* instance = new TerrainModel();
	if (instance == nullptr) {
		return nullptr;
	}

	//地形用テクスチャ読み込み
	instance->HeightMapLoad(_heightmapFilename);

	//テクスチャ読み込み
	instance->LoadTexture(_filename1, _filename2);

	//モデル生成
	instance->CreaetModel();

	return std::unique_ptr<TerrainModel>(instance);
}

void TerrainModel::Draw(ID3D12GraphicsCommandList* _cmdList)
{
	model->VIDraw(_cmdList);

	//テクスチャ転送
	_cmdList->SetGraphicsRootDescriptorTable(4, texture[TEXTURE::HEIGHT_MAP_TEX]->descriptor->gpu);
	_cmdList->SetGraphicsRootDescriptorTable(5, texture[TEXTURE::GRAPHIC_TEX_1]->descriptor->gpu);
	_cmdList->SetGraphicsRootDescriptorTable(6, texture[TEXTURE::GRAPHIC_TEX_2]->descriptor->gpu);

	//描画コマンド
	_cmdList->DrawIndexedInstanced(indexNum, 1, 0, 0, 0);
}

bool TerrainModel::FlatlandMap()
{
	//平面用テクスチャ
	std::string flatlandFilename = "heightmap02.bmp";
	texture[TEXTURE::HEIGHT_MAP_TEX] = Texture::Create(baseDirectory + flatlandFilename);

	hmInfo.terrainWidth = 256;
	hmInfo.terrainHeight = 256;

	size_t resize = hmInfo.terrainWidth * hmInfo.terrainHeight;
	hmInfo.heightMap.resize(resize);

	// 画像データの読み込み
	for (int j = 0; j < hmInfo.terrainHeight; j++)
	{
		for (int i = 0; i < hmInfo.terrainWidth; i++)
		{
			int index = (hmInfo.terrainHeight * j) + i;

			hmInfo.heightMap[index].x = float(i);
			hmInfo.heightMap[index].y = 0.0f;
			hmInfo.heightMap[index].z = float(j);
		}
	}

	CreaetModel();

	return true;
}

bool TerrainModel::HeightMapLoad(const std::string& _filename)
{
	//名前結合
	std::string fname = baseDirectory + _filename;

	texture[TEXTURE::HEIGHT_MAP_TEX] = Texture::Create(fname);

	//height mapを開く
	FILE* filePtr;
	int error = fopen_s(&filePtr, fname.c_str(), "rb");
	if (error != 0)
	{
		return false;
	}

	//ファイルヘッダーの読み込み
	BITMAPFILEHEADER bitmapFileHeader;
	size_t count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	//ビットマップ情報ヘッダーの読み込み
	BITMAPINFOHEADER bitmapInfoHeader;
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	//地形の縦横幅を保存
	hmInfo.terrainWidth = bitmapInfoHeader.biWidth;
	hmInfo.terrainHeight = bitmapInfoHeader.biHeight;

	//ビットマップ画像データのサイズを計算
	int imageSize = hmInfo.terrainWidth * hmInfo.terrainHeight * 3;

	//ビットマップ画像データにメモリを割り当て
	unsigned char* bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	//ビットマップデータの先頭に移動
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	//ビットマップ画像データの読み込み
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	//ファイルを閉じる
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	//保存用配列をマップの大きさに変更
	size_t resize = hmInfo.terrainWidth * hmInfo.terrainHeight;
	hmInfo.heightMap.resize(resize);

	//赤要素のみ使用するための変数
	int k = 0;

	//地形をちょうどよくするために割る値
	float heightFactor = 10.0f;

	// 画像データの読み込み
	for (int j = 0; j < hmInfo.terrainHeight; j++)
	{
		for (int i = 0; i < hmInfo.terrainWidth; i++)
		{
			int height = static_cast<int>(bitmapImage[k]);

			int index = (hmInfo.terrainHeight * j) + i;

			hmInfo.heightMap[index].x = float(i);
			hmInfo.heightMap[index].y = float(height / 3.0f);
			hmInfo.heightMap[index].z = float(j);

			k += 3;
		}
	}

	//ビットマップ画像データの解放
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

void TerrainModel::LoadTexture(const std::string& _filename1, const std::string& _filename2)
{
	// テクスチャ無し
	std::string filepath;
	if (_filename1 == "null") {
		filepath = "Resources/SubTexture/white1x1.png";
	}
	//テクスチャ有し
	else
	{
		filepath = baseDirectory + _filename1;
	}

	texture[TEXTURE::GRAPHIC_TEX_1] = Texture::Create(filepath);

	// テクスチャ無し
	if (_filename2 == "null") {
		filepath = "Resources/SubTexture/white1x1.png";
	}
	//テクスチャ有し
	else
	{
		filepath = baseDirectory + _filename2;
	}

	texture[TEXTURE::GRAPHIC_TEX_2] = Texture::Create(filepath);
}

void TerrainModel::CreaetModel()
{
	int windthSize = hmInfo.terrainWidth - 1;
	int heightSize = hmInfo.terrainHeight - 1;

	int surfaceNum = windthSize * heightSize;
	vertNum = surfaceNum * 4;
	indexNum = surfaceNum * 6;

	std::vector<Mesh::VERTEX> vertices;
	vertices.resize(vertNum);
	std::vector<unsigned long> indices;
	indices.resize(indexNum);

	unsigned long basicsIndices[6] = { 1 ,2 ,0 ,1 ,3 ,2 };

	//挿入インデックス番号
	unsigned long index = 0;

	//描画用頂点情報保存
	{
		//頂点保存
		for (int j = 0; j < heightSize; ++j)
		{
			for (int i = 0; i < windthSize; ++i)
			{
				int index1 = (hmInfo.terrainHeight * j) + i;// 左下
				int index2 = (hmInfo.terrainHeight * j) + (i + 1);// 右下
				int index3 = (hmInfo.terrainHeight * (j + 1)) + i;// 左上
				int index4 = (hmInfo.terrainHeight * (j + 1)) + (i + 1);// 右上

				int vertNum1 = index;
				index++;
				int vertNum2 = index;
				index++;
				int vertNum3 = index;
				index++;
				int vertNum4 = index;
				index++;

				// 左上
				vertices[vertNum2].pos = hmInfo.heightMap[index3];
				vertices[vertNum2].uv = XMFLOAT2(1.0f, 0.0f);

				// 右上
				vertices[vertNum4].pos = hmInfo.heightMap[index4];
				vertices[vertNum4].uv = XMFLOAT2(0.0f, 1.0f);

				// 左下
				vertices[vertNum1].pos = hmInfo.heightMap[index1];
				vertices[vertNum1].uv = XMFLOAT2(0.0f, 0.0f);

				// 右下
				vertices[vertNum3].pos = hmInfo.heightMap[index2];
				vertices[vertNum3].uv = XMFLOAT2(1.0f, 1.0f);
			}
		}

		//インデックス保存
		for (int i = 0; i < surfaceNum; i++)
		{
			int vertexNum = i * 4;
			index = i * 6;
			indices[index] = basicsIndices[0] + vertexNum;
			index++;
			indices[index] = basicsIndices[1] + vertexNum;
			index++;
			indices[index] = basicsIndices[2] + vertexNum;
			index++;
			indices[index] = basicsIndices[3] + vertexNum;
			index++;
			indices[index] = basicsIndices[4] + vertexNum;
			index++;
			indices[index] = basicsIndices[5] + vertexNum;
		}

		int normalNum = static_cast<int>(indices.size() / 3);
		for (int i = 0; i < normalNum; i++)
		{
			index = i * 3;
			unsigned long index1 = indices[index];
			index++;
			unsigned long index2 = indices[index];
			index++;
			unsigned long index3 = indices[index];

			XMVECTOR p0 = XMLoadFloat3(&vertices[index1].pos);
			XMVECTOR p1 = XMLoadFloat3(&vertices[index2].pos);
			XMVECTOR p2 = XMLoadFloat3(&vertices[index3].pos);

			XMVECTOR v1 = XMVectorSubtract(p0, p1);
			XMVECTOR v2 = XMVectorSubtract(p0, p2);

			XMVECTOR normal = XMVector3Cross(v1, v2);
			normal = XMVector3Normalize(normal);

			XMStoreFloat3(&vertices[index1].normal, normal);
			XMStoreFloat3(&vertices[index2].normal, normal);
			XMStoreFloat3(&vertices[index3].normal, normal);
		}
	}

	//当たり判定用頂点情報保存
	{
		int hitSurfaceNum = windthSize / 2 * heightSize / 2;
		int hitVertNum = hitSurfaceNum * 4;
		int hitIndexNum = hitSurfaceNum * 6;

		hitVertices.resize(hitVertNum);
		hitIndices.resize(hitIndexNum);
		index = 0;
		//頂点保存
		for (int j = 0; j < heightSize - 1; ++j)
		{
			if (j % 2 == 1) { continue; }
			for (int i = 0; i < windthSize - 1; ++i)
			{
				if (i % 2 == 1) { continue; }
				int index1 = (hmInfo.terrainHeight * j) + i;// 左下
				int index2 = (hmInfo.terrainHeight * j) + (i + 2);// 右下
				int index3 = (hmInfo.terrainHeight * (j + 2)) + i;// 左上
				int index4 = (hmInfo.terrainHeight * (j + 2)) + (i + 2);// 右上

				int vertNum1 = index;
				index++;
				int vertNum2 = index;
				index++;
				int vertNum3 = index;
				index++;
				int vertNum4 = index;
				index++;

				// 左上
				hitVertices[vertNum2].pos = hmInfo.heightMap[index3];
				hitVertices[vertNum2].uv = XMFLOAT2(1.0f, 0.0f);

				// 右上
				hitVertices[vertNum4].pos = hmInfo.heightMap[index4];
				hitVertices[vertNum4].uv = XMFLOAT2(0.0f, 1.0f);

				// 左下
				hitVertices[vertNum1].pos = hmInfo.heightMap[index1];
				hitVertices[vertNum1].uv = XMFLOAT2(0.0f, 0.0f);

				// 右下
				hitVertices[vertNum3].pos = hmInfo.heightMap[index2];
				hitVertices[vertNum3].uv = XMFLOAT2(1.0f, 1.0f);
			}
		}

		//インデックス保存
		for (int i = 0; i < hitSurfaceNum; i++)
		{
			int vertexNum = i * 4;
			index = i * 6;
			hitIndices[index] = basicsIndices[0] + vertexNum;
			index++;
			hitIndices[index] = basicsIndices[1] + vertexNum;
			index++;
			hitIndices[index] = basicsIndices[2] + vertexNum;
			index++;
			hitIndices[index] = basicsIndices[3] + vertexNum;
			index++;
			hitIndices[index] = basicsIndices[4] + vertexNum;
			index++;
			hitIndices[index] = basicsIndices[5] + vertexNum;
		}

		int normalNum = static_cast<int>(hitIndices.size() / 3);
		for (int i = 0; i < normalNum; i++)
		{
			index = i * 3;
			unsigned long index1 = hitIndices[index];
			index++;
			unsigned long index2 = hitIndices[index];
			index++;
			unsigned long index3 = hitIndices[index];

			XMVECTOR p0 = XMLoadFloat3(&hitVertices[index1].pos);
			XMVECTOR p1 = XMLoadFloat3(&hitVertices[index2].pos);
			XMVECTOR p2 = XMLoadFloat3(&hitVertices[index3].pos);

			XMVECTOR v1 = XMVectorSubtract(p0, p1);
			XMVECTOR v2 = XMVectorSubtract(p0, p2);

			XMVECTOR normal = XMVector3Cross(v1, v2);
			normal = XMVector3Normalize(normal);

			XMStoreFloat3(&hitVertices[index1].normal, normal);
			XMStoreFloat3(&hitVertices[index2].normal, normal);
			XMStoreFloat3(&hitVertices[index3].normal, normal);
		}
	}

	Mesh* mesh = new Mesh;

	//メッシュへ保存
	for (int i = 0; i < vertNum; i++)
	{
		mesh->AddVertex(vertices[i]);
	}

	//メッシュへ保存
	for (int i = 0; i < indexNum; i++)
	{
		mesh->AddIndex(indices[i]);
	}

	mesh->CreateBuffers();

	model = new Model;
	model->SetMeshes(mesh);
}
