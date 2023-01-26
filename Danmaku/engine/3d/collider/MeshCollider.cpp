#include "MeshCollider.h"
#include "Collision.h"
#include <bitset>
#include "Matrix4.h"
#include "GameHelper.h"

using namespace DirectX;

const bool operator==(const XMINT3& a, const XMINT3& b) {
	return (a.x == b.x) * (a.y == b.y) * (a.z == b.z);
}

const bool operator!=(const XMINT3& a, const XMINT3& b) {
	return !(a.x == b.x) || !(a.y == b.y) || !(a.z == b.z);
}

const bool operator<=(const XMFLOAT3& a, const XMFLOAT3& b) {
	return (a.x <= b.x) && (a.y <= b.y) && (a.z <= b.z);
}

const bool operator>=(const XMFLOAT3& a, const XMFLOAT3& b) {
	return (a.x >= b.x) && (a.y >= b.y) && (a.z >= b.z);
}

void MeshCollider::MinMax(Model* _model)
{
	const std::vector<Mesh*>& meshes = _model->GetMeshes();
	std::vector<Mesh*>::const_iterator it = meshes.cbegin();

	//[0]最大[1]最小
	XMFLOAT3 minmax[2] = { {0,0,0},{100,100,100} };

	for (; it != meshes.cend(); ++it) {
		Mesh* mesh = *it;
		const std::vector<Mesh::VERTEX>& vertices = mesh->GetVertices();
		const int size = static_cast<int>(vertices.size());

		for (int i = 0; i < size; i++)
		{
			if (vertices[i].pos.x > minmax[0].x)
			{
				minmax[0].x = vertices[i].pos.x;
			} else if (vertices[i].pos.x < minmax[1].x)
			{
				minmax[1].x = vertices[i].pos.x;
			}

			if (vertices[i].pos.y > minmax[0].y)
			{
				minmax[0].y = vertices[i].pos.y;
			} else if (vertices[i].pos.y < minmax[1].y)
			{
				minmax[1].y = vertices[i].pos.y;
			}

			if (vertices[i].pos.z > minmax[0].z)
			{
				minmax[0].z = vertices[i].pos.z;
			} else if (vertices[i].pos.z < minmax[1].z)
			{
				minmax[1].z = vertices[i].pos.z;
			}
		}
	}

	min = { minmax[1].x,minmax[1].y,minmax[1].z };
	max = { minmax[0].x,minmax[0].y,minmax[0].z };
}

void MeshCollider::MinMax(const std::vector<Mesh::VERTEX>& _vertices)
{
	//[0]最大[1]最小
	XMFLOAT3 minmax[2] = { {0,0,0},{100,100,100} };

	const std::vector<Mesh::VERTEX> vertices = _vertices;
	const int size = static_cast<int>(vertices.size());

	for (int i = 0; i < size; i++)
	{
		if (vertices[i].pos.x > minmax[0].x)
		{
			minmax[0].x = vertices[i].pos.x;
		} else if (vertices[i].pos.x < minmax[1].x)
		{
			minmax[1].x = vertices[i].pos.x;
		}

		if (vertices[i].pos.y > minmax[0].y)
		{
			minmax[0].y = vertices[i].pos.y;
		} else if (vertices[i].pos.y < minmax[1].y)
		{
			minmax[1].y = vertices[i].pos.y;
		}

		if (vertices[i].pos.z > minmax[0].z)
		{
			minmax[0].z = vertices[i].pos.z;
		} else if (vertices[i].pos.z < minmax[1].z)
		{
			minmax[1].z = vertices[i].pos.z;
		}
	}

	min = { minmax[1].x,minmax[1].y,minmax[1].z };
	max = { minmax[0].x,minmax[0].y,minmax[0].z };
}

