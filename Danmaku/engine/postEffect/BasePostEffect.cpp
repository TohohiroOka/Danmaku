#include "BasePostEffect.h"
#include "GraphicsPipelineManager.h"

BasePostEffect::BasePostEffect()
{
	this->Initialize();
}

void BasePostEffect::Initialize()
{
	HRESULT result;

	topologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VERTEX) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// 頂点データ
	VERTEX vertices
		[vertNum] = {
		{{ -1,-1,0.0f }, {0.0f,1.0f}}, // 左下
		{{ -1,+1,0.0f }, {0.0f,0.0f}}, // 左上
		{{ +1,-1,0.0f }, {1.0f,1.0f}}, // 右下
		{{ +1,+1,0.0f }, {1.0f,0.0f}}, // 右上
	};

	// 頂点バッファへのデータ転送
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

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
}

void BasePostEffect::Draw(const std::vector<std::string> _name)
{
	// パイプラインの設定
	GraphicsPipelineManager::SetPipeline(cmdList, pipelineName, topologyType);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//シェーダーリソースビュー
	int size = 0;
	size = int(_name.size());
	for (int i = 0; i < size; i++)
	{
		cmdList->SetGraphicsRootDescriptorTable(i + 1, texture[_name[i]].instance->descriptor->gpu);
	}
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}