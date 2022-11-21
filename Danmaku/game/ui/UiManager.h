#pragma once
#include "BossHpUi.h"
#include "PlayerUi.h"

class UiManager
{
public:

	UiManager() {};
	~UiManager() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_playerMaxHp">プレイヤー最大HP</param>
	/// <param name="_bulletEnergyMax">バレットエネルギー最大</param>
	/// <param name="_bossMaxHp">ボス最大HP</param>
	/// <returns></returns>
	static std::unique_ptr<UiManager> Create(const int _playerMaxHp, const float _bulletEnergyMax, const int _bossMaxHp);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_playerMaxHp">プレイヤー最大HP</param>
	/// <param name="_bossMaxHp">ボス最大HP</param>
	void Initialize(const int _playerMaxHp, const float _bulletEnergyMax,const int _bossMaxHp);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_playerHp">プレイヤーHP</param>
	/// <param name="_bulletEnergy">バレットエネルギー</param>
	/// <param name="_bossHp">ボスHP</param>
	void Update(const int _playerHp, const float _bulletEnergy, const int _bossHp);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	std::unique_ptr<PlayerUi> playerUi;
	std::unique_ptr<BossHpUi> bossHp;
	
};