#pragma once
#include "InterfaceScene.h"
#include <array>

class Title : public InterfaceScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	///	更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 更新
	/// </summary>
	void CameraUpdate(Camera* camera) override {};

	/// <summary>
	/// 描画
	/// </summary>
	void DrawNotPostB() override;

	/// <summary>
	///	描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 描画
	/// </summary>
	void DrawNotPostA() override;

	/// <summary>
	///	解放
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// imguiの表示
	/// </summary>
	void ImguiDraw() override;

	/// <summary>
	/// コンピュートシェーダーからの計算結果取得
	/// </summary>
	void GetConstbufferNum() override;

private:

	std::unique_ptr<Sprite> sp;
};