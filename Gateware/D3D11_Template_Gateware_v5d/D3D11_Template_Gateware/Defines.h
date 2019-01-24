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
	XMFLOAT4 cLightDir;
	XMFLOAT4 cLightColor;
	XMFLOAT4 cOutputColor;
};

//Vectors for Objects
vector<Vertex> Earth;
vector<unsigned  int> earthIndicies;

vector<Vertex> Sun;
vector<unsigned  int> sunIndicies;

XMMATRIX worldMatrix;
XMMATRIX viewMatrix;
XMMATRIX projectionMatrix;
XMMATRIX ReturnViewMatrix;
XMFLOAT4 LightDir[2];
XMFLOAT4 LightColor[2];
XMFLOAT4 OutputColor;

float ratio;
float oldRatio;



float timeSpent = 0, curDeg = 0;