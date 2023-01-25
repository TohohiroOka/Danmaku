#include "HeightMap.h"
#include "Camera.h"
#include "SafeDelete.h"
#include "Easing.h"

using namespace Microsoft::WRL;
using namespace DirectX;

GraphicsPipelineManager::GRAPHICS_PIPELINE HeightMap::pipeline;

std::unique_ptr<HeightMap> HeightMap::Create(TerrainModel* _model)
{
	//インスタンスを生成
	HeightMap* instance = new HeightMap();
	if (instance == nullptr) {
		return nullptr;
	}

	instance->SetModel(_model);

	//初期化
	instance->Initialize();

	return std::unique_ptr<HeightMap>(instance);
}

void HeightMap::PreDraw()
{
	// パイプラインステートの設定
	cmdList->SetPipelineState(pipeline.pipelineState.Get());

	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	//プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void HeightMap::Initialize()
{
	HRESULT result = S_FALSE;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(OBJECT_INFO) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferOData));
	if (FAILED(result)) { assert(0); }

	InterfaceObject3d::Initialize();
}

void HeightMap::AddConstBufferUpdate(const float _ratio)
{
	HRESULT result = S_FALSE;

	OBJECT_INFO* constMap = nullptr;
	result = constBufferOData->Map(0, nullptr, (void**)&constMap);//マッピング
	if (SUCCEEDED(result)) {
		constMap->ratio = _ratio;
		constBufferOData->Unmap(0, nullptr);
	}
}

void HeightMap::Draw()
{
	InterfaceObject3d::Draw();

	cmdList->SetGraphicsRootConstantBufferView(3, constBufferOData->GetGPUVirtualAddress());

	model->Draw(cmdList);
}

void HeightMap::SetChangeDrawModel(TerrainModel* _beforeModel, TerrainModel* afterModel, const float _ratio)
{
	std::vector<Mesh::VERTEX> verticesC = model->GetModel()->GetMeshes()[0]->GetVertices();

	const std::array<std::vector<Mesh::VERTEX>, 2> vertices = {
		_beforeModel->GetModel()->GetMeshes()[0]->GetVertices(),
		afterModel->GetModel()->GetMeshes()[0]->GetVertices() };

	int i = 0;
	Mesh& nowMesh = *model->GetModel()->GetMeshes()[0];

	if(_ratio<1){
		for (auto& vertex : verticesC)
		{
			vertex.pos = Easing::Lerp(vertices[0][i].pos, vertices[1][i].pos, _ratio);
			vertex.normal = Easing::Lerp(vertices[0][i].normal, vertices[1][i].normal, _ratio);

			nowMesh.ChangeVertex(vertex, i);
			i++;
		}
	} else {
		for (auto& vertex : verticesC)
		{
			vertex.pos =vertices[1][i].pos;
			vertex.normal =  vertices[1][i].normal;
			nowMesh.ChangeVertex(vertex, i);
		}
	}
	nowMesh.SendVertex();
}

void HeightMap::SetChangeHitModel(TerrainModel* _beforeModel, TerrainModel* afterModel, const float _ratio)
{
	std::vector<Mesh::VERTEX> verticesC = model->GetHitVertices();

	const std::array<std::vector<Mesh::VERTEX>, 2> vertices = {
		_beforeModel->GetHitVertices(),
		afterModel->GetHitVertices() };

	int i = 0;

	if (_ratio < 1) {
		for (auto& vertex : verticesC)
		{
			vertex.pos = Easing::Lerp(vertices[0][i].pos, vertices[1][i].pos, _ratio);
			vertex.normal = Easing::Lerp(vertices[0][i].normal, vertices[1][i].normal, _ratio);

			model->SetHitVertices(vertex, i);
			i++;
		}
	} else {
		for (auto& vertex : verticesC)
		{
			vertex.pos = vertices[1][i].pos;
			vertex.normal = vertices[1][i].normal;
			model->SetHitVertices(vertex, i);
		}
	}
}
