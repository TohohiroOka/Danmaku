#pragma once
#include "BaseUi.h"

class BossHpUi : public BaseUi
{
private:

	struct GAUGE {
		float maxNum = 0;//���̃Q�[�W��m���ɂȂ鐔�l�̍ő�l
		XMFLOAT2 pos = {};
		XMFLOAT2 maxSize = {};//�ő�̑傫��
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_maxHp">�{�X�ő�HP</param>
	BossHpUi(const int _maxHp);
	~BossHpUi() {};

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_maxHp">�{�X�ő�HP</param>
	/// <returns></returns>
	static std::unique_ptr<BossHpUi> Create(const int _maxHp);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="_bossHp">�{�Xhp</param>
	void Update(const int _bossHp);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	static const int sprite_num = 2;

private:

	//�X�v���C�g
	std::array<std::unique_ptr<Sprite>, sprite_num> sprite;
	//�Q�[�W�̏��
	GAUGE hpGauge;
};
