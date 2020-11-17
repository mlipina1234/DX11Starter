#include "ShaderIncludes.hlsli"

cbuffer LightData : register(b0)
{
	DirectionalLight directionalLight1;
	DirectionalLight directionalLight2;
	DirectionalLight directionalLight3;
	PointLight pointLight1;
	float specularValue;
	float3 cameraPosition;
}

//Texture2D diffuseTexture	: register(t0);
//Texture2D normalMap		: register(t1);

Texture2D Albedo		: register(t0);
Texture2D NormalMap		: register(t1);
Texture2D RoughnessMap	: register(t2);
Texture2D MetalnessMap	: register(t3);


SamplerState samplerOptions : register(s0);

float4 main(VertexToPixelNormalMap input) : SV_TARGET
{
	float3 unpackedNormal = NormalMap.Sample(samplerOptions, input.uv).rgb * 2 - 1;

	float3 N = input.normal;
	float3 T = input.tangent;
	T = normalize(T - N * dot(T, N));
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);
	
	input.normal = mul(unpackedNormal, TBN);

	input.normal = normalize(input.normal);

	//float4 surfaceColor = diffuseTexture.Sample(samplerOptions, input.uv);
	float3 surfaceColor = pow(Albedo.Sample(samplerOptions, input.uv).rgb, 2.2f);

	// PBR things
	float roughness = RoughnessMap.Sample(samplerOptions, input.uv).r;
	float metalness = MetalnessMap.Sample(samplerOptions, input.uv).r;
	float3 specularColor = lerp(F0_NON_METAL.rrr, surfaceColor.rgb, metalness);

	float3 totalColor = (
		ComputeDirectionalLightColor(input.worldPos, input.normal, directionalLight1, cameraPosition, roughness, metalness, specularColor, surfaceColor)
		+ ComputeDirectionalLightColor(input.worldPos, input.normal, directionalLight2, cameraPosition, roughness, metalness, specularColor, surfaceColor)
		+ ComputeDirectionalLightColor(input.worldPos, input.normal, directionalLight3, cameraPosition, roughness, metalness, specularColor, surfaceColor)
		+ ComputePointLightColor(input.worldPos, input.normal, pointLight1, cameraPosition, roughness, metalness, specularColor, surfaceColor)
		) * surfaceColor * input.color;
	return float4(pow(totalColor, 1.0f / 2.2f), 1);
}