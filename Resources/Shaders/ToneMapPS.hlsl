//#include"PostEffectTest.hlsli"
//Texture2D<float4>tex0:register(t0);//0番スロットに設定されたテクスチャ
//SamplerState smp:register(s0);
//
//
//float4 ColorSpaceConvert(float4 color);
//float4 ToneMapping(float4 color);
//float4 ApplyOETF(float4 color);
//float3 ReinhardTone(float3 color);
//float3 GtTone(float3 color);
//float3 OETF_BT709(float3 color);
//float3 OETF_BT2100(float3 color);
//float4 main(VSOutput input) : SV_TARGET
//{
//
//    float4 colortex0 = tex0.Sample(smp,input.uv);
//
//    float4 l_color = colortex0;
//
//    //色空間の変換
//    l_color = ColorSpaceConvert(l_color);
//    //トーンマッピング
//    l_color = ToneMapping(l_color);
//
//    //OETF適用
//    l_color = ApplyOETF(l_color);
//
//    return float4(l_color.rgb, 1);
//
//}
//
////色変換
//float4 ColorSpaceConvert(float4 color) {
//    float4 result = 0;
//
//    switch (ColorSpace) {
//        //そのまま色を返す
//        case 0:
//        {
//            result = color;
//        }
//    break;
//        case 1:
//        {
//            static const float3x3 conversion =
//            {
//                0.627404f,0.329283f,0.043313f,
//                0.069097f,0.919540f,0.011362f,
//                0.016391f,0.088013f,0.895595f,
//            };
//            result.rgb = mul(conversion, color.rgb);
//        }
//
//        break;
//    }
//  
//    return float4(result.rgb, 1);
//}
//
////トーンマッピング
//float4 ToneMapping(float4 color) {
//    float4 result = 0;
//
//   
//    switch (ToneType) {
//        //トーンなし
//    case 0:
//        result.rgb = color.rgb;
//        break;
//        //Reinhardトーン
//    case 1:
//        result.rgb = ReinhardTone(color.rgb);
//        break;
//    case 2:
//        //GTトーン
//        result.rgb = GtTone(color.rgb);
//        break;
//    }
//
//    return float4(result.rgb, 1);
//}
//
////Reinhartトーン
//float3 ReinhardTone(float3 color) {
//    float Lz = MaxLuminance / BaseLuminance;
//    float k = BaseLuminance * Lz / (BaseLuminance - Lz);
//
//    return float4(color.rgb * k / (color.rgb + float3(k, k, k)), 1);
//}
//
////GTトーン
//float3 GtTone(float3 color) {
//    float k = MaxLuminance / BaseLuminance;
//    float P = k;
//    float a = 1.0f;
//    float m = 0.22f;
//    float l = 0.4f;
//    float c = 1.33f;
//    float b = 0.0f;
//
//    float3 x = color.rgb;
//    float l0 = ((P - m) * l) / a;
//    float l1 = m - (m / a);
//    float l2 = m + ((1.0f - m) / a);
//
//    float s0 = m + l0;
//    float s1 = m + a * l0;
//    float c2 = (a * P) / (P - s1);
//    float cp = -c2 / P;
//
//    float3 w0 = 1.0f - smoothstep(0.0f, m, x);
//    float3 w2 = step(m + l0, x);
//    float3 w1 = 1.0f - w0 - w2;
//
//    float3 T = m * pow(x / m, c) + b;
//    float3 S = P - (P - s1) * exp(cp * (x - s0));
//    float3 L = m + a * (x - m);
//
//    return T * w0 + L * w1 + S * w2;
//}
//
////OETF適用
//float4 ApplyOETF(float4 color) {
//    float4 result = 0;
//
//    switch (ColorSpace) {
//        //そのまま色を返す
//    case 0:
//        result.rgb = OETF_BT709(color.rgb);
//        break;
//    case 1:
//        result.rgb = OETF_BT2100(color.rgb);
//        break;
//    }
//    return float4(result.rgb, 1);
//}
//
//float3 OETF_BT709(float3 color) {
//    float3 result;
//    result.r = (color.r <= 0.018f) ? 4.500 * color.r : (1.0f + 0.099) * pow(abs(color.r), 0.45f) - 0.099f;
//    result.g = (color.r <= 0.018f) ? 4.500 * color.g : (1.0f + 0.099) * pow(abs(color.g), 0.45f) - 0.099f;
//    result.b = (color.r <= 0.018f) ? 4.500 * color.b : (1.0f + 0.099) * pow(abs(color.b), 0.45f) - 0.099f;
//    
//    return result;
//}
//
//float3 OETF_BT2100(float3 color) {
//    float m1 = 2610.0 / 4096.0 / 4;
//    float m2 = 2523.0 / 4096.0 * 128;
//    float c1 = 3424.0 / 4096.0;
//    float c2 = 2413.0 / 4096.0 * 32;
//    float c3 = 2392.0 / 4096.0 * 32;
//
//    float3 cp = pow(abs(color.rgb), m1);
//    return pow((c1 + c2 * cp) / (1 + c3 * cp), m2);
//}
