#include "BufferStructs.h"
#include "Game.h"
#include <cmath>

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object created in Game
	for (Mesh* m : meshes)
	{
		delete m;
	}

	for (Entity* e : entities)
	{
		delete e;
	}
	
	delete material_cobblestone;
	delete material_wood;
	delete material_bronze;
	delete material_scratched;
	delete material_paint;
	delete material_floor;

	delete mainCamera;

	delete pixelShader;
	delete vertexShader;
	delete pixelShaderNormalMap;
	delete vertexShaderNormalMap;
	delete skybox;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();

	D3D11_SAMPLER_DESC samplerDesc = D3D11_SAMPLER_DESC();
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto thing = device->CreateSamplerState(&samplerDesc, &samplerState);

#pragma region Textures

	// Wood

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/wood_albedo.png").c_str(),
		nullptr,
		&woodAlbedoSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/wood_normals.png").c_str(),
		nullptr,
		&woodNormalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/wood_metal.png").c_str(),
		nullptr,
		&woodMetalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/wood_roughness.png").c_str(),
		nullptr,
		&woodRoughnessSRV
	);

	// Bronze

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/bronze_albedo.png").c_str(),
		nullptr,
		&bronzeAlbedoSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/bronze_normals.png").c_str(),
		nullptr,
		&bronzeNormalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/bronze_metal.png").c_str(),
		nullptr,
		&bronzeMetalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/bronze_roughness.png").c_str(),
		nullptr,
		&bronzeRoughnessSRV
	);

	// Cobblestone

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/cobblestone_albedo.png").c_str(),
		nullptr,
		&cobblestoneAlbedoSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/cobblestone_normals.png").c_str(),
		nullptr,
		&cobblestoneNormalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/cobblestone_metal.png").c_str(),
		nullptr,
		&cobblestoneMetalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/cobblestone_roughness.png").c_str(),
		nullptr,
		&cobblestoneRoughnessSRV
	);

	// Scratched

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/scratched_albedo.png").c_str(),
		nullptr,
		&scratchedAlbedoSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/scratched_normals.png").c_str(),
		nullptr,
		&scratchedNormalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/scratched_metal.png").c_str(),
		nullptr,
		&scratchedMetalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/scratched_roughness.png").c_str(),
		nullptr,
		&scratchedRoughnessSRV
	);

	// Paint

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/paint_albedo.png").c_str(),
		nullptr,
		&paintAlbedoSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/paint_normals.png").c_str(),
		nullptr,
		&paintNormalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/paint_metal.png").c_str(),
		nullptr,
		&paintMetalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/paint_roughness.png").c_str(),
		nullptr,
		&paintRoughnessSRV
	);

	// Floor

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/floor_albedo.png").c_str(),
		nullptr,
		&floorAlbedoSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/floor_normals.png").c_str(),
		nullptr,
		&floorNormalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/floor_metal.png").c_str(),
		nullptr,
		&floorMetalSRV
	);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/floor_roughness.png").c_str(),
		nullptr,
		&floorRoughnessSRV
	);

