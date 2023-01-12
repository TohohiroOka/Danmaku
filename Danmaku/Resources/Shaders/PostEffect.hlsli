cbuffer cbuff0:register(b0)
{
	float3 outlineColor;//�A�E�g���C���̐F
	float outlineWidth;//�A�E�g���C���̑���
	bool isFog;//�t�H�O�̗L��
	float2 toeColor;
	float2 linearColor;
	float2 shoulder;
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float2 uv : TEXCOORD;
};