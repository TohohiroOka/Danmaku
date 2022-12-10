#pragma once
#include "BasePostEffect.h"

class MainPostEffect : public BasePostEffect
{
private:

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct CONST_BUFFER_DATA
	{
		XMFLOAT4 outlineColor;//�A�E�g���C���̐F
		float outlineWidth;//�A�E�g���C���̑���
		unsigned int isFog;//�t�H�O�̗L��
		unsigned int isToneMap;//�g�[���}�b�v�̗L��
	};

public:

	MainPostEffect();
	~MainPostEffect() {};

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	static std::unique_ptr<MainPostEffect> Create();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private:

	//�t�H�O
	bool isFog;
	//�g�[���}�b�v
	bool isToneMap;

public:

	void SetFog(bool _isFog) {
		isFog = _isFog;
	}

	void SetToneMap(bool _isToneMap) {
		isToneMap = _isToneMap;
	}

};

