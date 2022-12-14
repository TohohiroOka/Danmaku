#include "PostEffect.hlsli"

Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
Texture2D<float4> outline:register(t1);//2番スロットに設定されたテクスチャ
Texture2D<float> depthTex:register(t2);//1番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラー

/// <summary>
/// アウトライン処理
/// </summary>
float4 SetOutline(float2 uv, float outlineWidth, float4 outlineColor);

/// <summary>
/// フォグ処理
/// </summary>
float4 SetFog(float2 uv);

float4 main(VSOutput input) : SV_TARGET
{
	//メインカラー
	float4 mainColor = tex.Sample(smp, input.uv);

	//トーンマップ
	if (isToneMap) {
		float a = toeColor.x;
		float b = toeColor.y;
		float c = linearColor.x;
		float d = linearColor.y;
		float e = 0.14f;
		float3 toneColor = saturate((mainColor * (a * mainColor + b)) / (mainColor * (c * mainColor + d) + e));

		//光度値の抽出
		float LuminousIntensity = dot(mainColor.rgb, float3(0.2125, 0.7154, 0.0712));

		mainColor = mainColor * (1 - step(LuminousIntensity, 1.0)) + float4(toneColor, mainColor.a) * step(LuminousIntensity, 1.0);
	}

	//アウトライン処理
	float4 outline = SetOutline(input.uv, outlineWidth, outlineColor);

	//フォグ処理

	float4 fog = float4(0, 0, 0, 0);
	//if (isFog)
	{
		fog = SetFog(input.uv);
	}

	//ポストエフェクトの合成
	return mainColor + outline + fog;
	//return mainColor;
}

float4 SetOutline(float2 uv, float outlineWidth, float4 outlineColor)
{
	float4 outlineTex = float4(0, 0, 0, 0);

	float xPoutline = uv.x + outlineWidth;
	float xMoutline = uv.x - outlineWidth;
	float yPoutline = uv.y + outlineWidth;
	float yMoutline = uv.y - outlineWidth;

	float add1 = xPoutline < 1;
	float add2 = xMoutline > 0;
	float add3 = yPoutline < 1;
	float add4 = yMoutline > 0;

	outlineTex = outlineTex + outline.Sample(smp, float2(xPoutline, yPoutline)) * (add1 * add3);
	outlineTex = outlineTex + outline.Sample(smp, float2(xPoutline, yMoutline)) * (add1 * add4);
	outlineTex = outlineTex + outline.Sample(smp, float2(xMoutline, yPoutline)) * (add2 * add3);
	outlineTex = outlineTex + outline.Sample(smp, float2(xMoutline, yMoutline)) * (add2 * add4);
	outlineTex = outlineTex + outline.Sample(smp, float2(xPoutline, uv.y)) * add1;
	outlineTex = outlineTex + outline.Sample(smp, float2(xMoutline, uv.y)) * add2;
	outlineTex = outlineTex + outline.Sample(smp, float2(uv.x, yPoutline)) * add3;
	outlineTex = outlineTex + outline.Sample(smp, float2(uv.x, yMoutline)) * add4;

	float4 normalColor = outline.Sample(smp, uv);
	outlineTex.rgb = outlineTex.rgb - normalColor.rgb * 8.0;

	//カラーの合計値
	float addColor = 0;
	addColor = outlineTex.r + outlineTex.g + outlineTex.b;
	//0.5より大きければアウトライン生成
	addColor = step(0.1, addColor);//0.5より大きければアウトライン生成

	outlineTex.r = outlineColor.r * addColor;
	outlineTex.g = outlineColor.g * addColor;
	outlineTex.b = outlineColor.b * addColor;

	return outlineTex;
}

float4 SetFog(float2 uv)
{
	float depth = pow(depthTex.Sample(smp, uv), 10000);
	depth = depth * depth;
	float4 depthColor = float4(0, depth/2, depth, 1.0);

	return depthColor;
}