#include "ShaderIncludes.hlsli"

TextureCube cubeMapTexture : register(t0);
SamplerState samplerOptions : register(s0);

float4 main(VertexToPixelSky input) : SV_TARGET
{
	return cubeMapTexture.Sample(samplerOptions, input.sampleDir);
}