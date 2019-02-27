#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>
#include"VertexShader_PPIV.csh"
#include"PixelShader_PPIV.csh"
#include"LightVertexShader_PPIV.csh"
#include"WaterVertexShader_PPIV.csh"
#include"LightPixelShader_PPIV.csh"
#include"SpotLightPixelShader_PPIV.csh"
#include"MultiTexturePixelShader_PPIV.csh"
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
#define _DEGBUG
#define SAFE_RELEASE(ptr) {if(ptr) {ptr->Release(); ptr = nullptr;}}

struct Vertex {
	float pos[4];
	float UV[2];
	float norm[3];
	float tangent[4];
	float biTangent[4];
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
	float cFloatScale;
	float cRange;
	float cTime;
};

//Vectors for Objects
vector<Vertex> Planet;
vector<unsigned  int> planetIndicies;

vector<Vertex> SkyBox;
vector<unsigned  int> skyBoxIndicies;

vector<Vertex> Ship;
vector<unsigned  int> shipIndicies;

vector<Vertex> Temple;
vector<unsigned  int> templeIndicies;

vector<Vertex> Bamboo;
vector<unsigned  int> bambooIndicies;

vector<Vertex> Terrain;
vector<unsigned  int> terrainIndicies;

vector<Vertex> Water;
vector<unsigned  int> waterIndicies;


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
XMFLOAT4 BambooPos[10];

XMMATRIX skyScale = XMMatrixScaling(50.0f, 50.0f, 50.0f);
XMMATRIX tempWorld;

XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -15.0f, 0.0f);
XMVECTOR Focus = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR eyePrime = Eye;
XMVECTOR focusPrime = Focus;
XMVECTOR upPrime = Up;
XMVECTOR camDiffX;
XMFLOAT4 camAngleX;
XMVECTOR camDiffY;
XMFLOAT4 camAngleY;
XMFLOAT4 lightPos = { 0,10,-25,1 };

float timer = 0;

float scale = 1;

bool sceneToggle = true;

float ratio;
float splitRatio;
float oldRatio;


POINT point = { 0,0 };
LPPOINT p = { &point }, q = { &point };



float timeSpent = 0, curDeg = 0, posX = 0, posY = 0;




//SkyBox Position
XMFLOAT4 SkyPos = { 0,0,0,1 };
XMFLOAT4 DynSkyPos = { 1,0,0,1 };

ID3D11Debug* debugDevice;


