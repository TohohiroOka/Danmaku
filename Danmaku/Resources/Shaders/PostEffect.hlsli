cbuffer cbuff0:register(b0)
{
	float3 outlineColor;//アウトラインの色
	float outlineWidth;//アウトラインの太さ
	bool isFog;//フォグの有無
	float2 toeColor;
	float2 linearColor;
	float2 shoulder;
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float2 uv : TEXCOORD;
};