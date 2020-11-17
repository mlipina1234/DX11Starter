#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "Entity.h"
#include "Vertex.h"
#include "Camera.h"
#include "Material.h"
#include "Lights.h"
#include "Sky.h"
#include "WICTextureLoader.h"

#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();

	// Custom entities
	std::vector<Mesh*> meshes;
	std::vector<Entity*> entities;
	Material* material_wood;
	Material* material_bronze;
	Material* material_cobblestone;
	Material* material_scratched;
	Material* material_paint;
	Material* material_floor;
	
	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Shaders and shader-related constructs
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShaderNormalMap;
	SimpleVertexShader* vertexShaderNormalMap;


	Camera* mainCamera;

	DirectionalLight directionalLight1;
	DirectionalLight directionalLight2;
	DirectionalLight directionalLight3;

	PointLight pointLight1;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> woodAlbedoSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> woodMetalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> woodNormalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> woodRoughnessSRV;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bronzeAlbedoSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bronzeMetalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bronzeNormalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bronzeRoughnessSRV;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobblestoneAlbedoSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobblestoneMetalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobblestoneNormalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobblestoneRoughnessSRV;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> scratchedAlbedoSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> scratchedMetalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> scratchedNormalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> scratchedRoughnessSRV;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> paintAlbedoSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> paintMetalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> paintNormalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> paintRoughnessSRV;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> floorAlbedoSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> floorMetalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> floorNormalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> floorRoughnessSRV;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	Sky* skybox;
};