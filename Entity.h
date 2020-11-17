#pragma once
#include "DXCore.h"

#include "Material.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"

class Entity
{
	Transform transform;
	Mesh* mesh;
	Material* material;

public:
	Entity(Mesh* mesh, Material* material);

	Mesh* GetMesh();
	Transform* GetTransform();
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* cam);
};

