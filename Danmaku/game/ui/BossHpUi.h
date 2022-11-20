#pragma once
#include "BaseUi.h"

class BossHpUi : public BaseUi
{
private:

	struct GAUGE {
		float maxNum = 0;//このゲージのm元になる数値の最大値
		XMFLOAT2 pos = {};
		XMFLOAT2 maxSize = {};//最大の大きさ
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_maxHp">ボス最大HP</param>
	BossHpUi(const int _maxHp);
	~BossHpUi() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="_maxHp">ボス最大HP</param>
	/// <returns></returns>
	static std::unique_ptr<BossHpUi> Create(const int _maxHp);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_bossHp">ボスhp</param>
	void Update(const int _bossHp);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	static const int sprite_num = 2;

private:

	//スプライト
	std::array<std::unique_ptr<Sprite>, sprite_num> sprite;
	//ゲージの情報
	GAUGE hpGauge;
};
