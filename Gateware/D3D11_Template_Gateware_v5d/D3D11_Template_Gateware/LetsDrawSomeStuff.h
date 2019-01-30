// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
#include "CameraControls.h"

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	ID3D11InputLayout *myInputLayout = nullptr;
	ID3D11VertexShader *myVertexShader = nullptr;
	ID3D11VertexShader *myVertexLightShader = nullptr;
	ID3D11PixelShader *myPixelShader = nullptr;
	ID3D11PixelShader *myLightPixelShader = nullptr;
	ID3D11Buffer *myVertexBuffer = nullptr;
	ID3D11Buffer *myShipVertexBuffer = nullptr;
	ID3D11Buffer *myIndexBuffer = nullptr;
	ID3D11Buffer *myShipIndexBuffer = nullptr;
	ID3D11Buffer *myConstantBuffer = nullptr;
	D3D11_VIEWPORT myPort;
	D3D_DRIVER_TYPE myDriverType = D3D_DRIVER_TYPE_NULL;
	
	//Matricies

	ID3D11RasterizerState* mySkyboxRasterizerState = nullptr;
	ID3D11RasterizerState* mySunRasterizerState = nullptr;
	ID3D11RasterizerState* myEarthRasterizerState = nullptr;
	ID3D11RasterizerState* myMoonRasterizerState = nullptr;
	ID3D11RasterizerState* myMarsRasterizerState = nullptr;
	ID3D11RasterizerState* myShipRasterizerState = nullptr;

	ID3D11ShaderResourceView* skyboxShaderResource = nullptr;
	ID3D11ShaderResourceView* earthbumpShaderResource = nullptr;
	ID3D11ShaderResourceView* earthShaderResource = nullptr;
	ID3D11ShaderResourceView* sunShaderResource = nullptr;
	ID3D11ShaderResourceView* moonShaderResource = nullptr;
	ID3D11ShaderResourceView* marsShaderResource = nullptr;
	ID3D11ShaderResourceView* shipShaderResource = nullptr;
	ID3D11SamplerState* mySampler = nullptr;
	
	

public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
};

FbxVector2 GetVertexUV(FbxGeometryElementUV* uvPtr, unsigned int VertexID, unsigned int controlPointIndex)
{
	FbxLayerElement::EMappingMode mapMode = uvPtr->GetMappingMode();
	switch (mapMode)
	{
	case FbxGeometryElement::EMappingMode::eByControlPoint:
	{
		FbxLayerElement::EReferenceMode refMode = uvPtr->GetReferenceMode();
		switch (refMode)
		{
		case FbxLayerElement::EReferenceMode::eDirect:
		{
			FbxVector2 uv = uvPtr->GetDirectArray().GetAt(controlPointIndex);
			return uv;
		}
		break;

		case FbxLayerElement::EReferenceMode::eIndexToDirect:
		{
			int id = uvPtr->GetIndexArray().GetAt(controlPointIndex);
			FbxVector2 uv = uvPtr->GetDirectArray().GetAt(id);
			return uv;
		}
		break;

		default:
			break;
		}
	}
	break;
	case FbxGeometryElement::EMappingMode::eByPolygonVertex:
	{
		FbxLayerElement::EReferenceMode refMode = uvPtr->GetReferenceMode();
		switch (refMode)
		{
		case FbxLayerElement::EReferenceMode::eDirect:
		{
			FbxVector2 uv = uvPtr->GetDirectArray().GetAt(VertexID);
			return uv;
		}
		break;

		case FbxLayerElement::EReferenceMode::eIndexToDirect:
		{
			int id = uvPtr->GetIndexArray().GetAt(VertexID);
			FbxVector2 uv = uvPtr->GetDirectArray().GetAt(id);
			return uv;
		}
		break;

		default:
			break;
		}
	}
	break;
	default:
		break;
	}

	return FbxVector2(0.0f, 0.0f);
}

