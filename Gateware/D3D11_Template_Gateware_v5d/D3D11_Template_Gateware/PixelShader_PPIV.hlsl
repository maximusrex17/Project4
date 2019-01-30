Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

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

struct OutputVertex {
	float4 pos : SV_POSITION; // System Value
	float2 uv : UV;
	float3 norm : NORMAL;
	float4 wPos : WORLDPOS;
};

float4 main(OutputVertex InputPixel) : SV_TARGET //System Value
{
	float4 finalColor = 0;
	float4 pointFinalColor = 0;
	float4 spotFinalColor = 0;

	float4 pointLightDir = 0;
	float4 spotLightDir = 0;
	float4 pointLightRatio = 0;
	float4 spotLightRatio = 0;
	float4 surfaceColor = 0;
	float4 spotFactor = 0;
	float4 surfaceRatio = 0;
	float4 coneRatio = 0;
	coneRatio = 0.15f;

	//Point Light
	surfaceColor = txDiffuse.Sample(samLinear, InputPixel.uv);
	pointLightDir = normalize(vsLightPos - InputPixel.wPos);
	pointLightRatio = saturate(dot(pointLightDir.xyz, InputPixel.norm));	
	pointFinalColor = (pointLightRatio * vsLightColor * surfaceColor);

	//SpotLight
	spotLightDir = normalize(vsLightPos1 - InputPixel.wPos);
	surfaceRatio = saturate(dot(-spotLightDir.xyz, vsLightDir1));
	spotLightRatio = normalize(vsLightPos1 - InputPixel.wPos);
	spotFactor = (surfaceRatio > coneRatio) ? 1 : 0;
	spotFinalColor = (spotFactor * spotLightRatio * vsLightColor1 * surfaceColor);
	finalColor = pointFinalColor + spotFinalColor;

	return finalColor;
}