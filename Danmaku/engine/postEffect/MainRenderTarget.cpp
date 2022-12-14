#include "MainRenderTarget.h"
#include "WindowApp.h"
#include "InterfaceObject3d.h"

using namespace DirectX;

const float MainRenderTarget::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
const std::array<std::string, MainRenderTarget::TEX_TYPE::SIZE> MainRenderTarget::texName = {
	"NORMAL",
	"BLOOM",
	"OUTLINE",
	"DEPTH",
};

MainRenderTarget::MainRenderTarget()
	:Sprite()
{
}

MainRenderTarget::~MainRenderTarget()
{
	Sprite::~Sprite();
	descHeapRTV.Reset();
	descHeapDSV.Reset();
}

void MainRenderTarget::Initialize()
{
	HRESULT result;

	//�e�N�X�`���o�b�t�@�����p�ϐ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		WindowApp::GetWindowWidth(), WindowApp::GetWindowHeight(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	//�[�x�ȊO�̃e�N�X�`������
	const int break_num = TEX_TYPE::SIZE - 1;

	for (int i = 0; i < break_num; i++)
	{
		// �e�N�X�`���p�o�b�t�@�̐���
		Texture* addTexture = new Texture();

		result = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // �e�N�X�`���p�w��
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R16G16B16A16_FLOAT, clearColor),
			IID_PPV_ARGS(&addTexture->texBuffer));
		if (FAILED(result)) { assert(0); }

		//�e�N�X�`���Ƃ��ĕۑ�
		Sprite::SetTexture(texName[i], addTexture);
	}

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < break_num; i++)
	{
		//�f�X�N���v�^�q�[�v��SRV����
		texture[texName[i]].instance->descriptor = std::make_unique<DescriptorHeapManager>();
		texture[texName[i]].instance->descriptor->CreateSRV(texture[texName[i]].instance->texBuffer, srvDesc);
	}

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHescDesc{};
	rtvDescHescDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHescDesc.NumDescriptors = break_num;

	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtvDescHescDesc, IID_PPV_ARGS(&descHeapRTV));
	if (FAILED(result)) { assert(0); }

	descHeapRTV->SetName(L"MainRTV");

	//�f�X�N���v�^�q�[�v��RTV����
	for (int i = 0; i < break_num; i++)
	{
		device->CreateRenderTargetView(texture[texName[i]].instance->texBuffer.Get(), nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}
}

void MainRenderTarget::CreateDepthBuffer()
{
	HRESULT result;

	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32_TYPELESS,
			WindowApp::GetWindowWidth(),
			(UINT)WindowApp::GetWindowHeight(),
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// �e�N�X�`���p�o�b�t�@�̐���
	Texture* addTexture = new Texture();

	//�[�x�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&addTexture->texBuffer));
	if (FAILED(result)) { assert(0); }

	//DSV�p�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��

	//�f�X�N���v�^�q�[�v��DSV����
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	if (FAILED(result)) { assert(0); }

	//�[�x�o�b�t�@���\�[�X�ݒ�
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(addTexture->texBuffer.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�e�N�X�`���Ƃ��ĕۑ�
	Sprite::SetTexture(texName[TEX_TYPE::DEPTH], addTexture);

	//�f�X�N���v�^�q�[�v��SRV����
	texture[texName[TEX_TYPE::DEPTH]].instance->descriptor = std::make_unique<DescriptorHeapManager>();
	texture[texName[TEX_TYPE::DEPTH]].instance->descriptor->CreateSRV(
		texture[texName[TEX_TYPE::DEPTH]].instance->texBuffer, srvDesc);
}

std::unique_ptr<MainRenderTarget> MainRenderTarget::Create()
{
	// Sprite�̃C���X�^���X�𐶐�
	MainRenderTarget* instance = new MainRenderTarget();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize();
	//�[�x�o�b�t�@����
	instance->CreateDepthBuffer();

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<MainRenderTarget>(instance);
}

void MainRenderTarget::PreDrawScene()
{
	const int break_num = TEX_TYPE::SIZE - 1;

	//���\�[�X�o���A��ύX
	for (int i = 0; i < break_num; i++)
	{
		cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texture[texName[i]].instance->texBuffer.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
	}

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[break_num];
	for (int i = 0; i < break_num; i++)
	{
		if (break_num == i) { break; }
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(break_num, rtvHs, false, &dsvH);

	//�r���[�|�[�g�ݒ�
	CD3DX12_VIEWPORT viewports[break_num];
	CD3DX12_RECT scissorRects[break_num];
	for (int i = 0; i < break_num; i++)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, FLOAT(WindowApp::GetWindowWidth()), FLOAT(WindowApp::GetWindowHeight()));
		scissorRects[i] = CD3DX12_RECT(0, 0, LONG(WindowApp::GetWindowWidth()), LONG(WindowApp::GetWindowHeight()));
	}

	cmdList->RSSetViewports(1, viewports);
	//�V�U�����O��`�ݒ�
	cmdList->RSSetScissorRects(1, scissorRects);

	//�S��ʃN���A
	for (int i = 0; i < break_num; i++)
	{
		cmdList->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}

	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MainRenderTarget::PostDrawScene()
{
	const int break_num = TEX_TYPE::SIZE - 1;

	//���\�[�X�o���A��ύX
	for (int i = 0; i < break_num; i++)
	{
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[texName[i]].instance->texBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}