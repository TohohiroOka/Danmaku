#pragma once
#include "BaseUi.h"

class PlayerUi : public BaseUi
{
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

	//プレイヤーの最大hp
	int maxHp;
	//スプライト
	std::unique_ptr<Sprite> damage;
	//ダメージスプライトのα値
	float damageAlpha;
};
