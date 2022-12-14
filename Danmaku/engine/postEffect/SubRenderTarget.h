#pragma once
#include "Sprite.h"

class SubRenderTarget : public Sprite
{
public://�����o�֐�

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <param name="_window">�e�N�X�`���T�C�Y</param>
	/// <param name="_keepName">�ۑ���</param>
	/// <returns></returns>
	static std::unique_ptr<SubRenderTarget> Create(const DirectX::XMFLOAT2& _window, const std::string& _keepName);

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SubRenderTarget();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SubRenderTarget();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_window">�e�N�X�`���T�C�Y</param>
	/// <param name="_keepName">�ۑ���</param>
	virtual void Initialize(const DirectX::XMFLOAT2& _window, const std::string& _keepName);

	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDrawScene(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDrawScene(ID3D12GraphicsCommandList* _cmdList);

protected://�ÓI�����o�ϐ�

	//��ʃN���A�J���[
	static const float clearColor[4];

protected://�����o�ϐ�

	//�ۑ���
	std::string name;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuffer;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�e�N�X�`���T�C�Y
	DirectX::XMFLOAT2 window;

public:

	std::string GetTexName() { return name; }
};
