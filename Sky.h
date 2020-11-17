#pragma once
#include "DXCore.h"
#include "Mesh.h"
#include "SimpleShader.h"
#include "Camera.h"
#include "DDSTextureLoader.h"

#include <DirectXMath.h>
#include <wrl/client.h>

class Sky
{
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubemapTextureSRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Mesh* mesh;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

public:
	Sky(
		Mesh* mesh, 
		ID3D11SamplerState* samplerState, 
		const wchar_t* cubemapTexturePath,
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		const wchar_t* vertexShaderFile, 
		const wchar_t* pixelShaderFile
	);

	~Sky();

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, Camera* camera);
};

