#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float2 offsetscroll = float2(offset.x, offset.y);
	return tex.Sample(smp, input.uv + offsetscroll) * color;
}