void ProcessFbxMesh(FbxNode* Node, vector<Vertex>& verts, vector<unsigned int>& indicies)
{
	float scale = 1;
	//FBX Mesh stuff
	char header[100];
	int childrenCount = Node->GetChildCount();

	for (int a = 0; a < childrenCount; a++)
	{
		FbxNode *childNode = Node->GetChild(a);
		FbxMesh *mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			const char* name = mesh->GetName();

			FbxVector4* lControlPoints = mesh->GetControlPoints();
			const int polyCount = mesh->GetPolygonCount();

			int VertexId = 0;
			for (int i = 0; i < polyCount; i++)
			{
				int polygonSize = mesh->GetPolygonSize(i);
				for (int j = 0; j < polygonSize; j++)
				{
					int lControlPointIndex = mesh->GetPolygonVertex(i, j);
					FbxVector4 pos = lControlPoints[lControlPointIndex];
					
					Vertex temp;

					temp.pos[0] = (float)pos.mData[0] / scale;
					temp.pos[1] = (float)pos.mData[1] / scale;
					temp.pos[2] = (float)pos.mData[2] / scale;
					temp.pos[3] = 1.0f;

					//UV Element
					for (int l = 0; l < mesh->GetElementUVCount(); ++l)
					{
						FbxGeometryElementUV* leUV = mesh->GetElementUV(l);
						FBXSDK_sprintf(header, 100, "            Texture UV: ");

						switch (leUV->GetMappingMode())
						{
						default:
							break;
						case FbxGeometryElement::eByControlPoint:
							switch (leUV->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
								//Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
								break;
							case FbxGeometryElement::eIndexToDirect:
							{
								int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
								//Display2DVector(header, leUV->GetDirectArray().GetAt(id));
							}
							break;
							default:
								break; // other reference modes not shown here!
							}
							break;

						case FbxGeometryElement::eByPolygonVertex:
						{
							int lTextureUVIndex = mesh->GetTextureUVIndex(i, j);
							FbxVector2 uv = GetVertexUV(leUV, VertexId, lControlPointIndex);
							temp.UV[0] = (float)uv.mData[0];
							temp.UV[1] = 1.0f - (float)uv.mData[1];

							switch (leUV->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
							case FbxGeometryElement::eIndexToDirect:
							{
								//Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
							}
							break;
							default:
								break; // other reference modes not shown here!
							}
						}
						break;

						case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
						case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
						case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
							break;
						}
					}

					//Normal Element
					for (int l = 0; l < mesh->GetElementNormalCount(); ++l)
					{
						FbxGeometryElementNormal* leNormal = mesh->GetElementNormal(l);
						FBXSDK_sprintf(header, 100, "            Normal: ");
						

						switch (leNormal->GetMappingMode())
						{
						case FbxGeometryElement::eByControlPoint:
							switch (leNormal->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
								//Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
								temp.norm[0] = leNormal->GetDirectArray().GetAt(lControlPointIndex).mData[0];
								temp.norm[1] = leNormal->GetDirectArray().GetAt(lControlPointIndex).mData[1];
								temp.norm[2] = leNormal->GetDirectArray().GetAt(lControlPointIndex).mData[2];
								break;
							case FbxGeometryElement::eIndexToDirect:
							{
								int id = leNormal->GetIndexArray().GetAt(lControlPointIndex);

								temp.norm[0] = leNormal->GetDirectArray().GetAt(id).mData[0];
								temp.norm[1] = leNormal->GetDirectArray().GetAt(id).mData[1];
								temp.norm[2] = leNormal->GetDirectArray().GetAt(id).mData[2];
								//Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
							}
							break;
							default:
								break; // other reference modes not shown here!
							}
						case FbxGeometryElement::eByPolygonVertex:
							switch (leNormal->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
							{
								temp.norm[0] = leNormal->GetDirectArray().GetAt(VertexId).mData[0];
								temp.norm[1] = leNormal->GetDirectArray().GetAt(VertexId).mData[1];
								temp.norm[2] = leNormal->GetDirectArray().GetAt(VertexId).mData[2];
							}
							break;
							case FbxGeometryElement::eIndexToDirect:
							{
								int id = leNormal->GetIndexArray().GetAt(VertexId);

								temp.norm[0] = leNormal->GetDirectArray().GetAt(id).mData[0];
								temp.norm[1] = leNormal->GetDirectArray().GetAt(id).mData[1];
								temp.norm[2] = leNormal->GetDirectArray().GetAt(id).mData[2];
								//Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
							}
							break;
							default:
								break; // other reference modes not shown here!
							}
						};
					}

					//Tangent Element
					for (int l = 0; l < mesh->GetElementTangentCount(); ++l)
					{
						FbxGeometryElementTangent* leTangent = mesh->GetElementTangent(l);
						FBXSDK_sprintf(header, 100, "            Tangent: ");

						if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
						{
							switch (leTangent->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
								//Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
								break;
							case FbxGeometryElement::eIndexToDirect:
							{
								//int id = leTangent->GetIndexArray().GetAt(vertexId);
								//Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
							}
							break;
							default:
								break; // other reference modes not shown here!
							}
						}

					}

					//Binormal Element
					for (int l = 0; l < mesh->GetElementBinormalCount(); ++l)
					{

						FbxGeometryElementBinormal* leBinormal = mesh->GetElementBinormal(l);

						FBXSDK_sprintf(header, 100, "            Binormal: ");
						if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
						{
							switch (leBinormal->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
								//Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
								break;
							case FbxGeometryElement::eIndexToDirect:
							{
								//int id = leBinormal->GetIndexArray().GetAt(vertexId);
								//Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
							}
							break;
							default:
								break; // other reference modes not shown here!
							}
						}
					}

					verts.push_back(temp);
					indicies.push_back(VertexId);

					VertexId++;

					///////////////////////////////////////////
					//Old Method
					//////////////////////////////////////////
					// create a temporary Vertex
					//Vertex temp;


					//
					//// Get the Normals array from the mesh
					//FbxArray<FbxVector4> normalsVec;
					//mesh->GetPolygonVertexNormals(normalsVec);
					//cout << "\nNormalVec Count:" << normalsVec.Size();

					//temp.norm[0] = normalsVec.GetAt(j).mData[0];
					//temp.norm[1] = normalsVec.GetAt(j).mData[1];
					//temp.norm[2] = normalsVec.GetAt(j).mData[2];
					//
					//for (int k = 0; k < mesh->GetElementUVCount(); k++)
					//{
					//	FbxGeometryElementUV *fbxUV = mesh->GetElementUV(k);
					//	int textIndex = mesh->GetTextureUVIndex(i, j);

					//	FbxVector2 uv = GetVertexUV(fbxUV, VertexId, controlPointIndex);
					//	temp.UV[0] = (float)uv.mData[0];
					//	temp.UV[1] = 1.0f - (float)uv.mData[1];						
					//}

					//verts.push_back(temp);
					//indicies.push_back(VertexId);

					//VertexId++;
				}
			}

			//=======================Texture===============================================
			int materialCount = childNode->GetSrcObjectCount<FbxSurfaceMaterial>();

			for (int index = 0; index < materialCount; index++)
			{
				FbxSurfaceMaterial* material = (FbxSurfaceMaterial*)childNode->GetSrcObject<FbxSurfaceMaterial>(index);

				if (material != NULL)
				{
					// This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
					FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

					// Check if it's layeredtextures
					int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

					if (layeredTextureCount > 0)
					{
						for (int j = 0; j < layeredTextureCount; j++)
						{
							FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
							int lcount = layered_texture->GetSrcObjectCount<FbxTexture>();

							for (int k = 0; k < lcount; k++)
							{
								FbxFileTexture* texture = FbxCast<FbxFileTexture>(layered_texture->GetSrcObject<FbxTexture>(k));
								// Then, you can get all the properties of the texture, include its name
								const char* textureName = texture->GetFileName();
							}
						}
					}
					else
					{
						// Directly get textures
						int textureCount = prop.GetSrcObjectCount<FbxTexture>();
						for (int j = 0; j < textureCount; j++)
						{
							FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxTexture>(j));
							// Then, you can get all the properties of the texture, include its name

							FbxProperty p = texture->RootProperty.Find("Earth");

						}
					}
				}
			}

			ProcessFbxMesh(childNode, verts, indicies);
		}
	}
}

