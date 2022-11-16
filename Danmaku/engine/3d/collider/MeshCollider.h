#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>
#include "PrimitiveObject3D.h"

#include "Vector3.h"

/// <summary>
/// メッシュ衝突判定オブジェクト
/// </summary>
class MeshCollider :
	public BaseCollider
{
public:
	MeshCollider()
	{
		// メッシュ形状をセット
		shapeType = COLLISIONSHAPE_MESH;
	}

	struct ONE_MESH {
		Triangle triangle;
		Sphere sphere;
	};

	/// <summary>
	/// 三次元空間の二点間の距離を求める
	/// </summary>
	/// <param name="pos1">座標1</param>
	/// <param name="pos2">座標2</param>
	/// <returns>二点間の距離</returns>
	float DistanceTwoPoints(const Vector3& _pos1, const Vector3& _pos2);

	/// <summary>
	/// 三角形から外接円を求める
	/// </summary>
	/// <param name="pos1">三角形の頂点1</param>
	/// <param name="pos2">三角形の頂点2</param>
	/// <param name="pos3">三角形の頂点3</param>
	void SetCircumscribedCircle(const DirectX::XMFLOAT3& _pos1, const DirectX::XMFLOAT3& _pos2, const DirectX::XMFLOAT3& _pos3, Sphere& _sphere);

	/// <summary>
	/// 八分木の最大最小の保存
	/// </summary>
	/// <param name="_model">モデル</param>
	void MinMax(Model* _model);

	/// <summary>
	/// 八分木の最大最小の保存
	/// </summary>
	/// <param name="_vertices">頂点</param>
	void MinMax(const std::vector<Mesh::VERTEX>* _vertices);

	/// <summary>
	/// 八分木の現在地セット
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <returns>現在番号</returns>
	int OctreeSet(const DirectX::XMFLOAT3& _pos);

	/// <summary>
	/// 三角形の配列を構築する
	/// </summary>
	/// <param name="_model">モデル</param>
	void ConstructTriangles(Model* _model);

	/// <summary>
	/// 三角形の配列を構築する
	/// </summary>
	/// <param name="_vertices">頂点</param>
	/// <param name="_indices">インデック</param>
	void ConstructTriangles(const std::vector<Mesh::VERTEX>* _vertices, std::vector<unsigned long>* _indices);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	/// <param name="_sphere">球</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <param name="_reject">排斥ベクトル（出力用）</param>
	/// <returns>交差しているか否か</returns>
	bool CheckCollisionSphere(const Sphere& _sphere, DirectX::XMVECTOR* _inter = nullptr, DirectX::XMVECTOR* _reject = nullptr);

	/// <summary>
	/// レイとの当たり判定
	/// </summary>
	/// <param name="_sphere">レイ</param>
	/// <param name="_distance">距離（出力用）</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	bool CheckCollisionRay(const Ray& _ray, float* _distance, DirectX::XMVECTOR* _inter);

	/// <summary>
	/// カプセルとの当たり判定
	/// </summary>
	/// <param name="_capsule">カプセル</param>
	/// <param name="_distance">距離（出力用）</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	bool CheckCollisionCapsule(const Capsule& _capsule, float* _distance, DirectX::XMVECTOR* _inter);

private:

	//八分木分割個数
	static const int octreeSplit = 64;
	//判定用メッシュの情報
	std::array<std::vector<ONE_MESH>, octreeSplit> colliderMeshes;
	//八分木の最小値
	DirectX::XMVECTOR min = {};
	//八分木の最大値
	DirectX::XMVECTOR max = {};
	//八分木の分割範囲
	DirectX::XMFLOAT3 octtreeRange[9] = {};
	// ワールド行列
	DirectX::XMMATRIX matWorld;
	// ワールド行列の逆行列
	DirectX::XMMATRIX invMatWorld;
	//オブジェクトの初期化フラグ
	bool isInit = false;
	//オブジェクトのbuffer生成フラグ
	bool isCreateBuffer = false;
	//コリジョンの視覚化用
	std::unique_ptr<PrimitiveObject3D> object = nullptr;
};

