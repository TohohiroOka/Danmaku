#pragma once
#include "Sprite.h"

class SubRenderTarget : public Sprite
{
public://メンバ関数

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <param name="_window">テクスチャサイズ</param>
	/// <param name="_keepName">保存名</param>
	/// <returns></returns>
	static std::unique_ptr<SubRenderTarget> Create(const DirectX::XMFLOAT2& _window, const std::string& _keepName);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SubRenderTarget();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SubRenderTarget();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_window">テクスチャサイズ</param>
	/// <param name="_keepName">保存名</param>
	virtual void Initialize(const DirectX::XMFLOAT2& _window, const std::string& _keepName);

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDrawScene(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDrawScene(ID3D12GraphicsCommandList* _cmdList);

protected://静的メンバ変数

	//画面クリアカラー
	static const float clearColor[4];

protected://メンバ変数

	//保存名
	std::string name;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuffer;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//テクスチャサイズ
	DirectX::XMFLOAT2 window;

public:

	std::string GetTexName() { return name; }
};
