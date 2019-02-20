Texture2D txDiffuse : register(t0);
Texture2D txRough : register(t1);
Texture2D txNorm : register(t2);
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
	float4 tangent : TANGENT;
	float4 biTangent : BITANGENT;
};

float4 main(OutputVertex InputPixel) : SV_TARGET //System Value
{
	float4 finalColor = 0;
	
	float4 lightDir = 0;
	float4 lightRatio = 0;
	float4 surfaceColor = 0;
	float4 roughColor = 0;
	float4 roughSurfaceColor = 0;
	float4 surfaceNorm = 0;
	float3x3 TBN;

	surfaceNorm = txNorm.Sample(samLinear, InputPixel.uv);
	surfaceNorm = (surfaceNorm * 2.0f) - 1.0f;

	//InputPixel.norm = normalize(InputPixel.norm);
	//InputPixel.tangent = normalize(InputPixel.tangent);
	//InputPixel.biTangent = normalize(InputPixel.biTangent);

	TBN[0] = InputPixel.tangent.xyz;
	TBN[1] = InputPixel.biTangent.xyz;
	TBN[2] = InputPixel.norm.xyz;

	surfaceNorm.xyz = mul(surfaceNorm.xyz, TBN);

	surfaceColor = txDiffuse.Sample(samLinear, InputPixel.uv);
	roughColor = txRough.Sample(samLinear, InputPixel.uv);
	lightDir = normalize(vsLightPos - InputPixel.wPos);
	lightRatio = saturate(dot(lightDir.xyz, surfaceNorm.xyz));
	lightRatio = saturate(lightRatio + roughColor);
	finalColor = (lightRatio * vsLightColor * surfaceColor);
	return finalColor;
	return surfaceNorm;
	//float4 finalColor = 0;
	////do NdotL lighting for 2 lights

	//finalColor.rgb = saturate(dot(-vsLightDir.rgb,InputPixel.norm) * vsLightColor.rgb) * txDiffuse.Sample(samLinear, InputPixel.uv);

	//finalColor.a = 1;
	//return finalColor;
}