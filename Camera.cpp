#include "Camera.h"



Camera::Camera(
	float x,
	float y,
	float z,
	float pitch,
	float yaw,
	float roll,
	float fovAngleInit,
	float movementSpeedInit,
	float mouseLookSpeedInit,
	float aspectRatioInit,
	float nearClipPlaneInit,
	float farClipPlaneInit
)
{
	transform = Transform();
	transform.SetPosition(x, y, z);
	transform.SetRotation(pitch, yaw, roll);
	fieldOfViewAngle = fovAngleInit;
	movementSpeed = movementSpeedInit;
	mouseLookSpeed = mouseLookSpeedInit;
	aspectRatio = aspectRatioInit;
	nearClipPlaneDistance = nearClipPlaneInit;
	farClipPlaneDistance = farClipPlaneInit;
	UpdateViewMatrix();
	UpdateProjectionMatrix(aspectRatio);
}

void Camera::UpdateProjectionMatrix(float newAspectRatio)
{
	DirectX::XMStoreFloat4x4(&projMatrix, DirectX::XMMatrixPerspectiveFovLH(fieldOfViewAngle, newAspectRatio, nearClipPlaneDistance, farClipPlaneDistance));
}

void Camera::UpdateViewMatrix()
{
	DirectX::XMStoreFloat4x4(&viewMatrix, 
		DirectX::XMMatrixLookToLH(
			DirectX::XMLoadFloat3(&transform.GetPosition()),
			DirectX::XMLoadFloat3(&transform.GetLocalForward()),
			DirectX::XMVectorSet(0, 1, 0, 0)
		)
	);
}

void Camera::Update(float dt, HWND windowHandle)
{
	// handle user input (using unreal controls)
	if (GetAsyncKeyState('W') & 0x8000)
	{
		transform.MoveRelative(0, 0, movementSpeed * dt);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		transform.MoveRelative(0, 0, movementSpeed * dt * -1.0f);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		transform.MoveRelative(movementSpeed * dt * 1.0f, 0, 0);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		transform.MoveRelative(movementSpeed * dt * -1.0f, 0, 0);
	}
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		transform.MoveAbsolute(0, movementSpeed * dt, 0);
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		transform.MoveAbsolute(0, movementSpeed * dt * -1.0f, 0);
	}

	//mouse
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(windowHandle, &mousePos);

	// view only pans when right click is held
	if (GetAsyncKeyState(VK_RBUTTON) && 0x8000)
	{
		float offset_X = (float) prevMousePos.x - mousePos.x;
		float offset_Y = (float) prevMousePos.y - mousePos.y;
		offset_X *= mouseLookSpeed * dt;
		offset_Y *= mouseLookSpeed * dt;
		transform.Rotate(offset_Y, offset_X, 0);
	}

	prevMousePos = mousePos;

	//update stuff
	UpdateViewMatrix();
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix() const
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
	return projMatrix;
}

Transform* Camera::GetTransform() 
{
	return &transform;
}