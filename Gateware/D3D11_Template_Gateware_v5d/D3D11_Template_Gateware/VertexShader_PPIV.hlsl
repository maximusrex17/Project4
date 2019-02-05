//Rule of Three
//Three things must match
//1. C++ Vetex Struct
//2. Input Layout
//3. HLSL Vertex Struct
Texture2D heightTexture : register(t0);
SamplerState heightSampler : register(s0);

cbuffer ConstantBuffer : register(b0) {
	matrix vsWorld;
	matrix vsView;
	matrix vsProjection;
	matrix vsRotateY;
	float4 vsLightPos;
	float4 vsLightPos1;
	float4 vsLightDir;
	float4 vsLightDir1;
	float4 vsLightColor;
	float4 vsLightColor1;
	float4 vsOutputColor;
}

struct InputVertex {
	float4 pos : POSITION;
	float2 uv : UV;
	float3 norm : NORMAL;
	float height : HEIGHT;
};

struct OutputVertex {
	float4 pos : SV_POSITION; // System Value
	float2 uv : UV;
	float3 norm : NORMAL;
	float4 wPos : WORLDPOS;
};



OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;
	output.pos = mul(input.pos, vsWorld);
	input.height = heightTexture.GatherRed(heightSampler, input.uv);
	output.pos = float4(output.pos.x, output.pos.y + (input.height * 0.1f), output.pos.z, output.pos.w);
	output.wPos = output.pos;
	output.pos = mul(output.pos, vsView);
	output.pos = mul(output.pos, vsProjection);
	output.norm = normalize(mul(float4(input.norm, 0), vsWorld).xyz);
	output.uv = input.uv;
	return output;
}