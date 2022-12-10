#pragma once
#include "BasePostEffect.h"

class MainPostEffect : public BasePostEffect
{
private:

	// 定数バッファ用データ構造体B0
	struct CONST_BUFFER_DATA
	{
		XMFLOAT4 outlineColor;//アウトラインの色
		float outlineWidth;//アウトラインの太さ
		unsigned int isFog;//フォグの有無
		unsigned int isToneMap;//トーンマップの有無
	};

public:

	MainPostEffect();
	~MainPostEffect() {};

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	static std::unique_ptr<MainPostEffect> Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	//フォグ
	bool isFog;
	//トーンマップ
	bool isToneMap;

public:

	void SetFog(bool _isFog) {
		isFog = _isFog;
	}

	void SetToneMap(bool _isToneMap) {
		isToneMap = _isToneMap;
	}

};

