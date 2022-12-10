#include "ShrinkBuffer.hlsli"

Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//輝度値算出
	if (isLuminance)
	{
		const float3 RGB2Y = float3(+0.29900f, +0.58700f, +0.11400f);
		const float EPSILON = 0.00001;
		float4 output;

		float l0 = dot(RGB2Y, tex.Sample(smp, input.uv0));
		float l1 = dot(RGB2Y, tex.Sample(smp, input.uv1));
		float l2 = dot(RGB2Y, tex.Sample(smp, input.uv4));
		float l3 = dot(RGB2Y, tex.Sample(smp, input.uv5));

		float l_max = max(max(l0, l1), max(l2, l3));

		float total = log(EPSILON + l0) + log(EPSILON + l1) + log(EPSILON + l2) + log(EPSILON + l3);

		output.r = l_max;
		output.gba = 0.25f * total;

		return output;
	}

	float4 output;

	float4 t0 = tex.Sample(smp, input.uv0);
	float4 t1 = tex.Sample(smp, input.uv1);
	float4 t2 = tex.Sample(smp, input.uv2);
	float4 t3 = tex.Sample(smp, input.uv3);
	float4 t4 = tex.Sample(smp, input.uv4);
	float4 t5 = tex.Sample(smp, input.uv5);
	float4 t6 = tex.Sample(smp, input.uv6);
	float4 t7 = tex.Sample(smp, input.uv7);
	float4 t8 = tex.Sample(smp, input.uv0 + float2(0,2.0 * ratio));
	float4 t9 = tex.Sample(smp, input.uv1 + float2(0,2.0 * ratio));
	float4 ta = tex.Sample(smp, input.uv2 + float2(0,2.0 * ratio));
	float4 tb = tex.Sample(smp, input.uv3 + float2(0,2.0 * ratio));
	float4 tc = tex.Sample(smp, input.uv4 + float2(0,2.0 * ratio));
	float4 td = tex.Sample(smp, input.uv5 + float2(0,2.0 * ratio));
	float4 te = tex.Sample(smp, input.uv6 + float2(0,2.0 * ratio));
	float4 tf = tex.Sample(smp, input.uv7 + float2(0,2.0 * ratio));

	//float l_max = max(t0.r, t1.r);
	//l_max = max(l_max, t2.r);
	//l_max = max(l_max, t3.r);
	//l_max = max(l_max, t4.r);
	//l_max = max(l_max, t5.r);
	//l_max = max(l_max, t6.r);
	//l_max = max(l_max, t7.r);
	//l_max = max(l_max, t8.r);
	//l_max = max(l_max, t9.r);
	//l_max = max(l_max, ta.r);
	//l_max = max(l_max, tb.r);
	//l_max = max(l_max, tc.r);
	//l_max = max(l_max, td.r);
	//l_max = max(l_max, te.r);
	//l_max = max(l_max, tf.r);

	float l_max = max(max(max(max(t0.r,t1.r),max(t2.r,t3.r)),
	max(max(t4.r,t5.r),max(t6.r,t7.r))),
	max(max(max(t8.r,t9.r),max(ta.r,tb.r)),
	max(max(tc.r,td.r),max(te.r,tf.r))));

	output.r = l_max;
	output.gba = (t0.gba + t1.gba + t2.gba + t3.gba +
		t4.gba + t5.gba + t6.gba + t7.gba + t8.gba + t9.gba + ta.gba + tb.gba +
		tc.gba + td.gba + te.gba + tf.gba)/16.0;

	return output;
}