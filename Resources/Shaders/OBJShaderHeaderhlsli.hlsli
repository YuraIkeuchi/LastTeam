cbuffer cbuff0:register(b0)
{
	float4 color;
	matrix mat;
	bool clips;
	float Cinter;
	float2 offset;
	float Tiling;//ƒ^ƒCƒŠƒ“ƒO
};

struct Output
{
	float4 svpos:SV_POSITION;
	float3 normal:NORMAL;
	float2 uv:TEXCOORD;
};
