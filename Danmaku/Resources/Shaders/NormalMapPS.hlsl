#include "NormalMap.hlsli"

Texture2D<float4> tex[3]:register(t0);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float3 bumpNormal;
	float3 lightDir;
	float lightIntensity;
	float4 texColor1;

	float4 texColor2 = tex[0].Sample(smp, input.uv);

	bumpNormal = tex[1].Sample(smp, input.uv).xyz;
	bumpNormal = (bumpNormal - 0.5f) * 2.0f;
	bumpNormal = normalize(bumpNormal);
	float3 lightpos = { light,10.0f,0.0f };
	lightDir = -lightpos.xyz;
	lightIntensity = saturate(dot(bumpNormal, lightDir));
	texColor1 = lerp(color2, texColor2, lightIntensity);
	texColor1 = texColor1 * color3;

	float4 texColor3 = tex[2].Sample(smp, input.uv + uvPos * 0.01f);

	texColor1 = texColor1 + (texColor3 * color3 * step(texColor3.x + texColor3.y + texColor3.z, 1.4f));

	return texColor1 * color1;
}