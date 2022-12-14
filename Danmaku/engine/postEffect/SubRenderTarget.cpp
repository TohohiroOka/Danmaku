#include "SubRenderTarget.h"

using namespace DirectX;

const float SubRenderTarget::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

std::unique_ptr<SubRenderTarget> SubRenderTarget::Create(const DirectX::XMFLOAT2& _window, const std::string& _keepName)
{
	//インスタンスを生成
	SubRenderTarget* instance = new SubRenderTarget();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize(_window, _keepName);

	//深度生成
	instance->CreateDepthBuffer();

	return std::unique_ptr<SubRenderTarget>(instance);
}

SubRenderTarget::SubRenderTarget()
{
	window = { 256.0f,256.0f };
}

SubRenderTarget::~SubRenderTarget()
{
	Sprite::~Sprite();
	descHeapRTV.Reset();
	descHeapDSV.Reset();
}

void SubRenderTarget::Initialize(const DirectX::XMFLOAT2& _window, const std::string& _keepName)
{
	HRESULT result;

	//名前の保存
	name = _keepName;

	//テクスチャサイズ指定
	window = _window;

	//テクスチャバッファ生成用変数
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		UINT(window.x), UINT(window.y),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	// テクスチャ用バッファの生成
	Texture* addTexture = new Texture();

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // テクスチャ用指定
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R16G16B16A16_FLOAT, clearColor),
		IID_PPV_ARGS(&addTexture->texBuffer));
	if (FAILED(result)) { assert(0); }

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV生成
	addTexture->descriptor = std::make_unique<DescriptorHeapManager>();
	addTexture->descriptor->CreateSRV(addTexture->texBuffer, srvDesc);

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHescDesc{};
	rtvDescHescDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHescDesc.NumDescriptors = 1;

	//RTV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&rtvDescHescDesc, IID_PPV_ARGS(&descHeapRTV));
	if (FAILED(result)) { assert(0); }

	descHeapRTV->SetName(L"SubRTV");

	//デスクリプタヒープにRTV生成
	device->CreateRenderTargetView(addTexture->texBuffer.Get(), nullptr,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), 0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));

	//テクスチャとして保存
	Sprite::SetTexture(name, addTexture);
}

void SubRenderTarget::CreateDepthBuffer()
{
	HRESULT result;

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		UINT(window.x),
		UINT(window.y),
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//深度バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));
	if (FAILED(result)) { assert(0); }

	//深度ビュー用のデスクリプタヒープ生成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビュー
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	if (FAILED(result)) { assert(0); }

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void SubRenderTarget::PreDrawScene(ID3D12GraphicsCommandList* _cmdList)
{
	//リソースバリアを変更
	_cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texture[name].instance->texBuffer.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//レンダーターゲットビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs;
	rtvHs = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), 0,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	//深度ステンシルビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	_cmdList->OMSetRenderTargets(1, &rtvHs, false, &dsvH);

	//ビューポート設定
	CD3DX12_VIEWPORT viewports;
	CD3DX12_RECT scissorRects;
	viewports = CD3DX12_VIEWPORT(0.0f, 0.0f, FLOAT(window.x), FLOAT(window.y));
	scissorRects = CD3DX12_RECT(0, 0, LONG(window.x), LONG(window.y));

	_cmdList->RSSetViewports(1, &viewports);
	//シザリング矩形設定
	_cmdList->RSSetScissorRects(1, &scissorRects);

	//全画面クリア
	_cmdList->ClearRenderTargetView(rtvHs, clearColor, 0, nullptr);

	//深度バッファのクリア
	_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void SubRenderTarget::PostDrawScene(ID3D12GraphicsCommandList* _cmdList)
{
	//リソースバリアを変更
	_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[name].instance->texBuffer.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}