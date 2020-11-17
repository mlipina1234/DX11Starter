#pragma once
#include <Windows.h>
#include <DirectXMath.h>

#include "Transform.h"

class Camera
{
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projMatrix;
	Transform transform;

	POINT prevMousePos;
	float nearClipPlaneDistance;
	float farClipPlaneDistance;
	float movementSpeed;
	float mouseLookSpeed;
	float fieldOfViewAngle;
	float aspectRatio;

public:
	Camera(
		float x = 0.0f,
		float y = 0.0f,
		float z = 0.0f,
		float roll = 0.0f,
		float pitch = 0.0f,
		float yaw = 0.0f,
		float fovAngle = 90.0f,
		float movementSpeedInit = 1.0f,
		float mouseLookSpeedInit = 1.0f,
		float aspectRatioInit = 1.0f,
		float nearClipPlaneInit = 1.0f,
		float farClipPlaneInit = 10.0f
	);

	DirectX::XMFLOAT4X4 GetViewMatrix() const;
	DirectX::XMFLOAT4X4 GetProjectionMatrix() const;
	Transform* GetTransform();

	void UpdateProjectionMatrix(float newAspectRatio);
	void UpdateViewMatrix();
	void Update(float dt, HWND windowHandle);
};

