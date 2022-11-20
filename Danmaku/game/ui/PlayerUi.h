#pragma once
#include "BaseUi.h"

class PlayerUi : public BaseUi
{
private:

	struct GAUGE {
		float maxNum = 0;//このゲージの元になる数値の最大値
		XMFLOAT2 pos = {};//座標
		XMFLOAT2 maxSize = {};//最大の大きさ
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_maxHp">プレイヤー最大HP</param>
	PlayerUi(const int _maxHp);
	~PlayerUi() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_maxHp">プレイヤー最大HP</param>
	/// <returns></returns>
	static std::unique_ptr<PlayerUi> Create(const int _maxHp);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_playerHp">プレイヤーhp</param>
	void Update(const int _playerHp);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	static const int sprite_num = 2;

private:

	//ハートパネル
	std::unique_ptr<Sprite> heartPanel;
	//hpゲージ
	std::array<std::unique_ptr<Sprite>, sprite_num> hp;
	//ゲージの情報
	GAUGE hpGauge;
};
