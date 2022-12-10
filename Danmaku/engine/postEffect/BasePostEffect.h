#pragma once
#include "Sprite.h"

class BasePostEffect : public Sprite
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BasePostEffect();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BasePostEffect() {};

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="_name">�e�N�X�`���ۑ���</param>
	void Draw(const std::vector<std::string> _name);
};
