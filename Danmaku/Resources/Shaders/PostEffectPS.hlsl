#include "PostEffect.hlsli"

Texture2D<float4> tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> bloom:register(t1);//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> outline:register(t2);//2�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float> depthTex:register(t3);//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

/// <summary>
/// �K�E�X����
/// </summary>
float Gaussian(float2 drawUV, float2 pickUV, float sigma);

/// <summary>
/// �u���[������
/// </summary>
float4 SetBloom(float2 uv);

/// <summary>
/// �A�E�g���C������
/// </summary>
float4 SetOutline(float2 uv, float outlineWidth, float3 outlineColor);

/// <summary>
/// �t�H�O����
/// </summary>
float4 SetFog(float2 uv);

/// <summary>
/// �g�[���}�b�v�̐F�ύX����
/// </summary>
float ToneChange(float mainColor, float2 toeColor, float2 linearColor, float2 shoulderColor,
	float a1, float b1, float at, float bt, float ct, float as, float bs, float cs);

/// <summary>
/// �g�[���}�b�v����
/// </summary>
float3 ToneMap(float3 mainColor, float2 toeColor, float2 linearColor, float2 shoulderColor);

float4 main(VSOutput input) : SV_TARGET
{
	//���C���J���[
	float4 mainColor = tex.Sample(smp, input.uv);

	//bloom����
	float4 bloom = SetBloom(input.uv);

	//�A�E�g���C������
	float4 outline = SetOutline(input.uv, outlineWidth, outlineColor);

	//�t�H�O����
	float4 fog = float4(0, 0, 0, 0);
	if (isFog)
	{
		fog = SetFog(input.uv);
	}

	//�g�[���}�b�v����
	float3 toneMapColor = ToneMap(float3(mainColor.rgb + bloom.rgb), toeColor, linearColor, shoulder);

	//�|�X�g�G�t�F�N�g�̍���
	return float4(toneMapColor, 1.0) + outline + fog;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-d * d) / (2 * sigma * sigma);
}

float4 SetBloom(float2 uv)
{
	float totalWeight = 0;
	float sigma = 0.005;
	float stepWidth = 0.001;
	float4 color = { 0, 0, 0, 0 };

	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
	{
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
		{
			float2 pickUV = uv + float2(px, py);
			float add1 = (pickUV.x <= 1) * (pickUV.x >= 0);
			float add2 = (pickUV.y <= 1) * (pickUV.y >= 0);
			float weight = Gaussian(uv, pickUV, sigma);
			color += bloom.Sample(smp, pickUV) * weight * add1 * add2;
			totalWeight += weight;
		}
	}

	color.rgb = color.rgb / totalWeight;

	//���ȉ��؂�̂�
	color = color * step(0.4, color.r + color.g + color.b);
	return color;
}

float4 SetOutline(float2 uv, float outlineWidth, float3 outlineColor)
{
	float4 outlineTex = float4(0, 0, 0, 0);

	float xPoutline = uv.x + outlineWidth;
	float xMoutline = uv.x - outlineWidth;
	float yPoutline = uv.y + outlineWidth;
	float yMoutline = uv.y - outlineWidth;

	float add1 = step(xPoutline, 1);
	float add2 = step(0, xMoutline);
	float add3 = step(yPoutline, 1);
	float add4 = step(0, yMoutline);

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
	float absTex= abs(depthTex.Sample(smp, uv));
	float depth = pow(absTex, 10000);
	depth = depth * depth;
	float4 depthColor = float4(0, depth/2, depth, 1.0);

	return depthColor;
}

float ToneChange(float mainColor, float2 toeColor, float2 linearColor, float2 shoulderColor,
	float a1, float b1, float at, float bt, float ct, float as, float bs, float cs)
{
	float r_toe = (-a1 / (mainColor + b1) + ct) * step(mainColor, toeColor.x);
	float r_linear = (a1 * mainColor + b1) * step(toeColor.x, mainColor) * step(mainColor, linearColor.x);
	float r_shoulder = (-as / (mainColor + bs) + cs) * step(linearColor.x, mainColor) * step(mainColor, shoulderColor.x);

	float color = r_toe + r_linear + r_shoulder;

	return color;
}

float3 ToneMap(float3 mainColor, float2 toeColor, float2 linearColor, float2 shoulderColor)
{
	float a1 = (linearColor.y - toeColor.y) / (linearColor.x - toeColor.x);
	float b1 = toeColor.y - (a1 * toeColor.x);

	float f_a = toeColor.y - a1 * toeColor.x;
	float at = (a1 * toeColor.x * toeColor.x * toeColor.y * toeColor.y) / (f_a * f_a);
	float bt = (a1 * toeColor.x * toeColor.x) / f_a;
	float ct = (toeColor.y * toeColor.y) / f_a;

	float x2_x3 = linearColor.x - shoulderColor.x;
	float as = (a1 * x2_x3 * x2_x3 * (linearColor.y - shoulderColor.y) * (linearColor.y - shoulderColor.y)) /
		((a1 * x2_x3 - linearColor.y + shoulderColor.y) * (a1 * x2_x3 - linearColor.y + shoulderColor.y));
	float bs = (a1 * linearColor.x * (shoulderColor.x - linearColor.x) + shoulderColor.x * (linearColor.y - shoulderColor.y)) /
		(a1 * x2_x3 - linearColor.y + shoulderColor.y);
	float cs = (shoulderColor.y * (a1 * x2_x3 + linearColor.y) - (linearColor.y * linearColor.y)) /
		(a1 * x2_x3 - linearColor.y + shoulderColor.y);

	//�F�̒l�ɂ���Ďg���C���F��ύX
	float3 returnColor = float3(0, 0, 0);

	//�F���C��
	returnColor.x = ToneChange(mainColor.x, toeColor, linearColor, shoulderColor, a1, b1, at, bt, ct, as, bs, cs);
	returnColor.y = ToneChange(mainColor.y, toeColor, linearColor, shoulderColor, a1, b1, at, bt, ct, as, bs, cs);
	returnColor.z = ToneChange(mainColor.z, toeColor, linearColor, shoulderColor, a1, b1, at, bt, ct, as, bs, cs);

	return returnColor;
}