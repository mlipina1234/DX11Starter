#include "Transform.h"


void Transform::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
	isDirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	scale = DirectX::XMFLOAT3(x, y, z);
	isDirty = true;
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	rotation = DirectX::XMFLOAT3(pitch, yaw, roll);
	isDirty = true;
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetRotation()
{
	return rotation;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	if (isDirty)
	{
		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
			* DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) 
			* DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		XMStoreFloat4x4(&worldMatrix, world);
		isDirty = false;
	}
	return worldMatrix;
}

void Transform::MoveAbsolute(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(position.x + x, position.y + y, position.z + z);
	isDirty = true;
}

void Transform::MoveRelative(float x, float y, float z)
{
	DirectX::XMVECTOR moveVector = DirectX::XMVectorSet(x, y, z, 0);
	DirectX::XMVECTOR rotationVector = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	moveVector = DirectX::XMVector3Rotate(moveVector, rotationVector);

	DirectX::XMVECTOR posVector = DirectX::XMLoadFloat3(&position);
	posVector = DirectX::XMVectorAdd(moveVector, posVector);
	DirectX::XMStoreFloat3(&position, posVector);
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	rotation = DirectX::XMFLOAT3(rotation.x + pitch, rotation.y + yaw, rotation.z + roll);
	isDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
	scale = DirectX::XMFLOAT3(scale.x, scale.y, scale.z);
	isDirty = true;
}

DirectX::XMFLOAT3 Transform::LocalToWorld(float x, float y, float z) const
{
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(x, y, z, 0);
	DirectX::XMVECTOR rotationVector = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	vec = DirectX::XMVector3Rotate(vec, rotationVector);
	DirectX::XMFLOAT3 returnVal;
	DirectX::XMStoreFloat3(&returnVal, vec);
	return returnVal;
}


DirectX::XMFLOAT3 Transform::GetLocalForward() const
{
	return LocalToWorld(0.f, 0.f, 1.f);
}

DirectX::XMFLOAT3 Transform::GetLocalRight() const
{
	return LocalToWorld(1.f, 0.f, 0.f);
}

DirectX::XMFLOAT3 Transform::GetLocalUp() const
{
	return LocalToWorld(0.f, 1.f, 0.f);
}