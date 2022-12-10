#include "PostEffect.hlsli"

Texture2D<float4> tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> outline:register(t1);//2�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float> depthTex:register(t2);//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> toneMap:register(t3);//1�ԃX���b�g�ɐݒ肳�ꂽ
SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

/// <summary>
/// �u���[������
/// </summary>
float4 SetTone(float4 color);

/// <summary>
/// �A�E�g���C������
/// </summary>
float4 SetOutline(float2 uv, float outlineWidth, float4 outlineColor);

/// <summary>
/// �t�H�O����
/// </summary>
float4 SetFog(float2 uv);

float4 main(VSOutput input) : SV_TARGET
{
	//���C���J���[
	float4 mainColor = tex.Sample(smp, input.uv);

	//�g�[���}�b�v
	if (isToneMap) {
		const float3 RGB2Y = float3(+0.29900, +0.58700, +0.11400);
		const float3 RGB2Cb = float3(-0.16874, -0.33126, +0.50000);
		const float3 RGB2Cr = float3(+0.50000, -0.41869, -0.08131);
		const float3 YCbCr2R = float3(+1.00000, +0.00000, +1.40200);
		const float3 YCbCr2G = float3(+1.00000, -0.34414, -0.71414);
		const float3 YCbCr2B = float3(+1.00000, +1.77200, +0.00000);

		float4 info = toneMap.Sample(smp, float2(0.5, 0.5));
		float3 texel = mainColor.rgb;

		float coeff = 0.18 * exp(-info.g);
		float l_max = coeff * info.r;

		// YCbCr�n�ɕϊ�
		float3 YCbCr = float3(0, 0, 0);
		YCbCr.y = dot(RGB2Cb, texel);
		YCbCr.z = dot(RGB2Cr, texel);

		// �F�̋����͕␳
		float lum = coeff * dot(RGB2Y, texel);
		YCbCr.x = lum * (1.0f + lum / (l_max * l_max)) / (1.0f + lum);

		// RGB�n�ɂ��ďo��
		float4 output;
		mainColor.r = dot(YCbCr2R, YCbCr);
		mainColor.g = dot(YCbCr2G, YCbCr);
		mainColor.b = dot(YCbCr2B, YCbCr);
	}

	//�A�E�g���C������
	float4 outline = SetOutline(input.uv, outlineWidth, outlineColor);

	//�t�H�O����

	float4 fog = float4(0, 0, 0, 0);
	//if (isFog)
	{
		fog = SetFog(input.uv);
	}

	//�|�X�g�G�t�F�N�g�̍���
	return mainColor + outline + fog;
	//return mainColor;
}

float4 SetTone(float4 color)
{
	return color;
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

	//�J���[�̍��v�l
	float addColor = 0;
	addColor = outlineTex.r + outlineTex.g + outlineTex.b;
	//0.5���傫����΃A�E�g���C������
	addColor = step(0.1, addColor);//0.5���傫����΃A�E�g���C������

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