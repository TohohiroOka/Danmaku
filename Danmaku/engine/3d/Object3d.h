#pragma once
#include "InterfaceObject3d.h"

class BaseCollider;
class Camera;
class LightGroup;

class Object3d : public InterfaceObject3d
{
public: // 静的メンバ関数

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="_model">モデル</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<Object3d> Create(Model* _model = nullptr);

	/// <summary>
	/// パイプラインのセット
	/// </summary>
	/// <param name="_name">パイプライン名</param>
	void SetPipeline(const std::string& _name) { pipelineName = _name; }

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

protected: // メンバ変数

	// 名前
	const char* name = nullptr;
	// モデル
	Model* model = nullptr;
	//パイプライン名
	std::string pipelineName;

public:

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* _model) { this->model = _model; }

	/// <summary>
	/// モデルを取得
	/// </summary>
	/// <returns>モデル</returns>
	inline Model* GetModel() { return model; }
};