XMINT3 MeshCollider::OctreeSet(const XMFLOAT3& _pos)
{
	XMINT3 octtreenum = {};
	for (int i = 0; i < 8; i++)
	{
		if (octreeRange[i].x <= _pos.x && _pos.x < octreeRange[i + 1].x)
		{
			octtreenum.x = i;
			break;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (octreeRange[i].y <= _pos.y && _pos.y < octreeRange[i + 1].y)
		{
			octtreenum.y = i;
			break;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (octreeRange[i].z <= _pos.z && _pos.z < octreeRange[i + 1].z)
		{
			octtreenum.z = i;
			break;
		}
	}

	return octtreenum;
}

void MeshCollider::SetOctreeRange()
{
	//範囲サイズ
	XMFLOAT3 minmaxRange = {
		max.m128_f32[0] - min.m128_f32[0],
		max.m128_f32[1] - min.m128_f32[1],
		max.m128_f32[2] - min.m128_f32[2]
	};

	if (minmaxRange.x <= 1.0f) {
		min.m128_f32[0] -= 10.0f;
		max.m128_f32[0] += 10.0f;
		minmaxRange.x = 20.0f;
	}
	if (minmaxRange.y <= 1.0f) {
		min.m128_f32[1] -= 10.0f;
		max.m128_f32[1] += 10.0f;
		minmaxRange.y = 20.0f;
	}
	if (minmaxRange.z <= 1.0f) {
		min.m128_f32[2] -= 10.0f;
		max.m128_f32[2] += 10.0f;
		minmaxRange.z = 20.0f;
	}

	//範囲指定
	for (int i = 0; i < 9; i++)
	{
		octreeRange[i].x = min.m128_f32[0] + minmaxRange.x / 8 * i;
		octreeRange[i].y = min.m128_f32[1] + minmaxRange.y / 8 * i;
		octreeRange[i].z = min.m128_f32[2] + minmaxRange.z / 8 * i;
	}

	//誤差範囲修正
	octreeRange[8].x += 1.0f;
	octreeRange[8].y += 1.0f;
	octreeRange[8].z += 1.0f;
}

void MeshCollider::ConstructTriangles(Model* _model)
{
	if (!isInit)
	{
		object = std::make_unique<PrimitiveObject3D>();
		isInit = true;
	}

	//初期化
	for (int x = 0; x < octreeSplit; x++)
	{
		for (int y = 0; y < octreeSplit; y++)
		{
			for (int z = 0; z < octreeSplit; z++)
			{
				triangle[x][y][z].clear();
			}
		}
	}

	//八分木用最大最小値
	MinMax(_model);

	SetOctreeRange();

	//メッシュ取得
	const std::vector<Mesh*>& meshes = _model->GetMeshes();

	//頂点を判定用に加工
	std::vector<Mesh*>::const_iterator it = meshes.cbegin();
	for (; it != meshes.cend(); ++it) {
		Mesh* mesh = *it;
		const std::vector<Mesh::VERTEX>& vertices = mesh->GetVertices();
		const std::vector<unsigned long>& indices = mesh->GetIndices();

		size_t triangleNum = indices.size() / 3;

		for (int i = 0; i < triangleNum; i++)
		{
			Triangle addTriangle;
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];

			addTriangle.p0 = {
				vertices[idx0].pos.x,
				vertices[idx0].pos.y,
				vertices[idx0].pos.z,
				1 };
			addTriangle.p1 = {
				vertices[idx1].pos.x,
				vertices[idx1].pos.y,
				vertices[idx1].pos.z,
				1 };

			addTriangle.p2 = {
				vertices[idx2].pos.x,
				vertices[idx2].pos.y,
				vertices[idx2].pos.z,
				1 };

			addTriangle.ComputeNormal();

			XMINT3 Octree1 = OctreeSet(vertices[idx0].pos);
			XMINT3 Octree2 = OctreeSet(vertices[idx1].pos);
			XMINT3 Octree3 = OctreeSet(vertices[idx2].pos);

			triangle[Octree1.x][Octree1.y][Octree1.z].emplace_back(addTriangle);
			if (Octree1 != Octree2)
			{
				triangle[Octree2.x][Octree2.y][Octree2.z].emplace_back(addTriangle);
			}
			if (Octree1 != Octree3 && Octree2 != Octree3)
			{
				triangle[Octree3.x][Octree3.y][Octree3.z].emplace_back(addTriangle);
			}

			object->SetVertex(vertices[idx0].pos);
			object->SetVertex(vertices[idx1].pos);
			object->SetVertex(vertices[idx2].pos);
		}
	}
}

void MeshCollider::ConstructTriangles(const std::vector<Mesh::VERTEX>& _vertices, std::vector<unsigned long>& _indices)
{
	if (!isInit)
	{
		object = std::make_unique<PrimitiveObject3D>();
		isInit = true;
	}

	for (int x = 0; x < octreeSplit; x++)
	{
		for (int y = 0; y < octreeSplit; y++)
		{
			for (int z = 0; z < octreeSplit; z++)
			{
				triangle[x][y][z].clear();
			}
		}
	}

	//八分木用最大最小値
	MinMax(_vertices);

	SetOctreeRange();

	const std::vector<Mesh::VERTEX> vertices = _vertices;
	const std::vector<unsigned long> indices = _indices;

	size_t triangleNum = indices.size() / 3;

	for (int i = 0; i < triangleNum; i++)
	{
		Triangle addTriangle;
		int idx0 = indices[i * 3 + 0];
		int idx1 = indices[i * 3 + 1];
		int idx2 = indices[i * 3 + 2];

		addTriangle.p0 = {
			vertices[idx0].pos.x,
			vertices[idx0].pos.y,
			vertices[idx0].pos.z,
			1 };
		addTriangle.p1 = {
			vertices[idx1].pos.x,
			vertices[idx1].pos.y,
			vertices[idx1].pos.z,
			1 };

		addTriangle.p2 = {
			vertices[idx2].pos.x,
			vertices[idx2].pos.y,
			vertices[idx2].pos.z,
			1 };

		addTriangle.ComputeNormal();

		XMINT3 Octree1 = OctreeSet(vertices[idx0].pos);
		XMINT3 Octree2 = OctreeSet(vertices[idx1].pos);
		XMINT3 Octree3 = OctreeSet(vertices[idx2].pos);

		triangle[Octree1.x][Octree1.y][Octree1.z].emplace_back(addTriangle);
		if (Octree1 != Octree2)
		{
			triangle[Octree2.x][Octree2.y][Octree2.z].emplace_back(addTriangle);
		}
		if (Octree1 != Octree3 && Octree2 != Octree3)
		{
			triangle[Octree3.x][Octree3.y][Octree3.z].emplace_back(addTriangle);
		}

		object->SetVertex(vertices[idx0].pos);
		object->SetVertex(vertices[idx1].pos);
		object->SetVertex(vertices[idx2].pos);
	}
}

void MeshCollider::Update()
{
	matWorld = GetObject3d()->GetMatWorld();
	invMatWorld = XMMatrixInverse(nullptr, matWorld);
	if (isInit && !isCreateBuffer)
	{
		object->Initialize();
	}
}

void MeshCollider::Draw()
{
	object->SetMatWorld(GetObject3d()->GetMatWorld());
	object->Draw();
}

bool MeshCollider::CheckCollisionSphere(const Sphere& _sphere, DirectX::XMVECTOR* _inter, DirectX::XMVECTOR* _reject)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	Sphere localSphere;
	localSphere.center = XMVector3Transform(_sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	XMVECTOR worldmax = XMVector3Transform(max, matWorld);
	XMVECTOR worldmin = XMVector3Transform(min, matWorld);

	//範囲探索
	if (localSphere.center.m128_f32[0] < worldmin.m128_f32[0] || localSphere.center.m128_f32[0] > worldmax.m128_f32[0] ||
		localSphere.center.m128_f32[1] < worldmin.m128_f32[1] || localSphere.center.m128_f32[1] > worldmax.m128_f32[1] ||
		localSphere.center.m128_f32[2] < worldmin.m128_f32[2] || localSphere.center.m128_f32[2] > worldmax.m128_f32[2])
	{
		return false;
	}

	//プレイヤーの八分木位置
	const XMINT3 Octree = OctreeSet({ localSphere.center.m128_f32[0],localSphere.center.m128_f32[1],localSphere.center.m128_f32[2] });

	//区分内を捜索
	std::vector<Triangle>::const_iterator it = triangle[Octree.x][Octree.y][Octree.z].cbegin();
	for (; it != triangle[Octree.x][Octree.y][Octree.z].cend(); ++it) {
		const Triangle& mesh = *it;

		//判定
		if (!Collision::CheckSphere2Triangle(localSphere, mesh, _inter, _reject)) { continue; }

		if (_inter) {
			*_inter = XMVector3Transform(*_inter, matWorld);
		}
		if (_reject) {
			*_reject = XMVector3TransformNormal(*_reject, matWorld);
		}
		return true;
	}
	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& _ray, float* _distance, DirectX::XMVECTOR* _inter)
{
	// オブジェクトのローカル座標系でのレイを得る
	Ray localRay;
	localRay.start = XMVector3Transform(_ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(_ray.dir, invMatWorld);
	localRay.dir=XMVector3Normalize(localRay.dir);

	XMVECTOR worldmax = XMVector3Transform(max, matWorld);
	XMVECTOR worldmin = XMVector3Transform(min, matWorld);

	//範囲探索
	if (_ray.start.m128_f32[0] < worldmin.m128_f32[0] || _ray.start.m128_f32[0] > worldmax.m128_f32[0] ||
		_ray.start.m128_f32[1] < worldmin.m128_f32[1] || _ray.start.m128_f32[1] > worldmax.m128_f32[1] ||
		_ray.start.m128_f32[2] < worldmin.m128_f32[2] || _ray.start.m128_f32[2] > worldmax.m128_f32[2])
	{
		return false;
	}

	//プレイヤーの八分木位置
	const XMINT3 Octree = OctreeSet({ localRay.start.m128_f32[0],localRay.start.m128_f32[1],localRay.start.m128_f32[2] });

	//区分内を捜索
	std::vector<Triangle>::const_iterator it = triangle[Octree.x][Octree.y][Octree.z].cbegin();
	for (; it != triangle[Octree.x][Octree.y][Octree.z].cend(); ++it) {
		const Triangle& mesh = *it;

		//判定
		XMVECTOR tempInter;
		if (!Collision::CheckRay2Triangle(localRay, mesh, nullptr, &tempInter)) { continue; }

		tempInter = XMVector3Transform(tempInter, matWorld);

		if (_distance) {
			XMVECTOR sub = tempInter - _ray.start;
			*_distance = XMVector3Dot(sub, _ray.dir).m128_f32[0];
		}

		if (_inter) {
			*_inter = tempInter;
		}

		return true;
	}

	return false;
}

bool MeshCollider::CheckCollisionCapsule(const Capsule& _capsule)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	Capsule localCapsule;
	localCapsule.startPosition = _capsule.startPosition.DirectXVector3Transform(invMatWorld);
	localCapsule.endPosition = _capsule.endPosition.DirectXVector3Transform(invMatWorld);
	localCapsule.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	XMVECTOR worldmax = XMVector3Transform(max, matWorld);
	XMVECTOR worldmin = XMVector3Transform(min, matWorld);

	//範囲探索
	if (localCapsule.startPosition.x < worldmin.m128_f32[0] || localCapsule.startPosition.x > worldmax.m128_f32[0] ||
		localCapsule.startPosition.y < worldmin.m128_f32[1] || localCapsule.startPosition.y > worldmax.m128_f32[1] ||
		localCapsule.startPosition.z < worldmin.m128_f32[2] || localCapsule.startPosition.z > worldmax.m128_f32[2] ||
		localCapsule.endPosition.x < worldmin.m128_f32[0] || localCapsule.endPosition.x > worldmax.m128_f32[0] ||
		localCapsule.endPosition.y < worldmin.m128_f32[1] || localCapsule.endPosition.y > worldmax.m128_f32[1] ||
		localCapsule.endPosition.z < worldmin.m128_f32[2] || localCapsule.endPosition.z > worldmax.m128_f32[2])
	{
		return false;
	}

	//プレイヤーの八分木位置
	const XMINT3 Octree[2] = {
		OctreeSet({ localCapsule.startPosition.x,localCapsule.startPosition.y,localCapsule.startPosition.z }),
		OctreeSet({ localCapsule.endPosition.x,localCapsule.endPosition.y,localCapsule.endPosition.z })
	};

	const int roopNum = 1 + (Octree[0] != Octree[1]);

	for (int i = 0; i < roopNum; i++)
	{
		std::vector<Triangle>::const_iterator it = triangle[Octree[i].x][Octree[i].y][Octree[i].z].cbegin();
		for (; it != triangle[Octree[i].x][Octree[i].y][Octree[i].z].cend(); ++it) {
			const Triangle& mesh = *it;

			if (Collision::CheckTriangleCapsule(mesh, localCapsule)) { continue; }
			return true;
		}
	}

	return false;
}