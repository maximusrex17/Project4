#pragma once
#include "Defines.h"

XMFLOAT4X4 MoveCamera(XMFLOAT4X4 view4x4) {
	//TODO: Add Camera Controls 'W' 'A' 'S' 'D' 'Up Arrow' 'Down Arrow' 'Left Arrow' 'Right Arrow'

	XMMATRIX view = XMLoadFloat4x4(&view4x4);
	XMFLOAT4X4 newView;

	//W Key: Forward
	if (GetAsyncKeyState(0x57)) {
		view *= XMMatrixTranslation(0.0f, 0.0f, -0.01f); 
		XMStoreFloat4x4(&newView, view);
		return newView;
	}

	//A Key: Left
	if (GetAsyncKeyState(0x41)) {
		view *= XMMatrixTranslation(0.01f, 0.0f, 0.0f);
		XMStoreFloat4x4(&newView, view);
		return newView;
	}

	//S Key: Backward
	if (GetAsyncKeyState(0x53)) {
		view *= XMMatrixTranslation(0.0f, 0.0f, 0.01f);
		XMStoreFloat4x4(&newView, view);
		return newView;
	}

	//D Key: Right
	if (GetAsyncKeyState(0x44)) {
		view *= XMMatrixTranslation(-0.01f, 0.0f, 0.0f);
		XMStoreFloat4x4(&newView, view);
		return newView;
	}

	if (GetAsyncKeyState(VK_CONTROL)) {
		if (GetAsyncKeyState(VK_UP)) {
			view *= XMMatrixRotationX(-0.001f);
			XMStoreFloat4x4(&newView, view);
			return newView;
		}

		else if (GetAsyncKeyState(VK_DOWN)) {
			view *= XMMatrixRotationX(0.001f);
			XMStoreFloat4x4(&newView, view);
			return newView;
		}
	}

	//Up Arrow Key: Rise
	if (GetAsyncKeyState(VK_UP)) {
		view *= XMMatrixTranslation(0.0f, -0.01f, 0.0f);
		XMStoreFloat4x4(&newView, view);
		return newView;
	}

	//Down Arrow Key: Fall
	if (GetAsyncKeyState(VK_DOWN)) {
		view *= XMMatrixTranslation(0.0f, 0.01f, 0.0f);
		XMStoreFloat4x4(&newView, view);
		return newView;
	}

	//Left Arrow Key: Rotate Left
	if (GetAsyncKeyState(VK_LEFT)) {
		view *= XMMatrixRotationY(0.001f);
		XMStoreFloat4x4(&newView, view);
		return newView;
	}

	//Right Arrow Key: Rotate Right
	if (GetAsyncKeyState(VK_RIGHT)) {
		view *= XMMatrixRotationY(-0.001f);
		XMStoreFloat4x4(&newView, view);
		return newView;
	}
	XMStoreFloat4x4(&newView, view);
	return newView;
}
