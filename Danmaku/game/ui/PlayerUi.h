#pragma once
#include "BaseUi.h"

class PlayerUi : public BaseUi
{
private:

	static const int sprite_num = 2;

	struct GAUGE {
		std::unique_ptr<Sprite> panelTex;//パネル
		std::array<std::unique_ptr<Sprite>, sprite_num> gaugeTex;//ゲージ
		float maxNum = 0;//このゲージの元になる数値の最大値
		XMFLOAT2 pos = {};//座標
		XMFLOAT2 maxSize = {};//最大の大きさ
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_maxHp">プレイヤー最大HP</param>
	/// <param name="_bulletEnergyMax">バレットエネルギー</param>
	PlayerUi(const int _maxHp, const float _bulletEnergyMax);
	~PlayerUi() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_maxHp">プレイヤー最大HP</param>
	/// <param name="_bulletEnergyMax">バレットエネルギー</param>
	/// <returns></returns>
	static std::unique_ptr<PlayerUi> Create(const int _maxHp, const float _bulletEnergyMax);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_playerHp">プレイヤーhp</param>
	void Update(const int _playerHp, const float _bulletEnergy);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//hpゲージ
	GAUGE hpGauge;
	//エネルギーゲージ
	GAUGE energyGauge;

};
