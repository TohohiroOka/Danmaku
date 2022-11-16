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

bool BeamManager::CheckPlayerBulletToEnemyCollision(const XMFLOAT3& _pos, float _scale)
{
	return false;
}