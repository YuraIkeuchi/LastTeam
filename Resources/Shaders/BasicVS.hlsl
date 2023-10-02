#include "Basic.hlsli"
float random(float2 st)
{
	return frac(sin(dot(st.xy,
		float2(12.9898, 78.233))) *
		43758.5453123);
}

float noise(float2 st)
{
	float2 i = floor(st);
	float2 f = frac(st);

	// Four corners in 2D of a tile
	float a = random(i);
	float b = random(i + float2(1.0, 0.0));
	float c = random(i + float2(0.0, 1.0));
	float d = random(i + float2(1.0, 1.0));

	float2 u = f * f * (3.0 - 2.0 * f);

	return lerp(a, b, u.x) +
		(c - a) * u.y * (1.0 - u.x) +
		(d - b) * u.x * u.y;
}

#define OCTAVES 6

float fbm(float2 st)
{
	// Initial values
	float value = 0.0;
	float amplitude = .5;
	float frequency = 0.;
	// Loop of octaves
	for (int i = 0; i < OCTAVES; i++)
	{
		value += amplitude * noise(st);
		st *= 2.;
		amplitude *= .5;
	}
	return value;
}

float pattern(float2 p, float4 scale_1, float scale_2, float4 add_1, float4 add_2)
{
	// first domain warping
	float2 q = float2(
		fbm(p + scale_1.x * add_1.xy),
		fbm(p + scale_1.y * add_1.zw)
		);

	// second domain warping
	float2 r = float2(
		fbm(p + scale_1.z * q + add_2.xy),
		fbm(p + scale_1.w * q + add_2.zw)
		);

	return fbm(p + scale_2 * r);
}

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// 法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mul(viewproj, world), pos);

	output.worldpos = mul(world, pos);
	output.normal = wnormal.xyz;


	//output.uv.y =uv.y+ uvval;
	float2 p = uv * 4;
	p = float2(fbm(p + uvval), fbm(p));
	p = p * 8.0;
	//	uv= fbm(p);
	if (uvval != 0)
	{
		uv -= fbm(p);
	}output.uv = uv;
	return output;
}
