Texture2D txDiffuse : register(t0);
Texture2D txRough : register(t1);
Texture2D txNorm : register(t2);
SamplerState samLinear : register(s0);

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
	float surfaceRatio = 0;
	float innerConeRatio = 0.99f;
	float outerConeRatio = 0.89f;
	float coneAttenuation = 0;
	float rangeAttenuation = 0;
	float spotFactor = 0;
	float3x3 TBN;

	surfaceNorm = txNorm.Sample(samLinear, InputPixel.uv);
	surfaceNorm = (surfaceNorm * 2.0f) - 1.0f;

	//InputPixel.norm = normalize(InputPixel.norm);
	//InputPixel.tangent = normalize(InputPixel.tangent);
	//InputPixel.biTangent = normalize(InputPixel.biTangent);

	TBN[0] = InputPixel.tangent.xyz;
	TBN[1] = InputPixel.biTangent.xyz;
	TBN[2] = InputPixel.norm.xyz;

	//surfaceNorm.xyz = mul(surfaceNorm.xyz, TBN);

	surfaceColor = txDiffuse.Sample(samLinear, InputPixel.uv);
	roughColor = txRough.Sample(samLinear, InputPixel.uv);

	//Spot Light
	lightDir = normalize(vsLightPos - InputPixel.wPos);
	surfaceRatio = saturate(dot(-lightDir, vsLightDir));
	coneAttenuation = 1.0f - saturate((innerConeRatio - surfaceRatio) / (innerConeRatio - outerConeRatio));
	rangeAttenuation = 1.0f - saturate(length(vsLightPos - InputPixel.wPos) / vsRange);
	spotFactor = (surfaceRatio > outerConeRatio) ? 1 : 0;
	lightRatio = saturate(dot(lightDir.xyz, surfaceNorm.xyz));
	lightRatio = saturate(lightRatio + 1.0f);
	finalColor = (lightRatio * vsLightColor * surfaceColor * spotFactor * coneAttenuation * rangeAttenuation);
	return finalColor;
	return surfaceNorm;
	//float4 finalColor = 0;
	////do NdotL lighting for 2 lights

	//finalColor.rgb = saturate(dot(-vsLightDir.rgb,InputPixel.norm) * vsLightColor.rgb) * txDiffuse.Sample(samLinear, InputPixel.uv);

	//finalColor.a = 1;
	//return finalColor;
}