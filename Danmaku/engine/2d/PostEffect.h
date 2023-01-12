#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public: // �T�u�N���X

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct CONST_BUFFER_DATA_POST
	{
		XMFLOAT3 outlineColor;//�A�E�g���C���̐F
		float outlineWidth;//�A�E�g���C���̑���
		bool isFog;//�t�H�O�̗L��
		float pad1;
		XMFLOAT2 toe;
		XMFLOAT2 linear;
		XMFLOAT2 shoulder;
	};

	enum TEX_TYPE
	{
		NORMAL,
		BLOOM,
		OUTLINE,
		DEPTH,
		SIZE
	};

public://�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	static std::unique_ptr<PostEffect> Create();

	/// <summary>
	/// �������
	/// </summary>
	void Finalize();

public://�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PostEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="_cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="_cmdList">�`��R�}���h���X�g</param>
	void PreDrawScene(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	/// <param name="_cmdList">�`��R�}���h���X�g</param>
	void PostDrawScene(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// �p�C�v���C���̃Z�b�g
	/// </summary>
	/// <param name="_pipeline">�p�C�v���C��</param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { pipeline = _pipeline; }

private://�ÓI�����o�ϐ�

	//��ʃN���A�J���[
	static const float clearColor[2][4];
	//�p�C�v���C��
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;

private://�����o�ϐ�

	//�e�N�X�`�����
	std::array<std::unique_ptr<Texture>, TEX_TYPE::SIZE> texture;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�t�H�O
	bool isFog;
	//�g�[���}�b�v�p
	XMFLOAT2 toe;
	XMFLOAT2 linear;
	XMFLOAT2 shoulder;

public:

	void SetFog(bool _isFog) {
		isFog = _isFog;
	}

	void SetToe(const XMFLOAT2& _toe) {
		toe = _toe;
	}

	XMFLOAT2 GetToe() {
		return toe;
	}

	void SetLinear(const XMFLOAT2& _linear) {
		linear = _linear;
	}

	XMFLOAT2 GetLinear() {
		return linear;
	}

	void SetShoulder(const XMFLOAT2& _shoulder) {
		shoulder = _shoulder;
	}

	XMFLOAT2 GetShoulder() {
		return shoulder;
	}
};