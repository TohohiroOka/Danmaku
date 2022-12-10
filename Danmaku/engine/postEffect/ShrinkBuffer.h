#pragma once
#include "BasePostEffect.h"

class ShrinkBuffer : public BasePostEffect
{
private:
	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct CONST_BUFFER_DATA
	{
		unsigned int isLuminance;//�P�x�̗L��
		float windowRatio;
	};

public://�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ShrinkBuffer();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ShrinkBuffer() {};

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private:

	//�摜�T�C�Y
	float window;
	//�P�x�Z�o���s����
	bool isLuminance;

public:
	
	void SetWindowSize(float _window) { window = _window; }
	void SetLuminance(bool _isLuminance) { isLuminance = _isLuminance; }
};