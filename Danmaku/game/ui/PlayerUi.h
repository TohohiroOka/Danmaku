#pragma once
#include "BaseUi.h"

class PlayerUi : public BaseUi
{
private:

	struct GAUGE {
		float maxNum = 0;//���̃Q�[�W�̌��ɂȂ鐔�l�̍ő�l
		XMFLOAT2 pos = {};//���W
		XMFLOAT2 maxSize = {};//�ő�̑傫��
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_maxHp">�v���C���[�ő�HP</param>
	PlayerUi(const int _maxHp);
	~PlayerUi() {};

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_maxHp">�v���C���[�ő�HP</param>
	/// <returns></returns>
	static std::unique_ptr<PlayerUi> Create(const int _maxHp);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="_playerHp">�v���C���[hp</param>
	void Update(const int _playerHp);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	static const int sprite_num = 2;

private:

	//�n�[�g�p�l��
	std::unique_ptr<Sprite> heartPanel;
	//hp�Q�[�W
	std::array<std::unique_ptr<Sprite>, sprite_num> hp;
	//�Q�[�W�̏��
	GAUGE hpGauge;
};
