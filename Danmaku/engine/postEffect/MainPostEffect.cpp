#include "MainPostEffect.h"
#include "InterfaceObject3d.h"

MainPostEffect::MainPostEffect()
{
	toeColor = {};
	linearColor = {};
	isFog = false;
	isToneMap = false;
}

MainPostEffect::~MainPostEffect()
{
	BasePostEffect::~BasePostEffect();
}

std::unique_ptr<MainPostEffect> MainPostEffect::Create()
{
	//インスタンスを生成
	MainPostEffect* instance = new MainPostEffect();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<MainPostEffect>(instance);
}

void MainPostEffect::Initialize()
{
	BasePostEffect::Initialize();

	// 定数バッファの生成
	HRESULT result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	constBuff->SetName(L"MAINPOST_CONST");
}

void MainPostEffect::Update()
{
	// 定数バッファへデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->toeColor = toeColor;
	constMap->linearColor = linearColor;
	constMap->outlineColor = InterfaceObject3d::GetOutlineColor();
	constMap->outlineWidth = InterfaceObject3d::GetOutlineWidth();
	constMap->isFog = isFog;
	constMap->isToneMap = isToneMap;
	constBuff->Unmap(0, nullptr);
}
