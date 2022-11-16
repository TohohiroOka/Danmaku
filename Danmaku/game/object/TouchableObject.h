#pragma once
#include "Object3d.h"

class TouchableObject : public Object3d
{
public:
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<TouchableObject> Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model * model);

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	inline void SetScale(float scale) { this->scale = { scale,scale,scale }; }
	void SetScale(XMFLOAT3 scale) = delete;
};

