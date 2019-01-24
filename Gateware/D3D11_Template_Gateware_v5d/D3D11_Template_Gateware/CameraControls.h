#pragma once
#include "Defines.h"

XMFLOAT4X4 MoveCamera(XMFLOAT4X4 view4x4, float posX, float posY) {
	//TODO: Add Camera Controls 'W' 'A' 'S' 'D' 'Up Arrow' 'Down Arrow' 'Left Arrow' 'Right Arrow'

	GetCursorPos(p);
	XMMATRIX view = XMLoadFloat4x4(&view4x4);
	XMFLOAT4X4 newView;

	//W Key: Forward
	if (GetAsyncKeyState(0x57)) {
		view *= XMMatrixTranslation(0.0f, 0.0f, -0.01f); 
		XMStoreFloat4x4(&newView, view);
	}

	//A Key: Left
	if (GetAsyncKeyState(0x41)) {
		view *= XMMatrixTranslation(0.01f, 0.0f, 0.0f);
		XMStoreFloat4x4(&newView, view);
	}
	
	//S Key: Backward
	if (GetAsyncKeyState(0x53)) {
		view *= XMMatrixTranslation(0.0f, 0.0f, 0.01f);
		XMStoreFloat4x4(&newView, view);
	}

	//D Key: Right
	if (GetAsyncKeyState(0x44)) {
		view *= XMMatrixTranslation(-0.01f, 0.0f, 0.0f);
		XMStoreFloat4x4(&newView, view);
	}

	if (p->y < 0) {
		view *= XMMatrixRotationX((0 - p->y)-0);
		XMStoreFloat4x4(&newView, view);
	}

	if (p->y > 0) {
		view *= XMMatrixRotationX((0 + p->y)/100);
		XMStoreFloat4x4(&newView, view);
	}


	//Q Key: Rise
	if (GetAsyncKeyState('Q')) {
		view *= XMMatrixTranslation(0.0f, -0.01f, 0.0f);
		XMStoreFloat4x4(&newView, view);
	}

	//E Key: Fall
	if (GetAsyncKeyState('E')) {
		view *= XMMatrixTranslation(0.0f, 0.01f, 0.0f);
		XMStoreFloat4x4(&newView, view);
	}

	//Left Arrow Key: Rotate Left
	if (GetAsyncKeyState(VK_LEFT)) {
		view *= XMMatrixRotationY(moveDeg);
		XMStoreFloat4x4(&newView, view);
	}

	//Right Arrow Key: Rotate Right
	if (GetAsyncKeyState(VK_RIGHT)) {
		view *= XMMatrixRotationY(-0.001f);
		XMStoreFloat4x4(&newView, view);
	}
	XMStoreFloat4x4(&newView, view);
	return newView;
}
