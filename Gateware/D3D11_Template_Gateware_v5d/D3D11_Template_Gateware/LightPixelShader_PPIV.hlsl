Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0) {
	matrix vsWorld;
	matrix vsView;
	matrix vsProjection;
	matrix vsRotateY;
	float4 vsLightPos;
	float4 vsLightDir;
	float4 vsLightColor;
	float4 vsOutputColor;
}

struct OutputVertex {
	float4 pos : SV_POSITION; // System Value
	float2 uv : UV;
	float3 norm : NORMAL;
};

float4 main(OutputVertex InputPixel) : SV_TARGET //System Value
{
	float4 finalColor = float4(0,0,0,0);

	//Directional Light
	finalColor = txDiffuse.Sample(samLinear, InputPixel.uv);	
	float dotResult = saturate(dot(-vsLightDir.xyz, normalize(InputPixel.norm)));
	finalColor = saturate(finalColor * dotResult);

	return finalColor;
}