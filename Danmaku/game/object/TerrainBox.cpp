#include "TerrainBox.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "DebugText.h"

#include "GameHelper.h"
#include "BulletManager.h"

const float TerrainBox::box_scale = 5.0f;

TerrainBox::~TerrainBox()
{

}

std::unique_ptr<TerrainBox> TerrainBox::Create()
{
	// オブジェクトのインスタンスを生成
	TerrainBox* instance = new TerrainBox();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<TerrainBox>(instance);
}

void TerrainBox::Initialize()
{
	//モデル読み込み
	const std::string jimen = "jimen.png";
	const std::string kabe = "kabe.png";
	terrainModel[0] = TerrainModel::FlatlandModelCreate(jimen, kabe);
	std::string mapName="heightmap";
	//使用する地形の生成
	for (int i = 1; i < model_num; i++) {
		terrainModel[i] = TerrainModel::Create(mapName + std::to_string(i) + ".bmp", jimen, kabe);
	}

	//地形ごとの座標
	std::array<XMFLOAT3, box_surface_num> surfacePos = {
		XMFLOAT3{0,0,0},{1275,1275,0},{0,0,1275},{0,1275,0},{1275,0,0},{0,1275,0}
	};

	//地形ごとの角度
	std::array<XMFLOAT3, box_surface_num> surfaceRota = {
		XMFLOAT3{0,0,0},{0,0,180},{-90,0,0},{90,0,0},{0,0,90},{0,0,-90}
	};

	//地形ごとの攻撃方向
	std::array<XMFLOAT3, box_surface_num> surfaceDirection = {
		XMFLOAT3{0,-1,0},{0,1,0},{0,0,-1},{0,0,1},{-1,0,0},{1,0,0}
	};

	//ボックスの面用に平面生成
	for (int i = 0; i < box_surface_num; i++) {
		surface[i].model = TerrainModel::FlatlandModelCreate(jimen, kabe);
		surface[i].object = HeightMap::Create(surface[i].model.get());
		surface[i].state = SURFACE_STATE::STOP;
		surface[i].attackDirection = surfaceDirection[i];
		surface[i].changeModelNum[0] = MODEL_NUMBER::FLAT;
		surface[i].changeModelNum[1] = MODEL_NUMBER::FLAT;
		if (i % 2 == 0) {
			surface[i].reverseSurface = i + 1;
		} else {
			surface[i].reverseSurface = i - 1;
		}
		//マップ情報変更
		surface[i].object->SetPosition(surfacePos[i]);
		surface[i].object->SetScale({ box_scale,box_scale,box_scale });
		surface[i].object->SetRotation(surfaceRota[i]);

		// コライダーの追加
		MeshCollider* collider = new MeshCollider;
		surface[i].object->SetCollider(collider);
		collider->ConstructTriangles(surface[i].object->GetHitVertices(), surface[i].object->GetHitIndices());
		collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	}

	SetChangeTerrain();

	//タイマー
	timer = 0;

	//現在の地形の状態
	phase = PHASE::CHANGE_TERRAIN;
}

void TerrainBox::Update()
{
	timer++;

	//攻撃
	if (phase == PHASE::ATTACK) {
		
		Attack();
	}
	//地形変化
	else if (phase == PHASE::CHANGE_TERRAIN) {
		ChangeTerrain();
	}

	//行動の更新
	if (timer == 0) {
		SetChangeTerrain();
	}

	DebugText* text = DebugText::GetInstance();
	std::string strtimer = std::to_string(timer);
	text->Print("timer : " + strtimer, 100, 100);
	text = nullptr;

}

void TerrainBox::Draw()
{
	for(auto& itr : surface)
	{
		itr.object->AddConstBufferUpdate();
		itr.object->Draw();
	}
}

void TerrainBox::HitDraw()
{
	for (auto& itr : surface)
	{
		itr.object->ColliderDraw();
	}
}

void TerrainBox::SetCollider(TERRAIN_INFO& _surface)
{
	//元の判定消去
	_surface.object->DeleteCollider();

	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	_surface.object->SetCollider(collider);
	//collider->ConstructTriangles(object->GetModel());
	collider->ConstructTriangles(_surface.object->GetHitVertices(), _surface.object->GetHitIndices());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
}

