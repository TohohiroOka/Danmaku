#include "Object3d.h"

#include "BaseCollider.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Model.h"
#include "Texture.h"
#include "GraphicsPipelineManager.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;

std::unique_ptr<Object3d> Object3d::Create(Model* _model)
{
	// 3Dオブジェクトのインスタンスを生成
	Object3d* instance = new Object3d();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	if (_model) {
		instance->SetModel(_model);
	}

	return std::unique_ptr<Object3d>(instance);
}

Object3d::~Object3d()
{
	InterfaceObject3d::~InterfaceObject3d();
}

void Object3d::Initialize()
{
	// nullptrチェック
	assert(device);

	name = typeid(*this).name();

	InterfaceObject3d::Initialize();
}

void Object3d::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Object3d::cmdList);

	// パイプラインの設定
	GraphicsPipelineManager* graPipManeger = GraphicsPipelineManager::GetInstance();
	graPipManeger->SetPipeline(cmdList, "OBJ", topologyType);
	graPipManeger = nullptr;

	// モデルの割り当てがなければ描画しない
	if (model == nullptr) {
		return;
	}

	InterfaceObject3d::Draw();

	// モデル描画
	model->Draw(cmdList);
}