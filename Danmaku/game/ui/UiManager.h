#pragma once
#include "BossHpUi.h"
#include "PlayerUi.h"

class UiManager
{
public:

	UiManager() {};
	~UiManager() {};

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_playerMaxHp">�v���C���[�ő�HP</param>
	/// <param name="_bulletEnergyMax">�o���b�g�G�l���M�[�ő�</param>
	/// <param name="_bossMaxHp">�{�X�ő�HP</param>
	/// <returns></returns>
	static std::unique_ptr<UiManager> Create(const int _playerMaxHp, const float _bulletEnergyMax, const int _bossMaxHp);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_playerMaxHp">�v���C���[�ő�HP</param>
	/// <param name="_bossMaxHp">�{�X�ő�HP</param>
	void Initialize(const int _playerMaxHp, const float _bulletEnergyMax,const int _bossMaxHp);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="_playerHp">�v���C���[HP</param>
	/// <param name="_bulletEnergy">�o���b�g�G�l���M�[</param>
	/// <param name="_bossHp">�{�XHP</param>
	void Update(const int _playerHp, const float _bulletEnergy, const int _bossHp);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	std::unique_ptr<PlayerUi> playerUi;
	std::unique_ptr<BossHpUi> bossHp;
	
};