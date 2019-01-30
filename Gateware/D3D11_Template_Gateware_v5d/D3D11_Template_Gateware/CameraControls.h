#pragma once
#include "Defines.h"

XMFLOAT4X4 MoveCamera(XMFLOAT4X4 view4x4) {
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

	if (GetAsyncKeyState(VK_SHIFT)) {
		if (p->y < posY) {
			posY = p->y;
			view *= XMMatrixRotationX(0.005);
			XMStoreFloat4x4(&newView, view);
		}

		if (p->y > posY) {
			posY = p->y;
			view *= XMMatrixRotationX(-0.005f);
			XMStoreFloat4x4(&newView, view);
		}

		if (p->x < posX) {
			posX = p->x;
			view *= XMMatrixRotationY(0.005);
			XMStoreFloat4x4(&newView, view);
		}

		if (p->x > posX) {
			posX = p->x;
			view *= XMMatrixRotationY(-0.005f);
			XMStoreFloat4x4(&newView, view);
		}
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

	XMStoreFloat4x4(&newView, view);
	return newView;
}

XMFLOAT4 MoveSky(XMFLOAT4 sky) {
	//TODO: Add Camera Controls 'W' 'A' 'S' 'D' 'Up Arrow' 'Down Arrow' 'Left Arrow' 'Right Arrow'
	XMFLOAT4 newSky;
	//W Key: Forward
	if (GetAsyncKeyState(0x57)) {
		sky.z -= 0.01f;
	}

	//A Key: Left
	if (GetAsyncKeyState(0x41)) {
		sky.x += 0.01f;
	}

	//S Key: Backward
	if (GetAsyncKeyState(0x53)) {
		sky.z += 0.01f;
	}

	//D Key: Right
	if (GetAsyncKeyState(0x44)) {
		sky.x -= 0.01f;
	}

	//Q Key: Rise
	if (GetAsyncKeyState('Q')) {
		sky.y -= 0.01f;
	}

	//E Key: Fall
	if (GetAsyncKeyState('E')) {
		sky.y += 0.01f;
	}
	newSky = { sky.x, sky.y, sky.z, sky.w };

	return newSky;
}
