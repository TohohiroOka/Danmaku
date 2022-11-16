#include "NormalMap.hlsli"

VSOutput main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;//ピクセルシェーダーに渡す値
	////行列をposに反映
	output.svpos = mul(world, pos);
	output.svpos = mul(view, output.svpos);
	output.svpos = mul(projection, output.svpos);

	//uvを反映
	output.uv = uv;

	//法線と行列の計算
	output.normal = mul(world, normal);
	output.normal = normalize(output.normal);

	return output;
}