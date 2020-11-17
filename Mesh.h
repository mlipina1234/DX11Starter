#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include "d3d11.h"
#include "Vertex.h"
#include <fstream>
#include <vector>
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

	Mesh(
		const char* fileName,
		Microsoft::WRL::ComPtr<ID3D11Device> device);

	const Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();

	const Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();

	const int GetIndexCount();

	void CreateBuffers(Vertex* vertexArray,
		int vertexArrayCount,
		unsigned int* indexArray,
		int indexArrayCount,
		Microsoft::WRL::ComPtr<ID3D11Device> device);

	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
};