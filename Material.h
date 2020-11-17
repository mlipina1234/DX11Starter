#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>

class Material
{
	DirectX::XMFLOAT4 colorTint;
	float specularity;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* srvNormal;
	ID3D11ShaderResourceView* srvRoughness;
	ID3D11ShaderResourceView* srvMetalness;
	ID3D11SamplerState* samplerState;

public:
	Material(
		DirectX::XMFLOAT4 tintInit, 
		float specularityInit, 
		SimplePixelShader* pixelShaderInit, 
		SimpleVertexShader* vertexShaderInit,
		ID3D11ShaderResourceView* srvInit,
		ID3D11SamplerState* samplerStateInit,
		ID3D11ShaderResourceView* srvNormalInit,
		ID3D11ShaderResourceView* srvRoughnessInit,
		ID3D11ShaderResourceView* srvMetalnessInit
	);

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	DirectX::XMFLOAT4 GetColorTint();
	float GetSpecularity() const;
	void SetColorTint(DirectX::XMFLOAT4 value);
	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView* GetNormalSRV();
	ID3D11ShaderResourceView* GetRoughnessSRV();
	ID3D11ShaderResourceView* GetMetalnessSRV();
	ID3D11SamplerState* GetSamplerState();
};

