cbuffer cbuff0:register(b0)
{
	unsigned int isLuminance;//‹P“x‚Ì—L–³
	float ratio;
};

struct VSOutput {
	float4 pos : SV_POSITION;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float2 uv2 : TEXCOORD2;
	float2 uv3 : TEXCOORD3;
	float2 uv4 : TEXCOORD4;
	float2 uv5 : TEXCOORD5;
	float2 uv6 : TEXCOORD6;
	float2 uv7 : TEXCOORD7;
};