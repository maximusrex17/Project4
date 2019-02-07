#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>
#include"VertexShader_PPIV.csh"
#include"PixelShader_PPIV.csh"
#include"LightVertexShader_PPIV.csh"
#include"LightPixelShader_PPIV.csh"
#include "VertexShader.h"
#include <directxmath.h>
#include <vector>
// FBX includes
#include "DDSTextureLoader.h"
#include "fbxsdk.h"
#include <iostream>

using namespace std;
using namespace DirectX;

#define RAND_NORMAL XMFLOAT3(rand()/float(RAND_MAX),rand()/float(RAND_MAX),rand()/float(RAND_MAX))

struct Vertex {
	float pos[4];
	float UV[2];
	float norm[3];
	//float color[4];
};

struct ConstantBuffer
{
	XMMATRIX cWorld;
	XMMATRIX cView;
	XMMATRIX cProjection;
	XMMATRIX cRotateY;
	XMFLOAT4 cLightPos;
	XMFLOAT4 cLightPos1;
	XMFLOAT4 cLightDir;
	XMFLOAT4 cLightDir1;
	XMFLOAT4 cLightColor;
	XMFLOAT4 cLightColor1;
	XMFLOAT4 cOutputColor;
	float cFloatScale;
	float cRange;
};

//Vectors for Objects
vector<Vertex> Planet;
vector<unsigned  int> planetIndicies;

vector<Vertex> SkyBox;
vector<unsigned  int> skyBoxIndicies;

vector<Vertex> Ship;
vector<unsigned  int> shipIndicies;


XMMATRIX worldMatrix;
XMMATRIX viewMatrix;
XMMATRIX projectionMatrix;
XMMATRIX ReturnViewMatrix;
XMMATRIX shipMatrix;
XMMATRIX flipMatrix;
XMMATRIX earthMatrix;
XMMATRIX moonMatrix;
XMMATRIX marsMatrix;
XMMATRIX skyMatrix;
XMMATRIX sunMatrix;
XMFLOAT4 LightDir[2];
XMFLOAT4 LightColor[2];
XMFLOAT4 OutputColor;

XMMATRIX copyWorld;
XMMATRIX tempWorld;

XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -15.0f, 0.0f);
XMVECTOR Focus = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

float ratio;
float oldRatio;


POINT point = { 0,0 };
LPPOINT p = { &point }, q = { &point };



float timeSpent = 0, curDeg = 0, posX = 0, posY = 0;




//SkyBox Position
XMFLOAT4 SkyPos = { 0,0,0,1 };
XMFLOAT4 DynSkyPos = { 1,0,0,1 };