#include "Ground.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "GameHelper.h"
#include "DebugText.h"

 const float Ground::scale = 5.0f;

 Ground::~Ground()
 {
	 terrainModel[0].reset();
	 terrainModel[1].reset();
	 terrainModel[2].reset();
 }

 std::unique_ptr<Ground> Ground::Create()
{
	// オブジェクトのインスタンスを生成
	Ground* instance = new Ground();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
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
	DirectX::XMFLOAT2 cubeScaleAPos = { mapSizeX / 2.0f,mapSizeZ / 2.0f };
	cubeObject[0]->SetScale({ cubeScaleAPos.x,cubeScaleAPos.x / 3.0f,cubeScaleAPos.y });
	cubeObject[0]->SetPosition({ cubeScaleAPos.x ,cubeScaleAPos.x / 3.0f - 1.0f ,cubeScaleAPos.y });
	cubeObject[0]->SetLight(false);

	//外
	cubeObject[1] = Object3d::Create(cubeModel[1].get());
	cubeObject[1]->SetScale({ mapSizeX,mapSizeX / 3.0f,mapSizeZ });
	cubeObject[1]->SetPosition({ cubeScaleAPos.x ,mapSizeX / 3.0f - 1.0f,cubeScaleAPos.y });
	cubeObject[1]->SetLight(false);

	//モデル読み込み
	const std::string jimen = "jimen.png";
	const std::string kabe = "kabe.png";
	mapName[0] = "heightmap02.bmp";
	mapName[1] = "heightmap01.bmp";
	mapName[2] = "heightmap03.bmp";	
	mapName[3] = "heightmap04.bmp";
	useModel= TerrainModel::FlatlandModelCreate(jimen, kabe);
	terrainModel[0] = TerrainModel::FlatlandModelCreate(jimen, kabe);
	for (int i = 0; i < modelNum; i++) {
		int tModel = i + 1;
		terrainModel[tModel] = TerrainModel::Create(mapName[i], jimen, kabe);
	}
	//マップ生成
	object = HeightMap::Create(useModel.get());

	//マップ情報変更
	object->SetScale({ scale,scale,scale });

	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	object->SetCollider(collider);
	//collider->ConstructTriangles(object->GetModel());
	collider->ConstructTriangles(object->GetHitVertices(), object->GetHitIndices());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);

	//マップ変更フラグ
	//初回にマップを変更する
	isChangeMap = true;
	//衝突判定の変更
	isHitChange = true;
	//変化の割合
	ratio = 0.0f;
	//変更するマップ指定
	ChangeMapKind = { 0,1 };
}

void Ground::Update()
{
	DebugText* text = DebugText::GetInstance();
	std::string strMapNumber1 = std::to_string(ChangeMapKind[0]);
	std::string strMapNumber2 = std::to_string(ChangeMapKind[1]);
	std::string strIsHitChange = std::to_string(isHitChange);

	text->Print("MapNumber :: 1:" + strMapNumber1 + " MapNumber :: 2:" + strMapNumber2, 800, 100);
	text->Print("isHitChange : " + strIsHitChange, 800, 150);
	text = nullptr;

	if (!isChangeMap) { return; }

	object->SetChangeModel(terrainModel[ChangeMapKind[0]].get(), terrainModel[ChangeMapKind[1]].get(), ratio);

	isChangeMap = false;

	//if (!isHitChange) { return; }
	//現在のコライダー削除
	object->DeleteCollider();

	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	object->SetCollider(collider);
	//collider->ConstructTriangles(object->GetModel());
	collider->ConstructTriangles(terrainModel[ChangeMapKind[1]]->GetHitVertices(), terrainModel[ChangeMapKind[1]]->GetHitIndices());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);

	if (!isHitChange) { return; }
	for (auto& i : ChangeMapKind) {
		i++;
		if (i >= terrainModel.size()) {
			i = 1;
		}
	}

	isHitChange = false;
}

void Ground::Draw()
{
	object->AddConstBufferUpdate();
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