void TerrainBox::SetChangeTerrain()
{
	//攻撃面
	const int changeTerrain = Randomint(box_surface_num - 1);
	surface[changeTerrain].changeModelNum[1] = Randomint(MODEL_NUMBER::ATTACK - 1) + 1;
	surface[changeTerrain].state = SURFACE_STATE::CHANGE_ATTACK;
	//防御面
	int defense = -1;
	do {
		defense = Randomint(model_num - 1) + 1;
	} while (surface[changeTerrain].reverseSurface == defense);

	int range= MODEL_NUMBER::DEFENSE - MODEL_NUMBER::ATTACK;
	surface[defense].changeModelNum[1] = Randomint(range - 1) + MODEL_NUMBER::ATTACK - 1;
	surface[defense].state = SURFACE_STATE::CHANGE_DEFENSE;
}

void TerrainBox::ChangeTerrain()
{
	//地形変更の時間
	const float maxTime = 100;

	//地形変更
	for (auto& itr : surface)
	{
		//チェンジフラグがtrue
		if (itr.state != SURFACE_STATE::CHANGE_ATTACK &&
			itr.state != SURFACE_STATE::CHANGE_DEFENSE) {continue;}

		//地形変化
		itr.object->SetChangeDrawModel(terrainModel[itr.changeModelNum[0]].get(),
			terrainModel[itr.changeModelNum[1]].get(), float(timer) / maxTime);

		//衝突判定変化
		if (timer % 10 == 0) {
			//判定用頂点変換
			itr.object->SetChangeHitModel(terrainModel[itr.changeModelNum[0]].get(),
				terrainModel[itr.changeModelNum[1]].get(), float(timer) / maxTime);

			SetCollider(itr);
		}

		//変化し終えたら
		if (timer < maxTime) { continue; }

		//判定用頂点変換
		itr.object->SetChangeHitModel(terrainModel[itr.changeModelNum[0]].get(),
			terrainModel[itr.changeModelNum[1]].get(), 1.0f);

		SetCollider(itr);

		//状態を変更
		//攻撃用
		if(itr.state == SURFACE_STATE::CHANGE_ATTACK){
			itr.state = SURFACE_STATE::ATTACK;
		}
		//防御用
		else if (itr.state == SURFACE_STATE::CHANGE_DEFENSE) {
			itr.state = SURFACE_STATE::DEFENSE;
		}
		itr.changeModelNum[0] = itr.changeModelNum[1];
	}

	//変化し終えたら
	if (timer < maxTime) { return; }
	phase = PHASE::ATTACK;
	timer = 0;
}

void TerrainBox::Attack()
{
	//長方形弾
	for (auto& itr : surface)
	{
		//攻撃する面かどうか
		if (itr.state != SURFACE_STATE::ATTACK) { continue; }

		//初期座標
		XMFLOAT3 initPos={};

		//座標入れ
		//x座標
		if (itr.attackDirection.x != 0) {
			if(itr.attackDirection.x == 1){
				initPos.x = 0;
			} else {
				initPos.x = mapSize;
			}
			initPos.y = Randomfloat(mapSize);
			initPos.z = Randomfloat(mapSize);
		}
		//y座標
		else if (itr.attackDirection.y != 0) {
			if (itr.attackDirection.y == 1) {
				initPos.y = 0;
			} else {
				initPos.y = mapSize;
			}
			initPos.x = Randomfloat(mapSize);
			initPos.z = Randomfloat(mapSize);
		}
		//z座標
		else if (itr.attackDirection.z != 0) {
			if (itr.attackDirection.z == 1) {
				initPos.z = 0;
			} else {
				initPos.z = mapSize;
			}
			initPos.x = Randomfloat(mapSize);
			initPos.y = Randomfloat(mapSize);
		}


		const int maxBullet = 50;
		for (int i = 0; i < maxBullet; i++) {

			BulletManager::SetBossBulletNormal(initPos, itr.attackDirection,
				10.0f, { 0.8f,0.2f,0.8f });

			//縦or横長に弾が伸びて撃たれる
			if (initPos.x != 0) {
				initPos.y++;
			} else if (initPos.y != 0) {
				initPos.z++;
			} else{
				initPos.x++;
			}
		}
	}
}
