#include "Bloom.hlsli"

Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
Texture2D<float4> bloomTex:register(t1);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラー

/// <summary>
/// ガウス処理
/// </summary>
float Gaussian(float2 drawUV, float2 pickUV, float sigma);

float4 main(VSOutput input) : SV_TARGET
{
	float totalWeight = 0;
	float sigma = 0.01;
	float stepWidth = 0.001;
	float4 color = { 0, 0, 0, 0 };

	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
	{
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
		{
			float2 pickUV = input.uv + float2(px, py);
			float add1 = (pickUV.x <= 1) * (pickUV.x >= 0);
			float add2 = (pickUV.y <= 1) * (pickUV.y >= 0);
			float weight = Gaussian(input.uv, pickUV, sigma);
			color += bloomTex.Sample(smp, pickUV) * weight * add1 * add2;
			totalWeight += weight;
		}
	}

	color.rgb = color.rgb / totalWeight;

	//メインカラー
	float4 mainColor = tex.Sample(smp, input.uv);

	return mainColor + float4(color.rgb, 0.0);
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-d * d) / (2 * sigma * sigma);
}