// Init
LetsDrawSomeStuff::LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint)
{
	HRESULT hr = S_OK;
	if (attatchPoint) // valid window?
	{
		//unsigned int width;
		//UINT height;
		//attatchPoint->GetWidth(width);
		//attatchPoint->GetHeight(height);
		// Create surface, will auto attatch to GWindow
		if (G_SUCCESS(GW::GRAPHICS::CreateGDirectX11Surface(attatchPoint, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT, &mySurface)))
		{
			mySurface->GetAspectRatio(ratio);
			// Grab handles to all DX11 base interfaces
			mySurface->GetDevice((void**)&myDevice);
			mySurface->GetSwapchain((void**)&mySwapChain);
			mySurface->GetContext((void**)&myContext);

			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)

			

			//Create Vertex Shader
			myDevice->CreateVertexShader(VertexShader_PPIV, sizeof(VertexShader_PPIV), nullptr, &myVertexShader);
			myDevice->CreateVertexShader(LightVertexShader_PPIV, sizeof(LightVertexShader_PPIV), nullptr, &myVertexLightShader);

			//Create Pixel Shader
			myDevice->CreatePixelShader(PixelShader_PPIV, sizeof(PixelShader_PPIV), nullptr, &myPixelShader);
			myDevice->CreatePixelShader(LightPixelShader_PPIV, sizeof(LightPixelShader_PPIV), nullptr, &myLightPixelShader);



			//Load in Planet model

			// Change the following filename to a suitable filename value.
			const char* lFilename = "Assets/planet.fbx";

			// Initialize the SDK manager. This object handles memory management.
			FbxManager* lSdkManager = FbxManager::Create();

			// Create the IO settings object.
			FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
			lSdkManager->SetIOSettings(ios);

			// Create an importer using the SDK manager.
			FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

			// Use the first argument as the filename for the importer.
			if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
				printf("Call to FbxImporter::Initialize() failed.\n");
				printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
				exit(-1);
			}
			// Create a new scene so that it can be populated by the imported file.
			FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

			// Import the contents of the file into the scene.
			lImporter->Import(lScene);

			// The file is imported, so get rid of the importer.
			lImporter->Destroy();

			// Process the scene and build DirectX Arrays
			ProcessFbxMesh(lScene->GetRootNode(), Planet, planetIndicies);

			//Load in Planet model

			// Change the following filename to a suitable filename value.
			lFilename = "Assets/spaceCraft.fbx";

			// Initialize the SDK manager. This object handles memory management.
			lSdkManager = FbxManager::Create();

			// Create the IO settings object.
			ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
			lSdkManager->SetIOSettings(ios);

			// Create an importer using the SDK manager.
			lImporter = FbxImporter::Create(lSdkManager, "");

			// Use the first argument as the filename for the importer.
			if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
				printf("Call to FbxImporter::Initialize() failed.\n");
				printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
				exit(-1);
			}
			// Create a new scene so that it can be populated by the imported file.
			lScene = FbxScene::Create(lSdkManager, "myScene");

			// Import the contents of the file into the scene.
			lImporter->Import(lScene);

			// The file is imported, so get rid of the importer.
			lImporter->Destroy();

			// Process the scene and build DirectX Arrays
			ProcessFbxMesh(lScene->GetRootNode(), Ship, shipIndicies);
			

			//Create Input Layout
			D3D11_INPUT_ELEMENT_DESC ieDesc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hr = myDevice->CreateInputLayout(ieDesc, ARRAYSIZE(ieDesc), VertexShader_PPIV, sizeof(VertexShader_PPIV), &myInputLayout);

			D3D11_BUFFER_DESC bDesc;
			D3D11_SUBRESOURCE_DATA subData;
			D3D11_RASTERIZER_DESC rDesc;

			ZeroMemory(&bDesc, sizeof(bDesc));
			ZeroMemory(&subData, sizeof(subData));
			ZeroMemory(&rDesc, sizeof(rDesc));

			rDesc.FillMode = D3D11_FILL_SOLID;
			rDesc.CullMode = D3D11_CULL_FRONT;
			rDesc.DepthBias = 0;
			rDesc.SlopeScaledDepthBias = 0.0f;
			rDesc.DepthBiasClamp = 0.0f;
			rDesc.DepthClipEnable = TRUE;
			rDesc.ScissorEnable = FALSE;
			rDesc.MultisampleEnable = FALSE;
			rDesc.AntialiasedLineEnable = FALSE;
			hr = myDevice->CreateRasterizerState(&rDesc, &mySkyboxRasterizerState);

			rDesc.FillMode = D3D11_FILL_SOLID;
			rDesc.CullMode = D3D11_CULL_BACK;
			rDesc.DepthBias = 0;
			rDesc.SlopeScaledDepthBias = 0.0f;
			rDesc.DepthBiasClamp = 0.0f;
			rDesc.DepthClipEnable = TRUE;
			rDesc.ScissorEnable = FALSE;
			rDesc.MultisampleEnable = FALSE;
			rDesc.AntialiasedLineEnable = FALSE;
			hr = myDevice->CreateRasterizerState(&rDesc, &mySunRasterizerState);
			hr = myDevice->CreateRasterizerState(&rDesc, &myEarthRasterizerState);
			hr = myDevice->CreateRasterizerState(&rDesc, &myMoonRasterizerState);
			hr = myDevice->CreateRasterizerState(&rDesc, &myMarsRasterizerState);
			hr = myDevice->CreateRasterizerState(&rDesc, &myShipRasterizerState);


			//Vertex Planet Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(Vertex) * Planet.size();
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;
			subData.pSysMem = Planet.data();
			hr = myDevice->CreateBuffer(&bDesc, &subData, &myVertexBuffer);

			//Index Planet Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(unsigned int) * planetIndicies.size();
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = planetIndicies.data();
			myDevice->CreateBuffer(&bDesc, &subData, &myIndexBuffer);

			//Vertex Ship Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(Vertex) * Ship.size();
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;
			subData.pSysMem = Ship.data();
			hr = myDevice->CreateBuffer(&bDesc, &subData, &myShipVertexBuffer);

			//Index Ship Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(unsigned int) * shipIndicies.size();
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = shipIndicies.data();
			myDevice->CreateBuffer(&bDesc, &subData, &myShipIndexBuffer);

			//Constant Buffer
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.ByteWidth = sizeof(ConstantBuffer);
			bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bDesc.CPUAccessFlags = 0;
			hr = myDevice->CreateBuffer(&bDesc, nullptr, &myConstantBuffer);

			hr = CreateDDSTextureFromFile(myDevice, L"Assets/earthTexture.dds", nullptr, &earthbumpShaderResource);
			hr = CreateDDSTextureFromFile(myDevice, L"Assets/earthbumpTexture.dds", nullptr, &earthShaderResource);
			hr = CreateDDSTextureFromFile(myDevice, L"Assets/sunTexture.dds", nullptr, &sunShaderResource);
			hr = CreateDDSTextureFromFile(myDevice, L"Assets/moonTexture.dds", nullptr, &moonShaderResource);
			hr = CreateDDSTextureFromFile(myDevice, L"Assets/marsTexture.dds", nullptr, &marsShaderResource);
			hr = CreateDDSTextureFromFile(myDevice, L"Assets/spaceShipTexture.dds", nullptr, &shipShaderResource);
			hr = CreateDDSTextureFromFile(myDevice, L"Assets/starSkyBox.dds", nullptr, &skyboxShaderResource);

			// Create the sample state
			D3D11_SAMPLER_DESC sampDesc = {};
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			hr = myDevice->CreateSamplerState(&sampDesc, &mySampler);

			//World Matrix
			worldMatrix = XMMatrixIdentity();
			copyWorld = XMMatrixIdentity();
			tempWorld = XMMatrixIdentity();

			//View Matrices
			viewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);
			ReturnViewMatrix = viewMatrix;

			//Projection Matrix
			projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(50.0f), ratio, 0.1f, 100.0f);


			skyMatrix = XMMatrixTranslationFromVector(XMLoadFloat4(&SkyPos));
			XMMATRIX skyScale = XMMatrixScaling(50.0f, 50.0f, 50.0f);
			skyMatrix *= skyScale;

		}
	}
}

