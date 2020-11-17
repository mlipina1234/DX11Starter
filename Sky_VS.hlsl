#include "ShaderIncludes.hlsli"

cbuffer ExternalData : register(b0)
{
	matrix view;
	matrix proj;
}

VertexToPixelSky main( VertexShaderInput input )
{
	VertexToPixelSky output;
	matrix viewNoTranslation = view;
	viewNoTranslation._14 = 0;
	viewNoTranslation._24 = 0;
	viewNoTranslation._34 = 0;
	matrix vp = mul(proj, viewNoTranslation);
	output.position = mul(vp, float4(input.position, 1.0f));

	// will force the z to be 1 after perspective divide occurs in the rasterizer
	output.position.z = output.position.w;

	output.sampleDir = input.position;
	return output;
}