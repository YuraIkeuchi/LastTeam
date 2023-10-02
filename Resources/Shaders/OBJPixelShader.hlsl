#include "OBJShaderHeaderhlsli.hlsli"
//#include"Header.hlsli"

//#include"header.hlsli"
Texture2D<float4> tex:register(t0);

SamplerState smp:register(s0);

//float4 main(output input) :sv_target
//{
//	return float4(input.uv,1,1);
//}]\
//
float3 BloomPixel(SamplerState smp, float2 uv, float2 texPixelSize)
{
	float2 uv2 = floor(uv / texPixelSize) * texPixelSize;
	uv2 += texPixelSize * 0.001f;
	float3 tl = max(tex.Sample(smp, uv2).rgb - 0.95f, 0.f);
	float3 tr = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, 0.f)).rgb - 0.95f, 0.f);
	float3 bl = max(tex.Sample(smp, uv2 + float2(0.f, texPixelSize.y)).rgb - 0.95f, 0.f);
	float3 br = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, texPixelSize.y)).rgb - 0.95f, 0.f);
	float2 f = frac(uv / texPixelSize);

	float3 tA = lerp(tl, tr, f.x);
	float3 tB = lerp(bl, br, f.x);

	return lerp(tA, tB, f.y);
}

#define bloomR 10

float3 getBloom(SamplerState smp, float2 uv, float2 texPixelSize)
{
	float3 bloom = float3(0.f, 0.f, 0.f);
	float2 offset = float2(1.f, 1.f) * texPixelSize * bloomR;
	bloom += BloomPixel(smp, uv + offset * float2(-1.f, -1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += BloomPixel(smp, uv + offset * float2(-1.f, 0.f), texPixelSize * bloomR) * 0.5f;
	bloom += BloomPixel(smp, uv + offset * float2(-1.f, 1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += BloomPixel(smp, uv + offset * float2(0.f, -1.f), texPixelSize * bloomR) * 0.5f;
	bloom += BloomPixel(smp, uv + offset * float2(0.f, 0.f), texPixelSize * bloomR) * 1.f;
	bloom += BloomPixel(smp, uv + offset * float2(0.f, 1.f), texPixelSize * bloomR) * 0.5f;
	bloom += BloomPixel(smp, uv + offset * float2(1.f, -1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += BloomPixel(smp, uv + offset * float2(1.f, 0.f), texPixelSize * bloomR) * 0.5f;
	bloom += BloomPixel(smp, uv + offset * float2(1.f, 1.f), texPixelSize * bloomR) * 0.292893f;
	bloom /= 4.171573f;
	return bloom;
}

float4 bloom(SamplerState smp, float2 uv, float intensity = 50.f)
{
	return float4(getBloom(smp, uv, 1.f) * intensity, 10.f);
}
float4 main(Output input) : SV_TARGET
{
	// テクスチャマッピング
float2 offsetscroll = float2(offset.x,offset.y);
	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 1.0f;
	float4 texcolor = float4(tex.Sample(smp, input.uv * Tiling + offsetscroll));

	if (clips) {
		//アンカーポイントできないうちはこれで範囲指定
		float v = input.uv.y > 0.5f ? 1
			: -1;
		clip(v);


		float CorrVal = 0.80f;
		texcolor.rgb += bloom(smp, input.uv);
		float SubAlpha = smoothstep(0, 1, input.uv.y);

		//奥行けば色濃くなるよ
	texcolor.w = abs(Cinter - input.uv.y)+(CorrVal-SubAlpha);
	//x補正込み 単色だと地味なんで
		texcolor.w *= abs(0.5f- input.uv.x)+0.7f;

	return float4(texcolor.rgb, texcolor.a) *color;
	}
	else
	{
		return float4(texcolor.rgb * brightness, texcolor.a) * color;
	}
	//return float4(input.normal,1);
	//return float4(tex.sample(smp,input.uv));
}
