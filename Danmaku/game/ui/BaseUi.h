#pragma once
#include "Sprite.h"

class BaseUi
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;

protected:

	struct SPRITE_INFO
	{
		std::string name;
		XMFLOAT2 texSize;
	};

	enum class SPRITE {
		BOSS_HP_IN,//ボスhpバー中身
		BOSS_HP_OUT,//ボスhpバー外枠
		PLAYER_HP_DOWN,//プレイヤーhpが少なくなった時のエフェクト
	};

public:

	/// <summary>
	/// 名前を保存する
	/// </summary>
	/// <param name="_name">ファイル名</param>
	/// <param name="_sprite">enum名</param>
	/// <param name="_texSize">テクスチャの大きさ</param>
	static void KeepName(const std::string& _name, const SPRITE _sprite, const XMFLOAT2& _texSize);

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

protected:

	//テクスチャ保存名
	static std::map<SPRITE, SPRITE_INFO> spriteName;

protected:

	//描画フラグ
	bool isDraw;
};