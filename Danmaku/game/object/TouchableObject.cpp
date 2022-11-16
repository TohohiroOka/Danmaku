#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

std::unique_ptr<TouchableObject> TouchableObject::Create(Model* model)
{
	// オブジェクトのインスタンスを生成
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return std::unique_ptr<TouchableObject>(instance);
}

bool TouchableObject::Initialize(Model* model)
{
	Object3d::Initialize();

	SetModel(model);

	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTriangles(model);
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);

	SetScale(50.0f);

	return true;
}
