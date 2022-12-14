#pragma once
#include "BasePostEffect.h"

class MainPostEffect : public BasePostEffect
{
private:

	// 定数バッファ用データ構造体B0
	struct CONST_BUFFER_DATA
	{
		XMFLOAT2 toeColor;//トーンマップ用の色指定
		XMFLOAT2 linearColor;//トーンマップ用の色指定
		XMFLOAT4 outlineColor;//アウトラインの色
		float outlineWidth;//アウトラインの太さ
		unsigned int isFog;//フォグの有無
		unsigned int isToneMap;//トーンマップの有無
	};

public:

	MainPostEffect();
	~MainPostEffect();

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

	//トーンマップ用の色指定
	XMFLOAT2 toeColor;
	//トーンマップ用の色指定
	XMFLOAT2 linearColor;
	//フォグ
	bool isFog;
	//トーンマップ
	bool isToneMap;

public:

	XMFLOAT2 GetToeColor() {
		return toeColor;
	}

	void SetToeColor(const XMFLOAT2& _toeColor) {
		toeColor = _toeColor;
	}

	XMFLOAT2 GetLinearColor() {
		return linearColor;
	}

	void SetLinearColor(const XMFLOAT2& _LinearColor) {
		linearColor = _LinearColor;
	}

	void SetFog(bool _isFog) {
		isFog = _isFog;
	}

	void SetToneMap(bool _isToneMap) {
		isToneMap = _isToneMap;
	}

};

