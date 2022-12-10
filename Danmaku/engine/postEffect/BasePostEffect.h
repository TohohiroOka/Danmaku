#pragma once
#include "Sprite.h"

class BasePostEffect : public Sprite
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BasePostEffect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BasePostEffect() {};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_name">テクスチャ保存名</param>
	void Draw(const std::vector<std::string> _name);
};
