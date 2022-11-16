﻿#include "LightGroup.h"
#include <assert.h>

using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* LightGroup::device = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* _device)
{
	// 再初期化チェック
	assert(!LightGroup::device);

	// nullptrチェック
	assert(_device);

	LightGroup::device = _device;
}

std::unique_ptr<LightGroup> LightGroup::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	// 初期化
	instance->Initialize();

	return std::unique_ptr<LightGroup>(instance);
}

LightGroup::~LightGroup()
{
	constBuff.Reset();
}

void LightGroup::Initialize()
{
	// nullptrチェック
	assert(device);

	HRESULT result;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	// 定数バッファへデータ転送
	TransferConstBuffer();
}

void LightGroup::Update()
{
	// 値の更新があった時だけ定数バッファに転送する
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* _cmdList, const UINT& _rootParameterIndex)
{
	// 定数バッファビューをセット
	_cmdList->SetGraphicsRootConstantBufferView(_rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファへデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		// 環境光
		constMap->ambientColor = ambientColor;
		// 平行光源
		for (int i = 0; i < DirLightNum; i++) {
			// ライトが有効なら設定を転送
			if (dirLights[i].IsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightVec = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights[i].GetLightColor();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->dirLights[i].active = 0;
			}
		}
		// 点光源
		for (int i = 0; i < PointLightNum; i++) {
			// ライトが有効なら設定を転送
			if (pointLights[i].IsActive()) {
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightPos = pointLights[i].GetLightPos();
				constMap->pointLights[i].lightColor = pointLights[i].GetLightColor();
				constMap->pointLights[i].lightAtten = pointLights[i].GetLightAtten();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->pointLights[i].active = 0;
			}
		}
		// スポットライト
		for (int i = 0; i < SpotLightNum; i++) {
			// ライトが有効なら設定を転送
			if (spotLights[i].IsActive()) {
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightVec = -spotLights[i].GetLightDir();
				constMap->spotLights[i].lightPos = spotLights[i].GetLightPos();
				constMap->spotLights[i].lightColor = spotLights[i].GetLightColor();
				constMap->spotLights[i].lightAtten = spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightFactorAngleCos = spotLights[i].GetLightFactorAngleCos();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->spotLights[i].active = 0;
			}
		}
		// 丸影
		for (int i = 0; i < CircleShadowNum; i++) {
			// 有効なら設定を転送
			if (circleShadows[i].IsActive()) {
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = -circleShadows[i].GetDir();
				constMap->circleShadows[i].casterPos = circleShadows[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight = circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos = circleShadows[i].GetFactorAngleCos();
			}
			// 無効なら色を0に
			else {
				constMap->circleShadows[i].active = 0;
			}
		}
		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSetting()
{
	dirLights[0].SetActive(true);
	dirLights[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[0].SetLightDir({ 0.0f, -1.0f, 0.0f, 0 });

	dirLights[1].SetActive(true);
	dirLights[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[1].SetLightDir({ +0.5f, +0.1f, +0.2f, 0 });

	dirLights[2].SetActive(true);
	dirLights[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[2].SetLightDir({ -0.5f, +0.1f, -0.2f, 0 });

	TransferConstBuffer();
}

void LightGroup::SetAmbientColor(const XMFLOAT3& _color)
{
	ambientColor = _color;
	dirty = true;
}

void LightGroup::SetDirLightActive(int _index, bool _active)
{
	assert(0 <= _index && _index < DirLightNum);

	dirLights[_index].SetActive(_active);
	dirty = true;
}

void LightGroup::SetDirLightDir(int _index, const XMVECTOR& _lightdir)
{
	assert(0 <= _index && _index < DirLightNum);

	dirLights[_index].SetLightDir(_lightdir);
	dirty = true;
}

void LightGroup::SetDirLightColor(int _index, const XMFLOAT3& _lightcolor)
{
	assert(0 <= _index && _index < DirLightNum);

	dirLights[_index].SetLightColor(_lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightActive(int _index, bool _active)
{
	assert(0 <= _index && _index < PointLightNum);

	pointLights[_index].SetActive(_active);
}

void LightGroup::SetPointLightPos(int _index, const XMFLOAT3& _lightpos)
{
	assert(0 <= _index && _index < PointLightNum);

	pointLights[_index].SetLightPos(_lightpos);
	dirty = true;
}

void LightGroup::SetPointLightColor(int _index, const XMFLOAT3& _lightcolor)
{
	assert(0 <= _index && _index < PointLightNum);

	pointLights[_index].SetLightColor(_lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightAtten(int _index, const XMFLOAT3& _lightAtten)
{
	assert(0 <= _index && _index < PointLightNum);

	pointLights[_index].SetLightAtten(_lightAtten);
	dirty = true;
}

void LightGroup::SetSpotLightActive(int _index, bool _active)
{
	assert(0 <= _index && _index < SpotLightNum);

	spotLights[_index].SetActive(_active);
}

void LightGroup::SetSpotLightDir(int _index, const XMVECTOR& _lightdir)
{
	assert(0 <= _index && _index < SpotLightNum);

	spotLights[_index].SetLightDir(_lightdir);
	dirty = true;
}

void LightGroup::SetSpotLightPos(int _index, const XMFLOAT3& _lightpos)
{
	assert(0 <= _index && _index < SpotLightNum);

	spotLights[_index].SetLightPos(_lightpos);
	dirty = true;
}

void LightGroup::SetSpotLightColor(int _index, const XMFLOAT3& _lightcolor)
{
	assert(0 <= _index && _index < SpotLightNum);

	spotLights[_index].SetLightColor(_lightcolor);
	dirty = true;
}

void LightGroup::SetSpotLightAtten(int _index, const XMFLOAT3& _lightAtten)
{
	assert(0 <= _index && _index < SpotLightNum);

	spotLights[_index].SetLightAtten(_lightAtten);
	dirty = true;
}

void LightGroup::SetSpotLightFactorAngle(int _index, const XMFLOAT2& _lightFactorAngle)
{
	assert(0 <= _index && _index < SpotLightNum);

	spotLights[_index].SetLightFactorAngle(_lightFactorAngle);
	dirty = true;
}

void LightGroup::SetCircleShadowActive(int _index, bool _active)
{
	assert(0 <= _index && _index < CircleShadowNum);

	circleShadows[_index].SetActive(_active);
}

void LightGroup::SetCircleShadowCasterPos(int _index, const XMFLOAT3& _casterPos)
{
	assert(0 <= _index && _index < CircleShadowNum);

	circleShadows[_index].SetCasterPos(_casterPos);
	dirty = true;
}

void LightGroup::SetCircleShadowDir(int _index, const XMVECTOR& _lightdir)
{
	assert(0 <= _index && _index < CircleShadowNum);

	circleShadows[_index].SetDir(_lightdir);
	dirty = true;
}

void LightGroup::SetCircleShadowDistanceCasterLight(int _index, float _distanceCasterLight)
{
	assert(0 <= _index && _index < CircleShadowNum);

	circleShadows[_index].SetDistanceCasterLight(_distanceCasterLight);
	dirty = true;
}

void LightGroup::SetCircleShadowAtten(int _index, const XMFLOAT3& _lightAtten)
{
	assert(0 <= _index && _index < CircleShadowNum);

	circleShadows[_index].SetAtten(_lightAtten);
	dirty = true;
}

void LightGroup::SetCircleShadowFactorAngle(int _index, const XMFLOAT2& _lightFactorAngle)
{
	assert(0 <= _index && _index < CircleShadowNum);

	circleShadows[_index].SetFactorAngle(_lightFactorAngle);
	dirty = true;
}
