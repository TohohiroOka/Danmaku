#pragma once
#include "BasePostEffect.h"

class MainPostEffect : public BasePostEffect
{
private:

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct CONST_BUFFER_DATA
	{
		XMFLOAT2 toeColor;//�g�[���}�b�v�p�̐F�w��
		XMFLOAT2 linearColor;//�g�[���}�b�v�p�̐F�w��
		XMFLOAT4 outlineColor;//�A�E�g���C���̐F
		float outlineWidth;//�A�E�g���C���̑���
		unsigned int isFog;//�t�H�O�̗L��
		unsigned int isToneMap;//�g�[���}�b�v�̗L��
	};

public:

	MainPostEffect();
	~MainPostEffect();

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

	//�g�[���}�b�v�p�̐F�w��
	XMFLOAT2 toeColor;
	//�g�[���}�b�v�p�̐F�w��
	XMFLOAT2 linearColor;
	//�t�H�O
	bool isFog;
	//�g�[���}�b�v
	bool isToneMap;

public:

	XMFLOAT2 GetToeColor() {
		return toeColor;
	}

	void SetToeColor(const XMFLOAT2& _toeColor) {
		toeColor = _toeColor;
	}

	XMFLOAT2 GetLinearColor() {
		return linearColor;
	}

	void SetLinearColor(const XMFLOAT2& _LinearColor) {
		linearColor = _LinearColor;
	}

	void SetFog(bool _isFog) {
		isFog = _isFog;
	}

	void SetToneMap(bool _isToneMap) {
		isToneMap = _isToneMap;
	}

};

