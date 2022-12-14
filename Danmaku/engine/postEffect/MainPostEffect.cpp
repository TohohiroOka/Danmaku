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
	//�C���X�^���X�𐶐�
	MainPostEffect* instance = new MainPostEffect();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize();

	return std::unique_ptr<MainPostEffect>(instance);
}

void MainPostEffect::Initialize()
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

	constBuff->SetName(L"MAINPOST_CONST");
}

void MainPostEffect::Update()
{
	// �萔�o�b�t�@�փf�[�^�]��
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
