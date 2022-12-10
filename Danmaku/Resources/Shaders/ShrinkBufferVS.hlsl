#include "ShrinkBuffer.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;//ピクセルシェーダーに渡す値
	output.pos = pos;

	output.uv0 = uv + float2(0.5 * ratio, 0.5 * ratio);
	output.uv1 = uv + float2(1.5 * ratio, 0.5 * ratio);
	output.uv2 = uv + float2(3.5 * ratio, 0.5 * ratio);
	output.uv3 = uv + float2(5.5 * ratio, 0.5 * ratio);
	output.uv4 = uv + float2(0.5 * ratio, 1.5 * ratio);
	output.uv5 = uv + float2(1.5 * ratio, 1.5 * ratio);
	output.uv6 = uv + float2(3.5 * ratio, 1.5 * ratio);
	output.uv7 = uv + float2(5.5 * ratio, 1.5 * ratio);

	return output;
}