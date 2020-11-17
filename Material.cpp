#include "Material.h"

Material::Material(
	DirectX::XMFLOAT4 tintInit, 
	float specularityInit, 
	SimplePixelShader* pixelShaderInit, 
	SimpleVertexShader* vertexShaderInit,
	ID3D11ShaderResourceView* srvInit,
	ID3D11SamplerState* samplerStateInit,
	ID3D11ShaderResourceView* srvNormalInit,
	ID3D11ShaderResourceView* srvRoughnessInit,
	ID3D11ShaderResourceView* srvMetalnessInit)
{
	colorTint = tintInit;
	pixelShader = pixelShaderInit;
	vertexShader = vertexShaderInit;
	specularity = specularityInit;
	srv = srvInit;
	samplerState = samplerStateInit;
	srvNormal = srvNormalInit;
	srvRoughness = srvRoughnessInit;
	srvMetalness = srvMetalnessInit;
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
	return colorTint;
}

float Material::GetSpecularity() const 
{
	return specularity;
}

void Material::SetColorTint(DirectX::XMFLOAT4 value)
{
	colorTint = value;
}

ID3D11ShaderResourceView* Material::GetSRV()
{
	return srv;
}
ID3D11SamplerState* Material::GetSamplerState()
{
	return samplerState;
}
ID3D11ShaderResourceView* Material::GetNormalSRV()
{
	return srvNormal;
}
ID3D11ShaderResourceView* Material::GetRoughnessSRV()
{
	return srvRoughness;
}
ID3D11ShaderResourceView* Material::GetMetalnessSRV()
{
	return srvMetalness;
}