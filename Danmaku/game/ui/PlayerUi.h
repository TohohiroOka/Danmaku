#pragma once
#include "BaseUi.h"

class PlayerUi : public BaseUi
{
private:

	static const int sprite_num = 2;

	struct GAUGE {
		std::unique_ptr<Sprite> panelTex;//�p�l��
		std::array<std::unique_ptr<Sprite>, sprite_num> gaugeTex;//�Q�[�W
		float maxNum = 0;//���̃Q�[�W�̌��ɂȂ鐔�l�̍ő�l
		XMFLOAT2 pos = {};//���W
		XMFLOAT2 maxSize = {};//�ő�̑傫��
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_maxHp">�v���C���[�ő�HP</param>
	/// <param name="_bulletEnergyMax">�o���b�g�G�l���M�[</param>
	PlayerUi(const int _maxHp, const float _bulletEnergyMax);
	~PlayerUi() {};

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_maxHp">�v���C���[�ő�HP</param>
	/// <param name="_bulletEnergyMax">�o���b�g�G�l���M�[</param>
	/// <returns></returns>
	static std::unique_ptr<PlayerUi> Create(const int _maxHp, const float _bulletEnergyMax);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="_playerHp">�v���C���[hp</param>
	void Update(const int _playerHp, const float _bulletEnergy);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	//hp�Q�[�W
	GAUGE hpGauge;
	//�G�l���M�[�Q�[�W
	GAUGE energyGauge;

};
