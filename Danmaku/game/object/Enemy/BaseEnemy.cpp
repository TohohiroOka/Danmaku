#include "BaseEnemy.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace DirectX;

std::array<std::unique_ptr<Model>, 2> BaseEnemy::bossModel;
std::unique_ptr<Model> BaseEnemy::enemyAModel;
XMFLOAT3 BaseEnemy::playerPos;

void BaseEnemy::StaticInitialize()
{
	bossModel[0] = Model::CreateFromOBJ("core");//コアボス
	bossModel[1] = Model::CreateFromOBJ("Square");//コアボス
	enemyAModel = Model::CreateFromOBJ("enemyA");//敵A
}

void BaseEnemy::StaticUpdate(const XMFLOAT3& _playerPos)
{
	playerPos = _playerPos;
}

void BaseEnemy::Initialize()
{
	isAlive = true;

	// コライダーの追加
	float radius = 3.0f;
	object->SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	object->GetCollider()->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void BaseEnemy::Update()
{
	pos.x += moveVec.x * speed;
	pos.y += moveVec.y * speed;
	pos.z += moveVec.z * speed;

	object->SetPosition(pos);

	MapCollider();
	object->SetPosition(pos);
}

void BaseEnemy::Draw()
{
	if (!isAlive) { return; }
	object->Draw();
}

void BaseEnemy::Finalize()
{
	bossModel[0].reset();
	enemyAModel.reset();
}

void BaseEnemy::SetMove(const XMFLOAT3& _pos)
{
	//正規化した弾丸の移動ベクトル算出
	XMFLOAT3 vecPt = {};
	vecPt.x = _pos.x - pos.x;
	vecPt.y = _pos.y - pos.y;
	vecPt.z = _pos.z - pos.z;

	//正規化
	float x = powf(vecPt.x, 2);
	float y = powf(vecPt.y, 2);
	float z = powf(vecPt.z, 2);
	float dist = sqrt(x + y + z);
	dist = 1.0f / dist;
	moveVec.x = vecPt.x * dist;
	moveVec.y = vecPt.y * dist;
	moveVec.z = vecPt.z * dist;
}

void BaseEnemy::MapCollider()
{
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(object->GetCollider());
	assert(sphereCollider);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QUERY_HIT& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(90.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};
	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	pos.x += callback.move.m128_f32[0];
	pos.y += callback.move.m128_f32[1];
	pos.z += callback.move.m128_f32[2];

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0.0f,-1.0f,0.0f,0.0f };
	RAYCAST_HIT raycastHit;

	// スムーズに坂を下る為の吸着距離
	const float adsDistance = 1.0f;

	//下の判定
	{
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			pos.y -= a;
		}
	}
	//左の判定
	{
		ray.dir = { -1.0f,0.0f,0.0f,0.0f };
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			pos.x -= a;
		}
	}
	//右の判定
	{
		ray.dir = { 1.0f,0.0f,0.0f,0.0f };
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			pos.x += a;
		}
	}
	//前の判定
	{
		ray.dir = { 0.0f,0.0f,-1.0f,0.0f };
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			pos.z -= a;
		}
	}
	//後の判定
	{
		ray.dir = { 0.0f,0.0f,1.0f,0.0f };
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			float a = (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			pos.z += a;
		}
	}
}