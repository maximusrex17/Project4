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
	float vsFloatScale;
	float vsRange;
}

struct OutputVertex {
	float4 pos : SV_POSITION; // System Value
	float2 uv : UV;
	float3 norm : NORMAL;
	float4 wPos : WORLDPOS;
};

float4 main(OutputVertex InputPixel) : SV_TARGET //System Value
{
	//float4 finalColor = 0;
	//
	//float4 lightDir = 0;
	//float4 lightRatio = 0;
	//float4 surfaceColor = 0;
	//
	//surfaceColor = txDiffuse.Sample(samLinear, InputPixel.uv);
	//lightDir = normalize(vsLightPos - InputPixel.wPos);
	//lightRatio = saturate(dot(lightDir.xyz, InputPixel.norm));
	//lightRatio = saturate(lightRatio + 1.0f);
	//finalColor *= txDiffuse.Sample(samLinear, InputPixel.uv);
	//finalColor = (lightRatio * vsLightColor * surfaceColor);
	//
	//return finalColor;
	float4 finalColor = 0;
	//do NdotL lighting for 2 lights

	finalColor.rgb = saturate(dot(-vsLightDir.rgb,InputPixel.norm) * vsLightColor.rgb) * txDiffuse.Sample(samLinear, InputPixel.uv);

	finalColor.a = 1;
	return finalColor;
}