#include "Gaussian.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	////ぼかし↓
	//float totalWeight = 0, _Sigma = 0.0025, _StepWidth = 0.001;
	//float4 col = float4(0, 0, 0, 0);

	//for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth) //xyで2aの幅で色を取得
	//{
	//	for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
	//	{
	//		float2 pickUV = input.uv + float2(px, py);
	//		float weight = Gaussian(input.uv, pickUV, _Sigma);
	//		col += tex0.Sample(smp, pickUV) * weight;//Gaussianで取得した重みを色にかける

	//		totalWeight += weight;//かけた重みの合計値を求める
	//	}
	//}

	//col.rgb = col.rgb / totalWeight; //かけた重み分割る
	//return float4(col.rgb, 1);
	float4 texcolor = tex0.Sample(smp, input.uv);
	//// //default
	////// return float4(texcolor.rgb, 1);
	//ぼかし
	// 2.5テクセル分ずらすためのUV値を求める
	float offsetU = 3.5f / 1280.0f;
	float offsetV = 3.5f / 720.0f;

	// 基準テクセルから右のテクセルのカラーをサンプリングする
	texcolor += tex0.Sample(smp, input.uv + float2(offsetU, 0.0f));

	// 基準テクセルから左のテクセルのカラーをサンプリングする
	texcolor += tex0.Sample(smp, input.uv + float2(-offsetU, 0.0f));

	// 基準テクセルから下のテクセルのカラーをサンプリングする
	texcolor += tex0.Sample(smp, input.uv + float2(0.0f, offsetV));

	// 基準テクセルから上のテクセルのカラーをサンプリングする
	texcolor += tex0.Sample(smp, input.uv + float2(0.0f, -offsetV));

	// 基準テクセルから右下のテクセルのカラーをサンプリングする
	texcolor += tex0.Sample(smp, input.uv + float2(offsetU, offsetV));

	// 基準テクセルから右上のテクセルのカラーをサンプリングする
	texcolor += tex0.Sample(smp, input.uv + float2(offsetU, -offsetV));

	// 基準テクセルから左下のテクセルのカラーをサンプリングする
	texcolor += tex0.Sample(smp, input.uv + float2(-offsetU, offsetV));

	// 基準テクセルから左上のテクセルのカラーをサンプリングする
	texcolor += tex0.Sample(smp, input.uv + float2(-offsetU, -offsetV));

	// 基準テクセルと近傍8テクセルの平均なので9で除算する
	texcolor /= 9.0f;

	return float4(texcolor.rgb, 1);
}
