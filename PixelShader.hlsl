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

Texture2D Albedo	: register(t0);// "t" registers
Texture2D RoughnessMap	: register(t1);
Texture2D MetalnessMap	: register(t2);


SamplerState samplerOptions : register(s0);// "s" registers

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	
	float3 surfaceColor = pow( Albedo.Sample(samplerOptions, input.uv).rgb, 2.2f );
	//float3 surfaceColor = diffuseTexture.Sample(samplerOptions, input.uv).rgb;
	//return float4(lightingAmount, lightingAmount, lightingAmount, 1);
	
	// PBR things
	float roughness = RoughnessMap.Sample(samplerOptions, input.uv).r;
	roughness = 0.2f;
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