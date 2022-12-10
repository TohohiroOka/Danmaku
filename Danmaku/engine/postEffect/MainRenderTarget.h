#pragma once
#include "Sprite.h"

class MainRenderTarget : public Sprite
{
public: // サブクラス

	enum TEX_TYPE
	{
		NORMAL,
		BLOOM,
		OUTLINE,
		DEPTH,
		SIZE,
	};

public://メンバ関数

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	static std::unique_ptr<MainRenderTarget> Create();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MainRenderTarget();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MainRenderTarget() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDrawScene();

private://静的メンバ変数

	//画面クリアカラー
	static const float clearColor[4];
	//加工用テクスチャ名
	static const std::array<std::string, TEX_TYPE::SIZE> texName;

private://メンバ変数

	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};