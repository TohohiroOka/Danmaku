#include "DrawLine3D.hlsli"

VSOutput main( float4 pos : POSITION )
{
	//�s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	output.svpos = mul(mul(viewproj, world), pos);

	return output;
}