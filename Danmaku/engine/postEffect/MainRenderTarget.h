#pragma once
#include "Sprite.h"

class MainRenderTarget : public Sprite
{
public: // �T�u�N���X

	enum TEX_TYPE
	{
		NORMAL,
		BLOOM,
		OUTLINE,
		DEPTH,
		SIZE,
	};

public://�����o�֐�

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	static std::unique_ptr<MainRenderTarget> Create();

	/// <summary>
	/// �������
	/// </summary>
	void Finalize();

public://�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	MainRenderTarget();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~MainRenderTarget() {};

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDrawScene();

private://�ÓI�����o�ϐ�

	//��ʃN���A�J���[
	static const float clearColor[4];
	//���H�p�e�N�X�`����
	static const std::array<std::string, TEX_TYPE::SIZE> texName;

private://�����o�ϐ�

	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};