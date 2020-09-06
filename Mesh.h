#pragma once

#include "DXCore.h"
#include "d3d11.h"
#include "Vertex.h"
#include <wrl/client.h>

class Mesh
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int vertexCount;
	int indexCount;

public:
	Mesh(
		Vertex* vertexArray, 
		int vertexArrayCount,
		unsigned int* indexArray,
		int indexArrayCount,
		Microsoft::WRL::ComPtr<ID3D11Device> device);

	const Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();

	const Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();

	const int GetIndexCount();
};