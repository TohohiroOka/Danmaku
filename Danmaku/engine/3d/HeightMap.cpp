#include "HeightMap.h"
#include "Camera.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "GraphicsPipelineManager.h"

using namespace Microsoft::WRL;
using namespace DirectX;

std::unique_ptr<HeightMap> HeightMap::Create(TerrainModel* _model)
{
	//�C���X�^���X�𐶐�
	HeightMap* instance = new HeightMap();
	if (instance == nullptr) {
		return nullptr;
	}

	instance->SetModel(_model);

	//������
	instance->Initialize();

	return std::unique_ptr<HeightMap>(instance);
}

void HeightMap::Initialize()
{
	HRESULT result = S_FALSE;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
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
	result = constBufferOData->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	if (SUCCEEDED(result)) {
		constMap->ratio = _ratio;
		constBufferOData->Unmap(0, nullptr);
	}
}

void HeightMap::Draw()
{
	// �p�C�v���C���̐ݒ�
	GraphicsPipelineManager::SetPipeline(cmdList, "HEIGHT_MAP", topologyType);

	InterfaceObject3d::Draw();

	cmdList->SetGraphicsRootConstantBufferView(3, constBufferOData->GetGPUVirtualAddress());

	model->Draw(cmdList);
}

void HeightMap::SetChangeModel(TerrainModel* _beforeModel, TerrainModel* afterModel, const float _ratio)
{
	//�`��p���_
	{
		std::vector<Mesh::VERTEX> verticesC = model->GetModel()->GetMeshes()[0]->GetVertices();

		const std::array<std::vector<Mesh::VERTEX>, 2> vertices = {
			_beforeModel->GetModel()->GetMeshes()[0]->GetVertices(),
			afterModel->GetModel()->GetMeshes()[0]->GetVertices() };

		int i = 0;
		Mesh& nowMesh = *model->GetModel()->GetMeshes()[0];

		for (auto& vertex : verticesC)
		{
			vertex.pos.y = Easing::Lerp(vertices[0][i].pos.y, vertices[1][i].pos.y, _ratio);
			vertex.normal = Easing::Lerp(vertices[0][i].normal, vertices[1][i].normal, _ratio);

			nowMesh.ChangeVertex(vertex, i);
			i++;
		}
		nowMesh.SendVertex();
	}

	//����p���_
	{
		std::vector<Mesh::VERTEX> verticesC = model->GetHitVertices();

		const std::array<std::vector<Mesh::VERTEX>, 2> vertices = {
			_beforeModel->GetHitVertices(),
			afterModel->GetHitVertices() };

		int i = 0;

		for (auto& vertex : verticesC)
		{
			vertex.pos.y = Easing::Lerp(vertices[0][i].pos.y, vertices[1][i].pos.y, _ratio);
			vertex.normal = Easing::Lerp(vertices[0][i].normal, vertices[1][i].normal, _ratio);

			model->SetHitVertices(vertex, i);
			i++;
		}
	}
}
