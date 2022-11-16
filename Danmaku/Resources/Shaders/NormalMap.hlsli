cbuffer cbuff0:register(b0)
{
	float4 color1;//色
	float4 color2;//色
	float4 color3;//色
	matrix world;//3D変換行列
	matrix view;//3D変換行列
	matrix projection;//3D変換行列
	float light;
	float uvPos;
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};