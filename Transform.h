#pragma once

#include <DirectXMath.h>

class Transform
{
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
	bool isDirty;
public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);
	void SetScale(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT4X4 GetWorldMatrix();

	void MoveAbsolute(float x, float y, float z);
	void MoveRelative(float x, float y, float z);
	void Rotate(float pitch, float yaw, float roll);
	void Scale(float x, float y, float z);

	DirectX::XMFLOAT3 LocalToWorld(float x, float y, float z) const;
	DirectX::XMFLOAT3 GetLocalForward() const;
	DirectX::XMFLOAT3 GetLocalRight() const;
	DirectX::XMFLOAT3 GetLocalUp() const;
};

