#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public: // サブクラス

	// 定数バッファ用データ構造体B0
	struct CONST_BUFFER_DATA_POST
	{
		XMFLOAT3 outlineColor;//アウトラインの色
		float outlineWidth;//アウトラインの太さ
		bool isFog;//フォグの有無
		float pad1;
		XMFLOAT2 toe;
		XMFLOAT2 linear;
		XMFLOAT2 shoulder;
	};

	enum TEX_TYPE
	{
		NORMAL,
		BLOOM,
		OUTLINE,
		DEPTH,
		SIZE
	};

public://メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	static std::unique_ptr<PostEffect> Create();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="_cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="_cmdList">描画コマンドリスト</param>
	void PreDrawScene(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	/// <param name="_cmdList">描画コマンドリスト</param>
	void PostDrawScene(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// パイプラインのセット
	/// </summary>
	/// <param name="_pipeline">パイプライン</param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { pipeline = _pipeline; }

private://静的メンバ変数

	//画面クリアカラー
	static const float clearColor[2][4];
	//パイプライン
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;

private://メンバ変数

	//テクスチャ情報
	std::array<std::unique_ptr<Texture>, TEX_TYPE::SIZE> texture;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//フォグ
	bool isFog;
	//トーンマップ用
	XMFLOAT2 toe;
	XMFLOAT2 linear;
	XMFLOAT2 shoulder;

public:

	void SetFog(bool _isFog) {
		isFog = _isFog;
	}

	void SetToe(const XMFLOAT2& _toe) {
		toe = _toe;
	}

	XMFLOAT2 GetToe() {
		return toe;
	}

	void SetLinear(const XMFLOAT2& _linear) {
		linear = _linear;
	}

	XMFLOAT2 GetLinear() {
		return linear;
	}

	void SetShoulder(const XMFLOAT2& _shoulder) {
		shoulder = _shoulder;
	}

	XMFLOAT2 GetShoulder() {
		return shoulder;
	}
};