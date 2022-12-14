#include "Sprite.h"
#include "WindowApp.h"
#include <cassert>
#include "GraphicsPipelineManager.h"

using namespace DirectX;
using namespace Microsoft::WRL;

ID3D12Device* Sprite::device = nullptr;
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
std::unordered_map<std::string, Sprite::INFORMATION> Sprite::texture;
XMMATRIX Sprite::matProjection;

Sprite::Sprite()
{
	//トポロジータイプの設定
	topologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	pipelineName = "SPRITE";
}

Sprite::~Sprite()
{
	vertBuff.Reset();
	constBuff.Reset();
}

bool Sprite::StaticInitialize(ID3D12Device* _device)
{
	// 初期化チェック
	assert(!Sprite::device);

	// nullptrチェック
	assert(_device);

	Sprite::device = _device;

	// 射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, float(WindowApp::GetWindowWidth()),
		float(WindowApp::GetWindowHeight()), 0.0f,
		0.0f, 1.0f);

	Sprite::LoadTexture("debugfont", "Resources/LetterResources/debugfont.png", false);

	return true;
}

void Sprite::LoadTexture(const std::string& _keepName, const std::string& _filename, bool _isDelete)
{
	// nullptrチェック
	assert(device);

	//同じキーがあればエラーを出力
	assert(!texture.count(_keepName));

	//テクスチャ読み込み
	texture[_keepName].instance = Texture::Create(_filename);
	texture[_keepName].isDelete = _isDelete;
}

void Sprite::SetTexture(const std::string& _keepName, Texture* _texture)
{
	// nullptrチェック
	assert(device);

	//同じキーがあればエラーを出力
	assert(!texture.count(_keepName));

	//テクスチャ読み込み
	texture[_keepName].instance = std::unique_ptr<Texture>(_texture);
	texture[_keepName].isDelete = false;

	texture[_keepName].instance->texBuffer->SetName(L"SET_TEX_BUFF");
}

std::unique_ptr<Sprite> Sprite::Create(const std::string& _name)
{
	// Spriteのインスタンスを生成
	Sprite* instance = new Sprite();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize(_name);

	//更新
	instance->Update();

	//ユニークポインタを返す
	return std::unique_ptr<Sprite>(instance);
}

void Sprite::Initialize(const std::string& _name)
{
	this->name = _name;

	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VERTEX) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// 頂点バッファへのデータ転送
	TransferVertices();

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VERTEX) * 4;
	vbView.StrideInBytes = sizeof(VERTEX);

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	// 定数バッファにデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}

	vertBuff->SetName(L"SPRITE_VERT_BUFF");
	constBuff->SetName(L"SPRITE_CONST_BUFF");
}

void Sprite::Update()
{
	// ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	//頂点バッファに反映
	TransferVertices();

	// 定数バッファにデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection;	// 行列の合成
		this->constBuff->Unmap(0, nullptr);
	}
}

void Sprite::Draw()
{
	// パイプラインの設定
	GraphicsPipelineManager* graPipManeger = GraphicsPipelineManager::GetInstance();
	graPipManeger->SetPipeline(cmdList, pipelineName, topologyType);
	graPipManeger = nullptr;
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, texture[name].instance->descriptor->gpu);

	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.x) * size.y;
	float bottom = (1.0f - anchorpoint.x) * size.y;
	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	VERTEX vertices[vertNum];

	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	// テクスチャ情報取得
	if (texture[name].instance->texBuffer)
	{
		D3D12_RESOURCE_DESC resDesc = texture[name].instance->texBuffer->GetDesc();

		float texLeft = texLeftTop.x / resDesc.Width;
		float texRight = (texLeftTop.x + texSize.x) / resDesc.Width;
		float texTop = texLeftTop.y / resDesc.Height;
		float texBottom = (texLeftTop.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { texLeft,	texBottom }; // 左下
		vertices[LT].uv = { texLeft,	texTop }; // 左上
		vertices[RB].uv = { texRight,	texBottom }; // 右下
		vertices[RT].uv = { texRight,	texTop }; // 右上
	}

	// 頂点バッファへのデータ転送
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

void Sprite::SceneFinalize()
{
	for (auto itr = texture.begin(); itr != texture.end(); ++itr) {
		if ((*itr).second.isDelete)
		{
			(*itr).second.instance.reset();
			auto deleteItr = itr;
			itr--;
			texture.erase(deleteItr);
			if (itr == texture.end()) { break; }
		}
	}
}

void Sprite::Finalize()
{
	for (auto itr = texture.begin(); itr != texture.end(); ++itr)
	{
		(*itr).second.instance.reset();
	}
	texture.clear();
}