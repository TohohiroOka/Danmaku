#include "Ground.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "GameHelper.h"

 const float Ground::scale = 5.0f;

std::unique_ptr<Ground> Ground::Create(const std::string& heightmapFilename,
	const std::string& filename1, const std::string& filename2)
{
	// オブジェクトのインスタンスを生成
	Ground* instance = new Ground();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->object = HeightMap::Create(heightmapFilename, filename1, filename2);
	instance->Initialize();

	return std::unique_ptr<Ground>(instance);
}

void Ground::Initialize()
{
	//キューブ
	cubeModel[0] = Model::CreateFromOBJ("Cube");
	cubeModel[1] = Model::CreateFromOBJ("Cube2");

	//中
	cubeObject[0] = Object3d::Create(cubeModel[0].get());
	float cubeScaleAPos = mapSize / 2.0f;
	cubeObject[0]->SetScale({ cubeScaleAPos,cubeScaleAPos / 3.0f,cubeScaleAPos });
	cubeObject[0]->SetPosition({ cubeScaleAPos ,cubeScaleAPos / 3.0f - 1.0f ,cubeScaleAPos });
	cubeObject[0]->SetLight(false);

	//外
	cubeObject[1] = Object3d::Create(cubeModel[1].get());
	cubeObject[1]->SetScale({ mapSize,mapSize / 3.0f,mapSize });
	cubeObject[1]->SetPosition({ cubeScaleAPos ,mapSize / 3.0f - 1.0f,cubeScaleAPos });
	cubeObject[1]->SetLight(false);

	//マップ
	object->SetScale({ scale,scale,scale });

	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	object->SetCollider(collider);
	//collider->ConstructTriangles(object->GetModel());
	collider->ConstructTriangles(object->GetHitVertices(), object->GetHitIndices());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
}

void Ground::Draw()
{
	object->AddConstBufferUpdate(ratio);
	object->Draw();
}

void Ground::CDraw()
{
	object->ColliderDraw();
}

void Ground::CubeDraw()
{
	for (auto& i : cubeObject) {
		i->Draw();
	}
}