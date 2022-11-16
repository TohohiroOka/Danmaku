#include "MeshCollider.h"
#include "Collision.h"
#include <bitset>
#include "Matrix4.h"

using namespace DirectX;

float MeshCollider::DistanceTwoPoints(const Vector3& _pos1, const Vector3& _pos2)
{
	float returnNum = 0;

	float x = 0, y = 0, z = 0;
	x = _pos2.x - _pos1.x;
	y = _pos2.y - _pos1.y;
	z = _pos2.z - _pos1.z;

	returnNum = sqrtf(x * x + y * y + z * z);

	return returnNum;
}

void MeshCollider::SetCircumscribedCircle(const XMFLOAT3& _pos1, const XMFLOAT3& _pos2, const XMFLOAT3& _pos3, Sphere& _sphere)
{
	const Vector3 point1 = { _pos1.x,_pos1.y,_pos1.z };
	const Vector3 point2 = { _pos2.x,_pos2.y,_pos2.z };
	const Vector3 point3 = { _pos3.x,_pos3.y,_pos3.z };

	//pos2->pos3のベクトル
	Vector3 p2_p3 = point3 - point2;

	// 鈍角を探します
	 // 鈍角三角形の場合はその対辺の中点が球の中心で両頂点までの距離が半径。
	float dot0 = (point2 - point1).dot(point3 - point1);
	if (dot0 <= 0.0f) {
		Vector3 center = (point2 + point3) / 2.0f;
		_sphere.center = { center.x,center.y,center.z };
		_sphere.radius = ((point2 - point3) / 2.0f).length();
		return;
	}
	float dot1 = (point1 - point2).dot(point3 - point2);
	if (dot1 <= 0.0f) {
		Vector3 center = (point1 + point3) / 2.0f;
		_sphere.center = { center.x,center.y,center.z };
		_sphere.radius = ((point1 - point3) / 2.0f).length();
	}
	float dot2 = (point1 - point3).dot(point2 - point3);
	if (dot2 <= 0.0f) {
		Vector3 center = (point1 + point2) / 2.0f;
		_sphere.center = { center.x,center.y,center.z };
		_sphere.radius = ((point1 - point2) / 2.0f).length();
	}

	// 鋭角三角形でした
	// 3頂点から等距離にある点が中心。連立方程式を解きます。
	Vector3 N = {};
	N = (point2 - point1).cross(point3 - point1);
	Vector3 v0 = {}, v1 = {}, e0 = {}, e1 = {};
	v0 = (point3 - point2).cross(N);
	v1 = (point3 - point1).cross(N);
	e0 = (point3 + point2) * 0.5f;
	e1 = (point3 + point1) * 0.5f;
	float a = v0.dot(v1);
	float b = v0.dot(v0);
	float c = -(e1 - e0).dot(v0);
	float d = v1.dot(v1);
	float e = -(e1 - e0).dot(v1);

	float div = -a * a + b * d;
	float t = (-a * c + b * e) / div;
	float s = (-c * d + a * e) / div;

	Vector3 center = e0 + s * v0;
	_sphere.center = { center.x,center.y,center.z };
	_sphere.radius = (center - point1).length();
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

void MeshCollider::MinMax(const std::vector<Mesh::VERTEX>* _vertices)
{
	//[0]最大[1]最小
	XMFLOAT3 minmax[2] = { {0,0,0},{100,100,100} };

	const std::vector<Mesh::VERTEX> vertices = *_vertices;
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

int MeshCollider::OctreeSet(const XMFLOAT3& _pos)
{
	int octtreenum[2] = {};
	for (int i = 0; i < 8; i++)
	{
		if (octtreeRange[i].x < _pos.x && _pos.x < octtreeRange[i + 1].x)
		{
			octtreenum[0] = i;
			break;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (octtreeRange[i].z < _pos.z && _pos.z < octtreeRange[i + 1].z)
		{
			octtreenum[1] = i;
			break;
		}
	}

	std::string binaryX = std::bitset<3>(octtreenum[0]).to_string();
	std::string binaryZ = std::bitset<3>(octtreenum[1]).to_string();

	std::string nowBinary =
		binaryZ.substr(0, 1) + binaryX.substr(0, 1) +
		binaryZ.substr(1, 1) + binaryX.substr(1, 1) +
		binaryZ.substr(2, 1) + binaryX.substr(2, 1);

	int Octree = strtoul(nowBinary.c_str(), NULL, 2);

	return Octree;
}

void MeshCollider::ConstructTriangles(Model* _model)
{
	if (!isInit)
	{
		object = std::make_unique<PrimitiveObject3D>();
		isInit = true;
	}

	for (int i = 0; i < octreeSplit; i++)
	{
		colliderMeshes[i].clear();
	}
	const std::vector<Mesh*>& meshes = _model->GetMeshes();

	int start = 0;

	MinMax(_model);
	
	XMFLOAT3 minmaxRange = { 
		max.m128_f32[0] - min.m128_f32[0],
		max.m128_f32[1] - min.m128_f32[1],
		max.m128_f32[2] - min.m128_f32[2]
	};

	for (int i = 0; i < 9; i++)
	{
		octtreeRange[i].x = minmaxRange.x / 8 * i;
		octtreeRange[i].y = minmaxRange.y / 8 * i;
		octtreeRange[i].z = minmaxRange.z / 8 * i;
	}

	std::vector<Mesh*>::const_iterator it = meshes.cbegin();
	for (; it != meshes.cend(); ++it) {
		Mesh* mesh = *it;
		const std::vector<Mesh::VERTEX>& vertices = mesh->GetVertices();
		const std::vector<unsigned long>& indices = mesh->GetIndices();

		size_t triangleNum = indices.size() / 3;

		for (int i = 0; i < triangleNum; i++)
		{
			ONE_MESH addMesh;
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];

			addMesh.triangle.p0 = {
				vertices[idx0].pos.x,
				vertices[idx0].pos.y,
				vertices[idx0].pos.z,
				1 };
			addMesh.triangle.p1 = {
				vertices[idx1].pos.x,
				vertices[idx1].pos.y,
				vertices[idx1].pos.z,
				1 };

			addMesh.triangle.p2 = {
				vertices[idx2].pos.x,
				vertices[idx2].pos.y,
				vertices[idx2].pos.z,
				1 };

			addMesh.triangle.ComputeNormal();

			SetCircumscribedCircle(vertices[idx0].pos, vertices[idx1].pos, vertices[idx2].pos, addMesh.sphere);

			int Octree1 = OctreeSet(vertices[idx0].pos);
			int Octree2 = OctreeSet(vertices[idx1].pos);
			int Octree3 = OctreeSet(vertices[idx2].pos);

			colliderMeshes[Octree1].emplace_back(addMesh);
			if (Octree1 != Octree2)
			{
				colliderMeshes[Octree2].emplace_back(addMesh);
			}
			if (Octree1 != Octree3 && Octree2 != Octree3)
			{
				colliderMeshes[Octree3].emplace_back(addMesh);
			}

			object->SetVertex(vertices[idx0].pos);
			object->SetVertex(vertices[idx1].pos);
			object->SetVertex(vertices[idx2].pos);
		}
	}
}

void MeshCollider::ConstructTriangles(const std::vector<Mesh::VERTEX>* _vertices, std::vector<unsigned long>* _indices)
{
	if (!isInit)
	{
		object = std::make_unique<PrimitiveObject3D>();
		isInit = true;
	}

	for (int i = 0; i < octreeSplit; i++)
	{
		colliderMeshes[i].clear();
	}

	int start = 0;

	MinMax(_vertices);

	XMFLOAT3 minmaxRange = {
		max.m128_f32[0] - min.m128_f32[0],
		max.m128_f32[1] - min.m128_f32[1],
		max.m128_f32[2] - min.m128_f32[2]
	};
	for (int i = 0; i < 9; i++)
	{
		octtreeRange[i].x = minmaxRange.x / 8 * i;
		octtreeRange[i].y = minmaxRange.y / 8 * i;
		octtreeRange[i].z = minmaxRange.z / 8 * i;
	}

	const std::vector<Mesh::VERTEX> vertices = *_vertices;
	const std::vector<unsigned long> indices = *_indices;

	size_t triangleNum = indices.size() / 3;

	for (int i = 0; i < triangleNum; i++)
	{
		ONE_MESH addMesh;
		int idx0 = indices[i * 3 + 0];
		int idx1 = indices[i * 3 + 1];
		int idx2 = indices[i * 3 + 2];

		addMesh.triangle.p0 = {
			vertices[idx0].pos.x,
			vertices[idx0].pos.y,
			vertices[idx0].pos.z,
			1 };
		addMesh.triangle.p1 = {
			vertices[idx1].pos.x,
			vertices[idx1].pos.y,
			vertices[idx1].pos.z,
			1 };

		addMesh.triangle.p2 = {
			vertices[idx2].pos.x,
			vertices[idx2].pos.y,
			vertices[idx2].pos.z,
			1 };

		addMesh.triangle.ComputeNormal();

		SetCircumscribedCircle(vertices[idx0].pos, vertices[idx1].pos, vertices[idx2].pos, addMesh.sphere);

		int Octree1 = OctreeSet(vertices[idx0].pos);
		int Octree2 = OctreeSet(vertices[idx1].pos);
		int Octree3 = OctreeSet(vertices[idx2].pos);

		colliderMeshes[Octree1].emplace_back(addMesh);
		if (Octree1 != Octree2)
		{
			colliderMeshes[Octree2].emplace_back(addMesh);
		}
		if (Octree1 != Octree3 && Octree2 != Octree3)
		{
			colliderMeshes[Octree3].emplace_back(addMesh);
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

	if (_sphere.center.m128_f32[0] < worldmin.m128_f32[0] || _sphere.center.m128_f32[0] > worldmax.m128_f32[0] ||
		_sphere.center.m128_f32[2] <  worldmin.m128_f32[2] || _sphere.center.m128_f32[2] >  worldmax.m128_f32[2])
	{
		return false;
	}

	//プレイヤーの八分木位置
	const int Octree = OctreeSet({ localSphere.center.m128_f32[0],localSphere.center.m128_f32[1],localSphere.center.m128_f32[2] });

	std::vector<ONE_MESH>::const_iterator it = colliderMeshes[Octree].cbegin();
	for (; it != colliderMeshes[Octree].cend(); ++it) {
		const ONE_MESH& mesh = *it;

		if (Collision::CheckSphere2Triangle(localSphere, mesh.triangle, _inter, _reject)) {
			if (_inter) {
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();

				*_inter = XMVector3Transform(*_inter, matWorld);
			}
			if (_reject) {
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();

				*_reject = XMVector3TransformNormal(*_reject, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& _ray, float* _distance, DirectX::XMVECTOR* _inter)
{
	// オブジェクトのローカル座標系でのレイを得る
	Ray localRay;
	localRay.start = XMVector3Transform(_ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(_ray.dir, invMatWorld);

	XMVECTOR worldmax = XMVector3Transform(max, matWorld);
	XMVECTOR worldmin = XMVector3Transform(min, matWorld);

	if (_ray.start.m128_f32[0] < worldmin.m128_f32[0] || _ray.start.m128_f32[0] > worldmax.m128_f32[0] ||
		_ray.start.m128_f32[2] <  worldmin.m128_f32[2] || _ray.start.m128_f32[2] >  worldmax.m128_f32[2])
	{
		return false;
	}

	//プレイヤーの八分木位置
	const int Octree = OctreeSet({ localRay.start.m128_f32[0],localRay.start.m128_f32[1],localRay.start.m128_f32[2] });

	std::vector<ONE_MESH>::const_iterator it = colliderMeshes[Octree].cbegin();

	for (; it != colliderMeshes[Octree].cend(); ++it) {
		const ONE_MESH& mesh = *it;

		XMVECTOR tempInter;
		if (Collision::CheckRay2Triangle(localRay, mesh.triangle, nullptr, &tempInter)) {

			const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();

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
	}

	return false;
}

bool MeshCollider::CheckCollisionCapsule(const Capsule& _capsule, float* _distance, DirectX::XMVECTOR* _inter)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	Capsule localCapsule;
	localCapsule.startPosition = _capsule.startPosition.DirectXVector3Transform(invMatWorld);
	localCapsule.endPosition = _capsule.endPosition.DirectXVector3Transform(invMatWorld);
	localCapsule.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	XMVECTOR worldmax = XMVector3Transform(max, matWorld);
	XMVECTOR worldmin = XMVector3Transform(min, matWorld);

	if (_capsule.startPosition.x < worldmin.m128_f32[0] || _capsule.startPosition.x > worldmax.m128_f32[0] ||
		_capsule.startPosition.z <  worldmin.m128_f32[2] || _capsule.startPosition.z >  worldmax.m128_f32[2]||
		_capsule.endPosition.x < worldmin.m128_f32[0] || _capsule.endPosition.x > worldmax.m128_f32[0] ||
		_capsule.endPosition.z <  worldmin.m128_f32[2] || _capsule.endPosition.z >  worldmax.m128_f32[2])
	{
		return false;
	}

	//プレイヤーの八分木位置
	const int Octree[2] = {
		OctreeSet({ localCapsule.startPosition.x,localCapsule.startPosition.y,localCapsule.startPosition.z }),
		OctreeSet({ localCapsule.endPosition.x,localCapsule.endPosition.y,localCapsule.endPosition.z })
	};

	const int roopNum = 1 + Octree[0] != Octree[1];

	for (int i=0;i< roopNum;i++)
	{
		std::vector<ONE_MESH>::const_iterator it = colliderMeshes[Octree[i]].cbegin();;

		for (; it != colliderMeshes[Octree[i]].cend(); ++it) {
			const ONE_MESH& mesh = *it;

			float distance;
			if (Collision::CheckSphereCapsule(mesh.sphere, localCapsule, &distance))
			/*if (!Collision::CheckSphereCapsule(mesh.sphere, localCapsule, &distance))continue;
			if (Collision::CheckTriangleCapsule(mesh.triangle, localCapsule))*/{
				return true;
			}
		}
	}
	return false;
}