Texture2D txDiffuse : register(t0);
Texture2D txRough : register(t1);
SamplerState samLinear : register(s1);

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

struct OutputVertex {
	float4 pos : SV_POSITION; // System Value
	float2 uv : UV;
	float3 norm : NORMAL;
	float4 wPos : WORLDPOS;
};

float4 main(OutputVertex InputPixel) : SV_TARGET //System Value
{
	float4 finalColor = 0;
	
	float4 lightDir = 0;
	float4 lightRatio = 0;
	float4 surfaceColor = 0;
	float4 roughColor = 0;
	float4 roughSurfaceColor = 0;
	
	surfaceColor = txDiffuse.Sample(samLinear, InputPixel.uv);
	roughColor = txRough.Sample(samLinear, InputPixel.uv);
	roughSurfaceColor = roughColor * (surfaceColor * 1.5f);
	lightDir = normalize(vsLightPos - InputPixel.wPos);
	lightRatio = saturate(dot(lightDir.xyz, InputPixel.norm));
	lightRatio = saturate(lightRatio + 1.0f);
	finalColor = (lightRatio * vsLightColor * roughSurfaceColor);
	
	return finalColor;
	//float4 finalColor = 0;
	////do NdotL lighting for 2 lights

	//finalColor.rgb = saturate(dot(-vsLightDir.rgb,InputPixel.norm) * vsLightColor.rgb) * txDiffuse.Sample(samLinear, InputPixel.uv);

	//finalColor.a = 1;
	//return finalColor;
}