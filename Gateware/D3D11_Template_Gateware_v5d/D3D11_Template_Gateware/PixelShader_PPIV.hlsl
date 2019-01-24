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
	float4 finalColor = 0;

	float4 lightDir = 0;
	float4 lightRatio = 0;

	lightDir = normalize(vsLightPos - InputPixel.pos);
	lightRatio = saturate(dot(lightDir.xyz, normalize(InputPixel.norm)));
	finalColor = (lightRatio * vsLightColor * vsOutputColor);

	return finalColor;
}