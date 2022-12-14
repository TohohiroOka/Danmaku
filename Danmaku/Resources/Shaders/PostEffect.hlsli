cbuffer cbuff0:register(b0)
{
	float2 toeColor;//トーンマップ用の色指定
	float2 linearColor;//トーンマップ用の色指定
	float4 outlineColor;//アウトラインの色
	float outlineWidth;//アウトラインの太さ
	unsigned int isFog;//フォグの有無
	unsigned int isToneMap;//トーンマップの有無
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float2 uv : TEXCOORD;
};