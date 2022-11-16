#include "BaseBeam.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "GameHelper.h"

using namespace DirectX;

std::unique_ptr<Model> BaseBeam::model;
std::array<BaseBeam::OBEJCT, BaseBeam::object_max_num> BaseBeam::object;

void BaseBeam::StaticInitialize()
{
	//’e
	model = Model::CreateFromOBJ("Square1");

	for (auto& i : object)
	{
		i.in = InstanceObject::Create(model.get());
		i.in->SetBloom(true);
		i.in->SetLight(false);
		i.out = InstanceObject::Create(model.get());
		i.out->SetBloom(true);
		i.out->SetLight(false);
	}
}

void BaseBeam::Initialize()
{
	isAlive = true;
	float inSize = mapSize / 1000.0f;
	float outSize = mapSize / 700.0f;
	inScale = { inSize,inSize,mapSize / 2.0f };
	outScale = { outSize,outSize,mapSize / 2.0f };
}

void BaseBeam::Update()
{
	pos.x += move.x;
	pos.y += move.y;
	pos.z += move.z;

	for (auto& i : object)
	{
		if (!i.out->GetInstanceDrawCheck()) { continue; }
		i.in->DrawInstance(pos, inScale, rotate, color);
		i.out->DrawInstance(pos, outScale, rotate, color);
		return;
	}
}

void BaseBeam::Draw()
{
	for (auto& i : object) {
		if (i.out->GetInstanceDrawNum() == 0) { continue; }
		i.in->Draw();
		i.out->Draw();
	}
}

void BaseBeam::Finalize()
{
	model.reset();
}