// Shutdown
LetsDrawSomeStuff::~LetsDrawSomeStuff()
{
	// Release DX Objects aquired from the surface
	myDevice->Release();
	mySwapChain->Release();
	myContext->Release();
	myInputLayout->Release();
	myVertexShader->Release();
	myVertexLightShader->Release();
	myPixelShader->Release();
	myLightPixelShader->Release();
	myVertexBuffer->Release();
	myIndexBuffer->Release();
	myConstantBuffer->Release();
	earthbumpShaderResource->Release();
	earthShaderResource->Release();
	sunShaderResource->Release();
	moonShaderResource->Release();
	marsShaderResource->Release();
	shipShaderResource->Release();
	skyboxShaderResource->Release();
	mySampler->Release();
	mySunRasterizerState->Release();
	myEarthRasterizerState->Release();
	myMoonRasterizerState->Release();
	myMarsRasterizerState->Release();
	myShipRasterizerState->Release();

	// TODO: "Release()" more stuff here!
	//delete &WorldMatrix;
	//delete &ProjectionMatrix;

	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}
}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{		

		if (ratio != mySurface->GetAspectRatio(ratio)) {
			projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(50.0f), ratio, 0.1f, 100.0f);
		
		}

		// this could be changed during resolution edits, get it every frame
		ID3D11RenderTargetView *myRenderTargetView = nullptr;
		ID3D11DepthStencilView *myDepthStencilView = nullptr;

		if (G_SUCCESS(mySurface->GetRenderTarget((void**)&myRenderTargetView)))
		{
			// Grab the Z Buffer if one was requested
			if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
			{
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
				myDepthStencilView->Release();
			}

			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below

			ID3D11RenderTargetView* const targets[] = { myRenderTargetView };
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);

			//Clear Screen to Black			
			const float black[] = { 0.2f, 0.2f, 0.2f, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, black);
			
			
			if (myDriverType == D3D_DRIVER_TYPE_REFERENCE)
			{
				curDeg += (float)XM_PI * 0.0125f;
			}
			else
			{
				static ULONGLONG timeStart = 0;
				ULONGLONG timeCur = GetTickCount64();
				if (timeStart == 0)
					timeStart = timeCur;
				curDeg = (timeCur - timeStart) / 1000.0f;
			}



			//////////////////////////////////////////////
			// TODO: Set Up the Solar System parameters//
			////////////////////////////////////////////

			//Sun Position
			XMFLOAT4 SunPos = { 0.0f, 0.0f, 0.0f, 1.0f };
			//Sun Direction (Directional Light)
			XMFLOAT4 SunDir = { 0.0f, 0.0f, 1.0f, 1.0f };
			//Sun Color
			XMFLOAT4 SunColor = { 1.0f, 1.0f, 1.0f, 1.0f };

			//Earth Position
			XMFLOAT4 EarthPos = { 10.0f, 0.0f, 0.0f, 1.0f };
			//Earth Color
			XMFLOAT4 EarthColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			//Moon Position
			XMFLOAT4 MoonPos = { 7.0f, 0.0f, 0.0f, 1.0f };
			//Moon Color
			XMFLOAT4 MoonColor = { 1.0f, 0.5f, 0.5f, 0.5f };

			//Mars Position
			XMFLOAT4 MarsPos = { 25.0f, 0.0f, 0.0f, 1.0f };
			//Mars Color
			XMFLOAT4 MarsColor = { 1.0f, 1.0f, 1.0f, 1.0f };

			//Ship Position
			XMFLOAT4 ShipPos = { 27.0f, 0.0f, 0.0f, 1.0f };
			//Ship Color
			XMFLOAT4 ShipColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			
			//Tilt the Earth

			//Rotate the Earth around the Axis

			//earthMatrix = XMMatrixTranslationFromVector(1.0f * XMLoadFloat4(&EarthPos));

			XMFLOAT4X4 viewfloat;
			XMStoreFloat4x4(&viewfloat, viewMatrix);
			viewMatrix = XMLoadFloat4x4(&MoveCamera(viewfloat));
			

			//Enter Key: Reset Camera
			if (GetAsyncKeyState(VK_RETURN)) {
				viewMatrix = ReturnViewMatrix;
			}

			XMMATRIX RotateLight = XMMatrixRotationY(-curDeg);
			XMVECTOR LightVec = XMLoadFloat4(&SunDir);
			LightVec = XMVector3Transform(LightVec, RotateLight);
			XMStoreFloat4(&SunDir, LightVec);


			
			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your Vertex & index buffers, Set your InputLayout & Topology & Draw!

			//Set Input Layout
			myContext->IASetInputLayout(myInputLayout);

			//Set Primitive Topology
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			
			UINT strides[] = { sizeof(Vertex) };
			UINT offsets[] = { 0 };
			ID3D11Buffer *tempVB[] = { myVertexBuffer };

			//Set Vertex Buffer
			myContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);

			//Set Index Buffer
			myContext->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			
			//Set Constant Buffer
			ConstantBuffer constBuff;
			constBuff.cWorld = XMMatrixTranspose(worldMatrix);
			constBuff.cView = XMMatrixTranspose(viewMatrix);
			constBuff.cProjection = XMMatrixTranspose(projectionMatrix);
			constBuff.cRotateY = XMMatrixRotationY(0.0f);
			constBuff.cLightPos = SunPos;
			constBuff.cLightDir = SunDir;
			constBuff.cLightColor = SunColor;
			constBuff.cOutputColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);

		//TODO: Render SkyBox
			myContext->RSSetState(mySkyboxRasterizerState);

			//Move SkyBox


			if (GetAsyncKeyState('W')) {
				//viewMatrix *= XMMatrixTranslation(0.0f, 0.0f, -0.01f);
				tempWorld *= XMMatrixTranslation(0.0f, 0.0f, 0.01f);
				copyWorld = worldMatrix;
				copyWorld = copyWorld * tempWorld;
			}

			if (GetAsyncKeyState('A')) {
				//viewMatrix *= XMMatrixTranslation(0.0f, 0.0f, -0.01f);
				tempWorld *= XMMatrixTranslation(-0.01f, 0.0f, 0.0f);
				copyWorld = worldMatrix;
				copyWorld = copyWorld * tempWorld;
			}

			if (GetAsyncKeyState('S')) {
				//viewMatrix *= XMMatrixTranslation(0.0f, 0.0f, -0.01f);
				tempWorld *= XMMatrixTranslation(0.0f, 0.0f, -0.01f);
				copyWorld = worldMatrix;
				copyWorld = copyWorld * tempWorld;
			}

			if (GetAsyncKeyState('D')) {
				//viewMatrix *= XMMatrixTranslation(0.0f, 0.0f, -0.01f);
				tempWorld *= XMMatrixTranslation(0.01f, 0.0f, 0.0f);
				copyWorld = worldMatrix;
				copyWorld = copyWorld * tempWorld;
			}
			//Change Constant Buffer
			constBuff.cWorld = XMMatrixTranspose(copyWorld) * skyMatrix;
			constBuff.cOutputColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			//Update Constant Buffer
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);

			//Set Vertex Shader and Vertex Constant Buffer
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->VSSetShader(myVertexShader, nullptr, 0);

			//Set Pixel Shader, Pixel Constant Buffer, Shader Resource and Samplers
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myLightPixelShader, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &skyboxShaderResource);
			myContext->PSSetSamplers(0, 1, &mySampler);

			//Draw Sun
			myContext->DrawIndexed(planetIndicies.size(), 0, 0);

		//TODO: Render Sun
			myContext->RSSetState(mySunRasterizerState);
						
			//Positioning and Scaling
			sunMatrix = XMMatrixTranslationFromVector(1.0f * XMLoadFloat4(&SunPos));
			XMMATRIX sunScale = XMMatrixScaling(4.0f, 4.0f, 4.0f);
			sunMatrix = sunScale * sunMatrix;

			//Change Constant Buffer
			constBuff.cWorld = XMMatrixTranspose(sunMatrix) * XMMatrixRotationY(0.5f * curDeg);
			constBuff.cOutputColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			//Update Constant Buffer
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);

			//Set Vertex Shader and Vertex Constant Buffer
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->VSSetShader(myVertexShader, nullptr, 0);

			//Set Pixel Shader, Pixel Constant Buffer, Shader Resource and Samplers
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myLightPixelShader, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &sunShaderResource);
			myContext->PSSetSamplers(0, 1, &mySampler);

			//Draw Sun
			myContext->DrawIndexed(planetIndicies.size(), 0, 0);

		//TODO: Render Earth
			myContext->RSSetState(myEarthRasterizerState);
			
			//Orbiting
			XMMATRIX earthOrbit = XMMatrixRotationY(-0.027f * curDeg);
			XMVECTOR earthOrbitVec = XMLoadFloat4(&EarthPos);
			earthOrbitVec = XMVector3Transform(earthOrbitVec, earthOrbit);
			XMStoreFloat4(&EarthPos, earthOrbitVec);

			//Postioning and Scaling
			earthMatrix = XMMatrixTranslationFromVector(XMLoadFloat4(&EarthPos));
			
			//Change Constant Buffer
			constBuff.cWorld = XMMatrixTranspose(earthMatrix) * XMMatrixRotationY(-1.0f * curDeg);

			//Set Vertex Shader and Vertex Constant Buffer
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->VSSetShaderResources(0, 1, &earthbumpShaderResource);

			//Update Constant Buffer
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);
			
			//Set Pixel Shader and Pixel Constant Buffer
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &earthShaderResource);
			myContext->PSSetSamplers(0, 1, &mySampler);

			//Draw Earth
			myContext->DrawIndexed(planetIndicies.size(), 0, 0);

		//TODO: Render the Moon
			myContext->RSSetState(myMoonRasterizerState);

			//Orbiting
			XMVECTOR MoonVec = XMLoadFloat4(&MoonPos);
			XMVECTOR newEarthVec = XMLoadFloat4(&EarthPos);
			XMMATRIX RotateMoon = XMMatrixRotationY(-0.037f * curDeg);
			MoonVec = XMVector3Transform(MoonVec, RotateMoon);
			XMStoreFloat4(&MoonPos, MoonVec);

			//Positioning and Scaling
			moonMatrix = XMMatrixTranslationFromVector(XMLoadFloat4(&MoonPos));
			XMMATRIX moonScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
			moonMatrix *= moonScale;

			moonMatrix *= XMMatrixTranslation(EarthPos.x, EarthPos.y, EarthPos.z);
			//Rotate Around Axis

			//Change Constant Buffer
			constBuff.cWorld = XMMatrixTranspose(moonMatrix) * XMMatrixRotationY(-0.037f * curDeg);

			//Set Vertex Shader and Vertex Constant Buffer
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->VSSetShader(myVertexShader, nullptr, 0);

			//Update Constant Buffer
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);

			//Set Pixel Shader, Pixel Constant Buffer, Shader Resource, and Sampler
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &moonShaderResource);
			myContext->PSSetSamplers(0, 1, &mySampler);

			//Draw Moon
			myContext->DrawIndexed(planetIndicies.size(), 0, 0);

		//TODO: Render Mars
			myContext->RSSetState(myMarsRasterizerState);

			//Orbiting
			XMVECTOR marsOrbitVec = XMLoadFloat4(&MarsPos);
			XMMATRIX marsOrbit = XMMatrixRotationY(-0.015f * curDeg);
			marsOrbitVec = XMVector3Transform(marsOrbitVec, marsOrbit);
			XMStoreFloat4(&MarsPos, marsOrbitVec);

			//Postioning and Scaling
			marsMatrix = XMMatrixTranslationFromVector(XMLoadFloat4(&MarsPos));
			XMMATRIX marsScale = XMMatrixScaling(0.53f, 0.53f, 0.53f);
			marsMatrix *= marsScale;

			//Change Constant Buffer
			constBuff.cWorld = XMMatrixTranspose(marsMatrix) * XMMatrixRotationY(1.026f * curDeg);

			//Set Vertex Shader and Vertex Constant Buffer
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->VSSetShader(myVertexShader, nullptr, 0);

			//Update Constant Buffer
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);

			//Set Pixel Shader and Pixel Constant Buffer
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &marsShaderResource);
			myContext->PSSetSamplers(0, 1, &mySampler);

			//Draw Mars
			myContext->DrawIndexed(planetIndicies.size(), 0, 0);

		//TODO: Render the Ship
			myContext->RSSetState(myShipRasterizerState);

			UINT shipStrides[] = { sizeof(Vertex) };
			UINT shipOffsets[] = { 0 };
			ID3D11Buffer *shipTempVB[] = { myShipVertexBuffer };

			//Set Vertex Buffer
			myContext->IASetVertexBuffers(0, 1, shipTempVB, shipStrides, shipOffsets);

			//Set Index Buffer
			myContext->IASetIndexBuffer(myShipIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			//Orbiting
			XMVECTOR ShipVec = XMLoadFloat4(&ShipPos);
			XMVECTOR oldFaceVec = ShipVec;
			XMMATRIX RotateShip = XMMatrixRotationY(0.75f * curDeg);
			ShipVec = XMVector3Transform(ShipVec, RotateShip);
			XMVECTOR vecOfDiff = XMVector3AngleBetweenVectors(ShipVec, oldFaceVec);
			XMFLOAT4 floatOfDiff;
			XMStoreFloat4(&floatOfDiff, vecOfDiff);
			XMStoreFloat4(&ShipPos, ShipVec);
			printf("Current z position of ship: %f\n", ShipPos.z);


			//Positioning and Scaling
			//shipMatrix = XMMatrixTranslationFromVector(XMLoadFloat4(&ShipPos));
			shipMatrix = XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
			if (ShipPos.z >= 0) {
				shipMatrix *= XMMatrixRotationY(-floatOfDiff.y);
			}
			if (ShipPos.z < 0) {
				shipMatrix *= XMMatrixRotationY(floatOfDiff.y);
			}
			XMMATRIX shipScale = XMMatrixScaling(0.05f, 0.05f, 0.05f);
			shipMatrix = shipMatrix * XMMatrixTranslationFromVector(1.8f * XMLoadFloat4(&ShipPos)) * shipScale;
			shipMatrix *= XMMatrixTranslation(EarthPos.x, EarthPos.y, EarthPos.z);
			//Rotate Around Axis

			//Change Constant Buffer
			constBuff.cWorld = XMMatrixTranspose(shipMatrix);

			//Set Vertex Shader and Vertex Constant Buffer
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->VSSetShader(myVertexShader, nullptr, 0);

			//Update Constant Buffer
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &constBuff, 0, 0);

			//Set Pixel Shader, Pixel Constant Buffer, Shader Resource, and Sampler
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &shipShaderResource);
			myContext->PSSetSamplers(0, 1, &mySampler);

			//Draw Ship
			myContext->DrawIndexed(shipIndicies.size(), 0, 0);

			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();

		}
	}
}