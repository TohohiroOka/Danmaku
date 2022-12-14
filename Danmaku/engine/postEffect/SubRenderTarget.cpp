#include "SubRenderTarget.h"

using namespace DirectX;

const float SubRenderTarget::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

std::unique_ptr<SubRenderTarget> SubRenderTarget::Create(const DirectX::XMFLOAT2& _window, const std::string& _keepName)
{
	//�C���X�^���X�𐶐�
	SubRenderTarget* instance = new SubRenderTarget();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize(_window, _keepName);

	//�[�x����
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

	//���O�̕ۑ�
	name = _keepName;

	//�e�N�X�`���T�C�Y�w��
	window = _window;

	//�e�N�X�`���o�b�t�@�����p�ϐ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		UINT(window.x), UINT(window.y),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	// �e�N�X�`���p�o�b�t�@�̐���
	Texture* addTexture = new Texture();

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // �e�N�X�`���p�w��
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R16G16B16A16_FLOAT, clearColor),
		IID_PPV_ARGS(&addTexture->texBuffer));
	if (FAILED(result)) { assert(0); }

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV����
	addTexture->descriptor = std::make_unique<DescriptorHeapManager>();
	addTexture->descriptor->CreateSRV(addTexture->texBuffer, srvDesc);

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHescDesc{};
	rtvDescHescDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHescDesc.NumDescriptors = 1;

	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtvDescHescDesc, IID_PPV_ARGS(&descHeapRTV));
	if (FAILED(result)) { assert(0); }

	descHeapRTV->SetName(L"SubRTV");

	//�f�X�N���v�^�q�[�v��RTV����
	device->CreateRenderTargetView(addTexture->texBuffer.Get(), nullptr,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), 0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));

	//�e�N�X�`���Ƃ��ĕۑ�
	Sprite::SetTexture(name, addTexture);
}

void SubRenderTarget::CreateDepthBuffer()
{
	HRESULT result;

	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		UINT(window.x),
		UINT(window.y),
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//�[�x�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));
	if (FAILED(result)) { assert(0); }

	//�[�x�r���[�p�̃f�X�N���v�^�q�[�v����
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	if (FAILED(result)) { assert(0); }

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void SubRenderTarget::PreDrawScene(ID3D12GraphicsCommandList* _cmdList)
{
	//���\�[�X�o���A��ύX
	_cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texture[name].instance->texBuffer.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs;
	rtvHs = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), 0,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	_cmdList->OMSetRenderTargets(1, &rtvHs, false, &dsvH);

	//�r���[�|�[�g�ݒ�
	CD3DX12_VIEWPORT viewports;
	CD3DX12_RECT scissorRects;
	viewports = CD3DX12_VIEWPORT(0.0f, 0.0f, FLOAT(window.x), FLOAT(window.y));
	scissorRects = CD3DX12_RECT(0, 0, LONG(window.x), LONG(window.y));

	_cmdList->RSSetViewports(1, &viewports);
	//�V�U�����O��`�ݒ�
	_cmdList->RSSetScissorRects(1, &scissorRects);

	//�S��ʃN���A
	_cmdList->ClearRenderTargetView(rtvHs, clearColor, 0, nullptr);

	//�[�x�o�b�t�@�̃N���A
	_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void SubRenderTarget::PostDrawScene(ID3D12GraphicsCommandList* _cmdList)
{
	//���\�[�X�o���A��ύX
	_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[name].instance->texBuffer.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}