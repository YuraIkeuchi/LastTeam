cbuffer cbuff0 : register(b0)
{
float3 Color; //色
float addsepia; //セピア
bool isTone; //トーンの有無
float frame; //水面表現
float2 P_1;
float2 P_2;
float2 P_3;
float RadPower;
float2 RadCenter;
float CloseIconRad;
};

//頂点シェーダーからの出力構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv :TEXCOORD;
};
