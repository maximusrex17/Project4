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
	float vsTime;
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
	//CubeLookUpX += sin(CubeLookUpY * 200 + (TIME * 5)) * 0.005 * (CubeLookUpY - 1);
	output.pos.x += sin(output.pos.y * 200 + (vsTime * 5)) * 0.0025f * (output.pos.y - 10);
	output.pos.y += sin(output.pos.x * 200 + (vsTime * 5)) * 0.0025f * (output.pos.x - 10);
	//input.height = heightTexture.GatherRed(heightSampler, input.uv);
	//output.pos.xyz += (input.height*vsFloatScale);
	output.wPos = output.pos;
	output.pos = mul(output.pos, vsView);
	output.pos = mul(output.pos, vsProjection);
	
	output.norm = mul(float4(input.norm, 0), vsWorld).xyz;
	//output.tangent = mul(float4(input.tangent.xyz * input.tangent.w, 0), vsWorld);
	//output.biTangent = mul(float4(cross(input.norm.xyz, input.tangent.xyz), 0), vsWorld);
	output.uv = input.uv;
	return output;
}