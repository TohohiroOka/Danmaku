#include "BeamManager.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "ParticleManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "GameHelper.h"

std::list<std::unique_ptr<BaseBeam>> BeamManager::beam;

BeamManager::~BeamManager()
{
	std::list<std::unique_ptr<BaseBeam>>().swap(beam);

	BaseBeam::Finalize();
}

std::unique_ptr<BeamManager> BeamManager::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	BeamManager* instance = new BeamManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<BeamManager>(instance);
}

void BeamManager::Initialize()
{
	BaseBeam::StaticInitialize();
}

void BeamManager::Update()
{
	for (auto& i : beam)
	{
		if (!i->GetIsAlive()) { continue; }
		i->Update();
	}

	//停止した弾を随時消去
	{
		for (auto it = beam.begin(); it != beam.end();)
		{
			if (!it->get()->GetIsAlive())
			{
				// 削除された要素の次を指すイテレータが返される。
				it = beam.erase(it);
			}
			// 要素削除をしない場合に、イテレータを進める
			else {
				++it;
			}
		}
	}
}

void BeamManager::Draw()
{
	BaseBeam::Draw();
}

void BeamManager::Reset()
{
	beam.clear();
}

bool BeamManager::CheckBeamToPlayerCollision(const XMFLOAT3& _pos, float _scale)
{
	//プレイヤー情報
	Sphere sphere;
	sphere.center = { _pos.x,_pos.y,_pos.z,0 };
	sphere.radius = _scale;

	for (auto& itr : beam)
	{
		if (!itr->GetIsAlive()) { continue; }

		//弾情報
		XMFLOAT3 beamPos = itr->GetPosition();
		XMFLOAT3 beamSize = itr->GetScale();
		XMFLOAT3 moveVec = itr->GetMoveVec();
		moveVec.x *= beamSize.z;
		moveVec.y *= beamSize.z;
		moveVec.z *= beamSize.z;

		//移動方向のレイ情報
		Capsule capsule;
		capsule.startPosition = { beamPos.x - moveVec.x,beamPos.y - moveVec.y,beamPos.z - moveVec.z };
		capsule.endPosition = { beamPos.x + moveVec.x,beamPos.y + moveVec.y,beamPos.z + moveVec.z };
		capsule.radius = beamSize.x;

		float dist = 0.0f;

		if (Collision::CheckSphereCapsule(sphere, capsule, &dist))
		{
			return true;
		}
	}
	return false;

}