#pragma endregion


	material_wood = new Material(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.5f, pixelShaderNormalMap, vertexShaderNormalMap, woodAlbedoSRV.Get(), samplerState.Get(), woodNormalSRV.Get(), woodRoughnessSRV.Get(), woodMetalSRV.Get());
	material_bronze = new Material(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, pixelShaderNormalMap, vertexShaderNormalMap, bronzeAlbedoSRV.Get(), samplerState.Get(), bronzeNormalSRV.Get(), bronzeRoughnessSRV.Get(), bronzeMetalSRV.Get());
	material_cobblestone = new Material(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, pixelShaderNormalMap, vertexShaderNormalMap, cobblestoneAlbedoSRV.Get(), samplerState.Get(), cobblestoneNormalSRV.Get(), cobblestoneRoughnessSRV.Get(), cobblestoneMetalSRV.Get());
	material_scratched = new Material(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.5f, pixelShaderNormalMap, vertexShaderNormalMap, scratchedAlbedoSRV.Get(), samplerState.Get(), scratchedNormalSRV.Get(), scratchedRoughnessSRV.Get(), scratchedMetalSRV.Get());
	material_paint = new Material(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.5f, pixelShaderNormalMap, vertexShaderNormalMap, paintAlbedoSRV.Get(), samplerState.Get(), paintNormalSRV.Get(), paintRoughnessSRV.Get(), paintMetalSRV.Get());
	material_floor = new Material(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.5f, pixelShaderNormalMap, vertexShaderNormalMap, floorAlbedoSRV.Get(), samplerState.Get(), floorNormalSRV.Get(), floorRoughnessSRV.Get(), floorMetalSRV.Get());

	directionalLight1 = DirectionalLight();
	directionalLight1.ambientColor = XMFLOAT3(0.01f, 0.01f, 0.02f);
	directionalLight1.diffuseColor = XMFLOAT3(0.3f, 0.3f, 0.4f);
	//directionalLight1.diffuseColor = XMFLOAT3(0.0f, 0.0f, 0.0f);
	directionalLight1.direction = XMFLOAT3(1.0f, 1.0f, -1.0f);

	directionalLight2 = DirectionalLight();
	directionalLight2.ambientColor =	XMFLOAT3(0.01f, 0.01f, 0.01f);
	directionalLight2.diffuseColor =	XMFLOAT3(0.01f, 0.01f, 0.01f);
	directionalLight2.direction =		XMFLOAT3(0.0f, 1.0f, 0.0f);

	directionalLight3 = DirectionalLight();
	directionalLight3.ambientColor =	XMFLOAT3(0.01f, 0.01f, 0.01f);
	directionalLight3.diffuseColor =	XMFLOAT3(0.01f, 0.02f, 0.01f);
	directionalLight3.direction =		XMFLOAT3(0.0f, 1.0f, -1.0f);

	pointLight1 = PointLight();
	pointLight1.color = XMFLOAT3(1, 1, 1);
	pointLight1.position = XMFLOAT3(-5, 100, 0);

	CreateBasicGeometry();
	mainCamera = new Camera(
		0,
		0,
		-2.0f,
		0,
		0,
		0,
		80.0f,
		1.0f,
		2.0f,
		(float)this->width / this->height,
		0.1f,
		50.0f
	);
	
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create constant buffer for VertexShaderExternalData
	unsigned int size = sizeof(VertexShaderExternalData);
	size = (size + 15) / 16 * 16;
	D3D11_BUFFER_DESC cbDesc	= {}; //sets to all zeros
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth			= size;
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;


	//Setup skybox
	
	//meshes.push_back(new Mesh(GetFullPathTo("../../assets/meshes/cube.obj").c_str(), device));
	skybox = new Sky(
		new Mesh(GetFullPathTo("../../assets/meshes/cube.obj").c_str(), device),
		samplerState.Get(),
		GetFullPathTo_Wide(L"../../assets/textures/SpaceCubeMap.dds").c_str(),
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"Sky_VS.cso").c_str(),
		GetFullPathTo_Wide(L"Sky_PS.cso").c_str()
	);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(
		device.Get(), 
		context.Get(), 
		GetFullPathTo_Wide(L"VertexShader.cso").c_str()
	); 
	pixelShader = new SimplePixelShader(
		device.Get(), 
		context.Get(), 
		GetFullPathTo_Wide(L"PixelShader.cso").c_str()
	);
	vertexShaderNormalMap = new SimpleVertexShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"NormalMap_VS.cso").c_str()
	);
	pixelShaderNormalMap = new SimplePixelShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"NormalMap_PS.cso").c_str()
	);
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT3 Normal = { 0, 0, -1 };
	XMFLOAT2 UV = { 0, 0 };
	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), Normal, UV },
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f), Normal, UV },
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), Normal, UV },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2 };

	//meshes.push_back( new Mesh(vertices, 3, indices, 3, device) );
	//entities.push_back(new Entity(meshes.back()));

	// square mesh
	Vertex squareVertices[] =
	{
		{ XMFLOAT3(+0.7f, +0.7f, +0.0f), Normal, UV },
		{ XMFLOAT3(+0.7f, +0.4f, +0.0f), Normal, UV },
		{ XMFLOAT3(+0.4f, +0.4f, +0.0f), Normal, UV },
		{ XMFLOAT3(+0.4f, +0.7f, +0.0f), Normal, UV },
	};
	unsigned int squareIndices[] = { 0, 1, 2, 0, 2, 3 };

	meshes.push_back(new Mesh(GetFullPathTo("../../assets/meshes/helix.obj").c_str(), device));
	entities.push_back(new Entity(meshes.back(), material_cobblestone));
	entities.push_back(new Entity(meshes.back(), material_wood));
	entities.push_back(new Entity(meshes.back(), material_bronze));
	entities.push_back(new Entity(meshes.back(), material_scratched));
	entities.push_back(new Entity(meshes.back(), material_paint));
	entities.push_back(new Entity(meshes.back(), material_floor));

	for (int i = 0; i < entities.size(); i++)
	{
		Transform* t = entities[i]->GetTransform();
		t->SetPosition(i * 2.0f + -1.0f, -1.0f, 1.0f);
		t->SetRotation(0.0f, 0.0f, 0.0f);
		t->SetScale(0.7f, 0.7f, 0.7f);
	}

	meshes.push_back(new Mesh(GetFullPathTo("../../assets/meshes/sphere.obj").c_str(), device));
	entities.push_back(new Entity(meshes.back(), material_cobblestone));
	entities.push_back(new Entity(meshes.back(), material_wood));
	entities.push_back(new Entity(meshes.back(), material_bronze));
	entities.push_back(new Entity(meshes.back(), material_scratched));
	entities.push_back(new Entity(meshes.back(), material_paint));
	entities.push_back(new Entity(meshes.back(), material_floor));

	for (int i = (int)entities.size() / 2; i < entities.size(); i++)
	{
		Transform* t = entities[i]->GetTransform();
		t->SetPosition(i * 2.0f + -14, 1.0f, 1.0f);
		t->SetRotation(0.0f, 0.0f, 0.0f);
		t->SetScale(1, 1, 1);
	}

	meshes.push_back(new Mesh(GetFullPathTo("../../assets/meshes/cylinder.obj").c_str(), device));
	entities.push_back(new Entity(meshes.back(), material_cobblestone));
	entities.push_back(new Entity(meshes.back(), material_wood));
	entities.push_back(new Entity(meshes.back(), material_bronze));
	entities.push_back(new Entity(meshes.back(), material_scratched));
	entities.push_back(new Entity(meshes.back(), material_paint));
	entities.push_back(new Entity(meshes.back(), material_floor));

	for (int i = (int)entities.size() * 2 / 3; i < entities.size(); i++)
	{
		Transform* t = entities[i]->GetTransform();
		t->SetPosition(i * 2.0f + -28, 3.0f, 1.0f);
		t->SetRotation(0.0f, 0.0f, 0.0f);
		t->SetScale(1, 1, 1);
	}
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	if (mainCamera != nullptr)
	{
		mainCamera->UpdateProjectionMatrix((float)this->width / this->height);
	}
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	mainCamera->Update(deltaTime, this->hWnd);
	for (Entity* e : entities)
	{
		e->GetTransform()->MoveAbsolute(sin(totalTime ) * 0.00001f, cos(totalTime) * 0.00001f, 0);
		e->GetTransform()->Rotate(0.0f, 0.0f, deltaTime);
	}
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();


}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	pixelShader->SetData(
		"directionalLight1",
		&directionalLight1,
		sizeof(DirectionalLight)
	);
	pixelShader->SetData(
		"directionalLight2",
		&directionalLight2,
		sizeof(DirectionalLight)
	);
	pixelShader->SetData(
		"directionalLight3",
		&directionalLight3,
		sizeof(DirectionalLight)
	);
	pixelShader->SetData(
		"pointLight1",
		&pointLight1,
		sizeof(PointLight)
	);
	pixelShaderNormalMap->SetData(
		"directionalLight1",
		&directionalLight1,
		sizeof(DirectionalLight)
	);
	pixelShaderNormalMap->SetData(
		"directionalLight2",
		&directionalLight2,
		sizeof(DirectionalLight)
	);
	pixelShaderNormalMap->SetData(
		"directionalLight3",
		&directionalLight3,
		sizeof(DirectionalLight)
	);
	pixelShaderNormalMap->SetData(
		"pointLight1",
		&pointLight1,
		sizeof(PointLight)
	);
	pixelShader->CopyAllBufferData();

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	//=====================================| DRAW MESHES

	for (Entity* entity : entities)
	{
		entity->Draw(context, mainCamera);
	}

	// Draw the skybox last
	skybox->Draw(context, mainCamera);
	
	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}