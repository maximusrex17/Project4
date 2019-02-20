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
	float4 vsLightDir;
	float4 vsLightColor;
	float vsFloatScale;
	float vsRange;
}

struct InputVertex {
	float4 pos : POSITION;
	float2 uv : UV;
	float3 norm : NORMAL;
	float height : HEIGHT;
	float4 tangent : TANGENT;
	float4 biTangent : BITANGENT;
};

struct OutputVertex {
	float4 pos : SV_POSITION; // System Value
	float2 uv : UV;
	float3 norm : NORMAL;
	float4 wPos : WORLDPOS;
	float4 tangent : TANGENT;
	float4 biTangent : BITANGENT;
};



OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;
	output.pos = mul(input.pos, vsWorld);
	input.height = heightTexture.GatherRed(heightSampler, input.uv);
	output.pos.xyz += (input.height*vsFloatScale);
	//output.pos = float4(output.pos.x + (input.height * 0.05f), output.pos.y + (input.height * 0.05f), output.pos.z + (input.height * 0.05f), output.pos.w);
	output.wPos = output.pos;
	output.pos = mul(output.pos, vsView);
	output.pos = mul(output.pos, vsProjection);
	output.norm = normalize(mul(float4(input.norm, 0), vsWorld).xyz);
	output.tangent = normalize(mul(input.tangent, vsWorld));
	output.biTangent = normalize(mul(input.biTangent, vsWorld));
	output.uv = input.uv;
	return output;
}