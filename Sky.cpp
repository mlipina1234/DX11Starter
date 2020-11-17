#include "Sky.h"

using namespace DirectX;

Sky::Sky(
	Mesh* mesh,
	ID3D11SamplerState* samplerState,
	const wchar_t* cubemapTexturePath,
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	const wchar_t* vertexShaderFile,
	const wchar_t* pixelShaderFile
)
{
	this->mesh = mesh;
	this->samplerState = samplerState;
	this->depthStencilState = depthStencilState;

	D3D11_RASTERIZER_DESC skyboxRasterizerDesc = {};
	skyboxRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	skyboxRasterizerDesc.CullMode = D3D11_CULL_FRONT;
	device->CreateRasterizerState(&skyboxRasterizerDesc, &rasterizerState);

	D3D11_DEPTH_STENCIL_DESC skyStencilDesc = {};
	skyStencilDesc.DepthEnable = true;
	skyStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&skyStencilDesc, &depthStencilState);

	CreateDDSTextureFromFile(
		device,
		deviceContext,
		cubemapTexturePath,
		nullptr,
		&cubemapTextureSRV
	);

	pixelShader = new SimplePixelShader(
		device,
		deviceContext,
		pixelShaderFile
	);
	vertexShader = new SimpleVertexShader(
		device,
		deviceContext,
		vertexShaderFile
	);
}

Sky::~Sky()
{
	delete mesh;
	delete vertexShader;
	delete pixelShader;
}

void Sky::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, Camera* camera)
{
	 // change render states
	deviceContext->RSSetState(rasterizerState.Get());
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

	// prepare shaders
	vertexShader->SetShader();
	pixelShader->SetShader();

	pixelShader->SetShaderResourceView("cubeMapTexture", cubemapTextureSRV.Get());
	pixelShader->SetSamplerState("samplerOptions", samplerState.Get());
	pixelShader->CopyAllBufferData();

	vertexShader->SetMatrix4x4("view", camera->GetViewMatrix());
	vertexShader->SetMatrix4x4("proj", camera->GetProjectionMatrix());
	vertexShader->CopyAllBufferData();

	// render skybox
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,
		0);
	deviceContext->RSSetState(0);
	deviceContext->OMSetDepthStencilState(0, 0);
}