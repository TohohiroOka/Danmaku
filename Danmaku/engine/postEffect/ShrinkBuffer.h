#pragma once
#include "BasePostEffect.h"

class ShrinkBuffer : public BasePostEffect
{
private:
	// 定数バッファ用データ構造体B0
	struct CONST_BUFFER_DATA
	{
		unsigned int isLuminance;//輝度の有無
		float windowRatio;
	};

public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ShrinkBuffer();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ShrinkBuffer() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	//画像サイズ
	float window;
	//輝度算出を行うか
	bool isLuminance;

public:
	
	void SetWindowSize(float _window) { window = _window; }
	void SetLuminance(bool _isLuminance) { isLuminance = _isLuminance; }
};