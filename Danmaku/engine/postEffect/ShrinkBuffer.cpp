#include "ShrinkBuffer.h"

using namespace DirectX;

ShrinkBuffer::ShrinkBuffer()
{
	isLuminance = false;
	//������
	this->Initialize();
}

void ShrinkBuffer::Initialize()
{
	BasePostEffect::Initialize();

	// �萔�o�b�t�@�̐���
	HRESULT result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }
}

void ShrinkBuffer::Update()
{
	//�萔�o�b�t�@�Ƀf�[�^��]��
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	if (SUCCEEDED(result)) {
		constMap->windowRatio = window;
		constMap->isLuminance = isLuminance;
		constBuff->Unmap(0, nullptr);
	}
}
