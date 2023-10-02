#include"PostEffectTest.hlsli"
Texture2D<float4> tex0:register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);

struct TonemapParams
{
	float3 mToe;
	float2 mMid;
	float3 mShoulder;
	float2 mBx;
	float2 mBy; // only used for InvTonemap
};

float3 InvToneMap(TonemapParams tc, float3 y);
bool HasSingularity(float2 p1, float2 p2, float2 p3, TonemapParams tc);
void PrepareTonemapParams(float2 p1, float2 p2, float2 p3, out TonemapParams tc);



float3 ToneMap(const TonemapParams tc, float3 x)
{
	float3 toe = -tc.mToe.x / (x + tc.mToe.y) + tc.mToe.z;
	float3 mid = tc.mMid.x * x + tc.mMid.y;
	float3 shoulder = -tc.mShoulder.x / (x + tc.mShoulder.y) + tc.mShoulder.z;

	float3 result = lerp(toe, mid, step(tc.mBx.x, x));
	result = lerp(result, shoulder, step(tc.mBx.y, x));
	return result;
}

float3 InvToneMap(const TonemapParams tc, float3 y)
{
	float3 inv_toe = -tc.mToe.x / (y - tc.mToe.z) - tc.mToe.y;
	float3 inv_mid = (y - tc.mMid.y) / tc.mMid.x;
	float3 inv_shoulder = -tc.mShoulder.x / (y - tc.mShoulder.z) - tc.mShoulder.y;

	float3 result = lerp(inv_toe, inv_mid, step(tc.mBy.x, y));
	result = lerp(result, inv_shoulder, step(tc.mBy.y, y));
	return result;
}

bool HasSingularity(const float2 p1, const float2 p2, const float2 p3, const TonemapParams tc)
{
	float2 pointAtInfinity = float2(1e6, 1e6);

	bool hasSingularity = false;
	if (p1.x < 0.0 || p1.y < 0.0) { hasSingularity = true; }
	if (p2.x < p1.x || p2.y < p1.y) { hasSingularity = true; }
	if (p3.x < p2.x || p3.y < p2.y) { hasSingularity = true; }

	// toe -> mid
	if (0.0 < -tc.mToe.y && -tc.mToe.y <= p1.x) { hasSingularity = true; }

	// mid -> shoulder
	if (p2.x < -tc.mShoulder.y && -tc.mShoulder.y <= p3.x) { hasSingularity = true; }

	// shoulder -> infinity
	if (p3.x < -tc.mShoulder.y && -tc.mShoulder.y <= pointAtInfinity.x) { hasSingularity = true; }

	return hasSingularity;
}

void PrepareTonemapParams(float2 p1, float2 p2, float2 p3, out TonemapParams tc)
{
	float denom = p2.x - p1.x;
	denom = abs(denom) > 1e-5 ? denom : 1e-5;
	float slope = (p2.y - p1.y) / denom;

	{
		tc.mMid.x = slope;
		tc.mMid.y = p1.y - slope * p1.x;
	}

	{
		float denom = p1.y - slope * p1.x;
		denom = abs(denom) > 1e-5 ? denom : 1e-5;
		tc.mToe.x = slope * p1.x * p1.x * p1.y * p1.y / (denom * denom);
		tc.mToe.y = slope * p1.x * p1.x / denom;
		tc.mToe.z = p1.y * p1.y / denom;
	}

	{
		float denom = slope * (p2.x - p3.x) - p2.y + p3.y;
		denom = abs(denom) > 1e-5 ? denom : 1e-5;
		tc.mShoulder.x = slope * pow(p2.x - p3.x, 2.0) * pow(p2.y - p3.y, 2.0) / (denom * denom);
		tc.mShoulder.y = (slope * p2.x * (p3.x - p2.x) + p3.x * (p2.y - p3.y)) / denom;
		tc.mShoulder.z = (-p2.y * p2.y + p3.y * (slope * (p2.x - p3.x) + p2.y)) / denom;
	}

	tc.mBx = float2(p1.x, p2.x);
	tc.mBy = float2(p1.y, p2.y);
}

float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp, input.uv);
	float3 color = float3(0.0f, 0.0f, 0.0f);
	color = colortex0.rgb;
	float4 outputcolor;
	float2 p1 = P_1;
	float2 p2 = P_2;
	float2 p3 = P_3;


	TonemapParams tc;
	PrepareTonemapParams(p1, p2, p3, tc);
	color = pow(color, 2.2);
	color = ToneMap(tc, color);
	color = pow(color, 1.0 / 2.2);
	outputcolor = float4(color, 1.0);
	return outputcolor;
}
