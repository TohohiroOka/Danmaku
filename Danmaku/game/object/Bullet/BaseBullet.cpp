#include "BaseBullet.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "GameHelper.h"

using namespace DirectX;

std::unique_ptr<Model> BaseBullet::model;
std::array<std::unique_ptr<InstanceObject>, BaseBullet::object_max_num> BaseBullet::bullet;
int BaseBullet::usingNum = 0;
std::array<std::unique_ptr<Emitter>, BaseBullet::effect_max_num>  BaseBullet::effect;
std::array<int, BaseBullet::effect_max_num> BaseBullet::effectNum;

void BaseBullet::StaticInitialize()
{
	//弾
	model = Model::CreateFromOBJ("Square1");

	for (auto& i : bullet)
	{
		i = InstanceObject::Create(model.get());
		i->SetBloom(true);
		i->SetLight(false);
	}

	ParticleManager::LoadTexture("effect1", "Resources/particle/effect1.png");//汎用エフェクト

	for (auto& i : effect) {
		i = Emitter::Create("effect1");
		i->SetBloom();
	}
}

void BaseBullet::Initialize()
{
	isAlive = true;
	scale = 1.0f;
	rotate = { 0,0,0 };
}

void BaseBullet::StaticUpdate() {
	for (int i = 0; i < effect_max_num; i++) {
		effectNum[i] = effect[i]->GetCreateNum();
	}
}

void BaseBullet::Update()
{
	if (!isAlive) { return; }

	//最大値にいったら生存フラグを消す
	if (pos.x < 0.0f || pos.x>mapSize || pos.y > 710 || pos.y < -1.0f || pos.z < 0.0f || pos.z>mapSize) {
		isAlive = false;
	}

	Ray ray;
	ray.start = { pos.x,pos.y,pos.z,1 };
	ray.dir = { moveVec.x,moveVec.y,moveVec.z,0 };
	RAYCAST_HIT raycastHit;

	//レイの距離
	float distance = sqrtf(move.x * move.x + move.y * move.y + move.z + move.z);

	//カプセル
	Capsule capsule;
	capsule.startPosition = { pos.x,pos.y,pos.z };
	capsule.endPosition = { pos.x + move.x,pos.y + move.y,pos.z + move.z };
	capsule.radius = scale;
	if (CollisionManager::GetInstance()->QueryCapsule(capsule, COLLISION_ATTR_LANDSHAPE))
	{
		isAlive = false;
	}

	XMFLOAT3 bPos = pos;
	pos.x += move.x;
	pos.y += move.y;
	pos.z += move.z;

	for (auto& i : bullet)
	{
		if (!i->GetInstanceDrawCheck()) { continue; }
		i->DrawInstance(pos, { scale,scale,scale }, rotate, { color.x,color.y,color.z,1.0f });
		break;
	}

	for (int i = 0; i < effect_max_num; i++) {
		if (effectNum[i] >= 512) { continue; }
		effect[i]->InEmitter(10, pos, { 0,0,0 }, { 0,0,0 }, 5.0f, 0.0f,
			{ color.x,color.y,color.z,1.0f }, { color.x,color.y,color.z,1.0f });
		effectNum[i]++;
		break;
	}
}

void BaseBullet::Draw()
{
	for (auto& i : bullet) {
		if (i->GetInstanceDrawNum() == 0) { continue; }
		i->Draw();
	}
}

void BaseBullet::EffectDraw()
{
	for (int i = 0; i < effect_max_num; i++) {
		if (effectNum[i] == 0) { continue; }
		effect[i]->Update();
		effect[i]->Draw();
	}
}

void BaseBullet::Finalize()
{
	model.reset();
}