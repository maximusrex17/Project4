#pragma once
#include "Defines.h"

XMFLOAT4X4 MoveCamera(XMFLOAT4X4 view4x4) {
	//TODO: Add Camera Controls 'W' 'A' 'S' 'D' 'Up Arrow' 'Down Arrow' 'Left Arrow' 'Right Arrow'

	GetCursorPos(p);
	XMMATRIX view = XMLoadFloat4x4(&view4x4);
	XMFLOAT4X4 newView;

	////W Key: Forward
	//if (GetAsyncKeyState(0x57)) {
	//	view *= XMMatrixTranslation(0.0f, 0.0f, -0.01f);
	//	XMStoreFloat4x4(&newView, view);
	//}
	//
	////A Key: Left
	//if (GetAsyncKeyState(0x41)) {
	//	view *= XMMatrixTranslation(0.01f, 0.0f, 0.0f);
	//	XMStoreFloat4x4(&newView, view);
	//}
	//
	////S Key: Backward
	//if (GetAsyncKeyState(0x53)) {
	//	view *= XMMatrixTranslation(0.0f, 0.0f, 0.01f);
	//	XMStoreFloat4x4(&newView, view);
	//}
	//
	////D Key: Right
	//if (GetAsyncKeyState(0x44)) {
	//	view *= XMMatrixTranslation(-0.01f, 0.0f, 0.0f);
	//	XMStoreFloat4x4(&newView, view);
	//}

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

	////Q Key: Rise
	//if (GetAsyncKeyState('Q')) {
	//	view *= XMMatrixTranslation(0.0f, -0.01f, 0.0f);
	//	XMStoreFloat4x4(&newView, view);
	//}
	//
	////E Key: Fall
	//if (GetAsyncKeyState('E')) {
	//	view *= XMMatrixTranslation(0.0f, 0.01f, 0.0f);
	//	XMStoreFloat4x4(&newView, view);
	//}

	XMStoreFloat4x4(&newView, view);
	return newView;
}

XMFLOAT4X4 MoveSky(XMFLOAT4X4 sky4x4) {
	//TODO: Add Camera Controls 'W' 'A' 'S' 'D' 'Up Arrow' 'Down Arrow' 'Left Arrow' 'Right Arrow'

	GetCursorPos(p);
	XMMATRIX sky = XMLoadFloat4x4(&sky4x4);
	XMFLOAT4X4 newSky;

	//W Key: Forward
	if (GetAsyncKeyState(0x57)) {
		sky *= XMMatrixTranslation(0.0f, 0.0f, 0.01f);
		XMStoreFloat4x4(&newSky, sky);
	}

	//A Key: Left
	if (GetAsyncKeyState(0x41)) {
		sky *= XMMatrixTranslation(-0.01f, 0.0f, 0.0f);
		XMStoreFloat4x4(&newSky, sky);
	}

	//S Key: Backward
	if (GetAsyncKeyState(0x53)) {
		sky *= XMMatrixTranslation(0.0f, 0.0f, -0.01f);
		XMStoreFloat4x4(&newSky, sky);
	}

	//D Key: Right
	if (GetAsyncKeyState(0x44)) {
		sky *= XMMatrixTranslation(0.01f, 0.0f, 0.0f);
		XMStoreFloat4x4(&newSky, sky);
	}

	if (GetAsyncKeyState(VK_SHIFT)) {
		if (p->y < posY) {
			posY = p->y;
			sky *= XMMatrixRotationX(0.005);
			XMStoreFloat4x4(&newSky, sky);
		}
	
		if (p->y > posY) {
			posY = p->y;
			sky *= XMMatrixRotationX(-0.005f);
			XMStoreFloat4x4(&newSky, sky);
		}
	
		if (p->x < posX) {
			posX = p->x;
			sky *= XMMatrixRotationY(0.005);
			XMStoreFloat4x4(&newSky, sky);
		}
	
		if (p->x > posX) {
			posX = p->x;
			sky *= XMMatrixRotationY(-0.005f);
			XMStoreFloat4x4(&newSky, sky);
		}
	}

	//Q Key: Rise
	if (GetAsyncKeyState('Q')) {
		sky *= XMMatrixTranslation(0.0f, 0.01f, 0.0f);
		XMStoreFloat4x4(&newSky, sky);
	}

	//E Key: Fall
	if (GetAsyncKeyState('E')) {
		sky *= XMMatrixTranslation(0.0f, -0.01f, 0.0f);
		XMStoreFloat4x4(&newSky, sky);
	}

	XMStoreFloat4x4(&newSky, sky);;
	return newSky;
}
