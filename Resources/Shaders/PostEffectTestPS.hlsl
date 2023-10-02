#include"PostEffectTest.hlsli"
Texture2D<float4> tex0:register(t0); //0番スロットに設定されたテクスチャ
Texture2D<float4> tex1:register(t1); //1番スロットに設定されたテクスチャ
SamplerState smp:register(s0);
float4 RadBlur(float2 retPos, float2 offset)
{
	float4 colors[10];

	colors[0] = tex0.Sample(smp, retPos) * 0.19f;
	colors[1] = tex0.Sample(smp, retPos + offset) * 0.17f;
	colors[2] = tex0.Sample(smp, retPos + offset * 2.0f) * 0.15f;
	colors[3] = tex0.Sample(smp, retPos + offset * 3.0f) * 0.13f;
	colors[4] = tex0.Sample(smp, retPos + offset * 4.0f) * 0.11f;
	colors[5] = tex0.Sample(smp, retPos + offset * 5.0f) * 0.09f;
	colors[6] = tex0.Sample(smp, retPos + offset * 6.0f) * 0.07f;
	colors[7] = tex0.Sample(smp, retPos + offset * 7.0f) * 0.05f;
	colors[8] = tex0.Sample(smp, retPos + offset * 8.0f) * 0.03f;
	colors[9] = tex0.Sample(smp, retPos + offset * 9.0f) * 0.01f;
	//return float4(col.rgb, 1);
	float4 Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Color = (colors[0] + colors[1] + colors[2] + colors[3] + colors[4]
		+ colors[5] + colors[6] + colors[7] + colors[8] + colors[9]);

	return Color;
}

#define WINHEIGHT 1280
#define WINWIDTH 780
float4 main(VSOutput input) : SV_TARGET
{
	 float4 texcolor = tex0.Sample(smp,input.uv);
	////// //default
	//////// return float4(texcolor.rgb, 1);
	// //ぼかし
	// // 2.5テクセル分ずらすためのUV値を求める
	// float offsetU = 3.5f / 1280.0f;
	// float offsetV = 3.5f / 720.0f;

	// // 基準テクセルから右のテクセルのカラーをサンプリングする
	// texcolor += tex0.Sample(smp, input.uv + float2(offsetU, 0.0f));

	// // 基準テクセルから左のテクセルのカラーをサンプリングする
	// texcolor += tex0.Sample(smp, input.uv + float2(-offsetU, 0.0f));

	// // 基準テクセルから下のテクセルのカラーをサンプリングする
	// texcolor += tex0.Sample(smp, input.uv + float2(0.0f, offsetV));

	// // 基準テクセルから上のテクセルのカラーをサンプリングする
	// texcolor += tex0.Sample(smp, input.uv + float2(0.0f, -offsetV));

	// // 基準テクセルから右下のテクセルのカラーをサンプリングする
	// texcolor += tex0.Sample(smp, input.uv + float2(offsetU, offsetV));

	// // 基準テクセルから右上のテクセルのカラーをサンプリングする
	// texcolor += tex0.Sample(smp, input.uv + float2(offsetU, -offsetV));

	// // 基準テクセルから左下のテクセルのカラーをサンプリングする
	// texcolor += tex0.Sample(smp, input.uv + float2(-offsetU, offsetV));

	// // 基準テクセルから左上のテクセルのカラーをサンプリングする
	// texcolor += tex0.Sample(smp, input.uv + float2(-offsetU, -offsetV));

	// // 基準テクセルと近傍8テクセルの平均なので9で除算する
	// texcolor /= 9.0f;

	// return float4(texcolor.rgb,1);
		float2 ScreenSize = float2(WINWIDTH,WINHEIGHT);
	float2 vCenter = float2(RadCenter.x, RadCenter.y);
	float2 cpos = (input.uv * ScreenSize) - vCenter;
	float lens = length(cpos);

	// //ここまでがぼかし
	// //ボーダー柄
	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex0.Sample(smp, input.uv);
	float uzu = min(max(1.0 - (lens / 0), 0.0), 1.0) * 0;
	float x = cpos.x * cos(uzu) - cpos.y * sin(uzu);
	float y = cpos.x * sin(uzu) + cpos.y * cos(uzu);
	float2 retPos = (float2(x, y) + vCenter) / ScreenSize;


	float2 ViewportOffset = (float2(0.5, 0.5) / float2(WINWIDTH, WINHEIGHT));
	float2 center = float2(smoothstep(0, WINWIDTH, RadCenter.x), smoothstep(0, WINHEIGHT,RadCenter.y));
	float2 dir = center - input.uv;
	float len = length(dir);
	float2 offset = normalize(dir) * ViewportOffset;
	offset *= (RadPower * len);


	const int winH = 720, winW = 1280;
	float CenterX = input.svpos.x - winW/2;
	float CenterY = input.svpos.y -winH/2;

	float2 LeftUp = { CenterX - CloseIconRad / 2,CenterY-CloseIconRad/2 };
	float2 LeftDown = { CenterX - CloseIconRad / 2, CenterY + CloseIconRad / 2 };

	float2 RightUp= { CenterX + CloseIconRad / 2, CenterY - CloseIconRad / 2 };
	float2 RightDown= { CenterX + CloseIconRad / 2, CenterY + CloseIconRad / 2 };



	float v[4];
	v[0] = (CenterX+(winW - winH) > CloseIconRad) ? -1 : 1;
	v[1] = (CenterX - (winW - winH) <-CloseIconRad) ? -1 : 1;
	v[2] = (CenterY > CloseIconRad-(winW-winH)) ? -1 : 1;
	v[3] = (CenterY <-(CloseIconRad - (winW - winH))) ? -1 : 1;


	//for(int i=0;i<4;i++)
	clip(v[0]);
	clip(v[1]);
	clip(v[2]);
	clip(v[3]);
	//色反転
	float4 color =RadBlur(retPos, offset);

	return float4(color.rgb, 1);

}
