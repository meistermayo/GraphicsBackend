#include "GraphicsBackend.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Model.h"

#ifdef BACKEND_D3D
#include <d3d11.h>
#include "d3dUtil.h"
#include <d3dcompiler.h>
#include <d3dcommon.h>
#endif

std::wstring stringToWString(std::string inStr)
{
	// this resizes by inStr.size() scales by wchar_t-sizes, not bytes.
	std::wstring wStr;
	wStr.resize(inStr.size());

	// may have to check this later... seems to return 0 but doesn't fail what I want it to do...
	MultiByteToWideChar(CP_ACP, 0, inStr.c_str(), -1, &wStr[0], inStr.size());

	return wStr;
}

GraphicsBackend_Base* GraphicsBackend::pInst = nullptr;

void VertexBufferObject::LoadToGPU()
{
#ifdef BACKEND_D3D
		// Vertex buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(StandardVertex) * num;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = pVerts;
		HRESULT hr = GraphicsBackend::GetDevice().md3dDevice->CreateBuffer(&bd, &InitData, &mpVertexBuffer);
		assert(SUCCEEDED(hr));
#endif
}

VertexBufferObject::~VertexBufferObject()
{
	ReleaseAndDeleteCOMobject(mpVertexBuffer);
}

void VertexBufferObject::Bind()
{
#ifdef BACKEND_D3D
	UINT stride = sizeof(StandardVertex);
	UINT offset = 0;
	GraphicsBackend::GetContext().md3dImmediateContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
#endif
}

void IndexBufferObject::LoadToGPU()
{
#ifdef BACKEND_D3D
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(TriangleByIndex) * num;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = pTris;
		HRESULT hr = GraphicsBackend::GetDevice().md3dDevice->CreateBuffer(&bd, &InitData, &mpIndexBuffer);
		assert(SUCCEEDED(hr));
#endif
}

void IndexBufferObject::Bind()
{
#ifdef BACKEND_D3D
	GraphicsBackend::GetContext().md3dImmediateContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
#endif
}

IndexBufferObject::~IndexBufferObject()
{
	ReleaseAndDeleteCOMobject(mpIndexBuffer);
}

void TextureSampler::LoadTexture(std::string filepath, bool ComputeMip, size_t miplevel, uint32_t filterflags)
{
#ifdef BACKEND_D3D
	std::wstring lpFilepath = stringToWString(filepath);

	size_t pos = lpFilepath.find_last_of(L'.');
	std::wstring ext = lpFilepath.substr(pos + 1);

	DirectX::ScratchImage scrtTex;
	HRESULT hr = S_OK;

	if (ext == L"tga" || ext == L"TGA")
	{
		hr = LoadFromTGAFile(lpFilepath.c_str(), nullptr, scrtTex);
	}
	else if (ext == L"dds" || ext == L"DDS")
	{
		hr = LoadFromDDSFile(lpFilepath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, scrtTex);
	}
	else
	{
		assert(false && "ERROR: Invalid file format");
	}
	assert(SUCCEEDED(hr));

	if (ComputeMip)
	{
		assert(scrtTex.GetImageCount() == 1 && "ERROR: File already contains MIP map.");
		DirectX::ScratchImage mipchain;
		hr = DirectX::GenerateMipMaps(*(scrtTex.GetImage(0, 0, 0)), filterflags, miplevel, mipchain);
		assert(SUCCEEDED(hr));

		CreateShaderResourceView(GraphicsBackend::GetDevice().md3dDevice, mipchain.GetImage(0, 0, 0), mipchain.GetImageCount(), mipchain.GetMetadata(), &mpTextureRV);
	}
	else
	{
		CreateShaderResourceView(GraphicsBackend::GetDevice().md3dDevice, scrtTex.GetImage(0, 0, 0), scrtTex.GetImageCount(), scrtTex.GetMetadata(), &mpTextureRV);
	}

	width = scrtTex.GetMetadata().width;
	height = scrtTex.GetMetadata().height;
#endif
	// crash 3 - scratch image ??? todo
}

void TextureSampler::LoadColorTexture(Vect color)
{
	width = 1;
	height = 1;

#ifdef BACKEND_D3D
	float colArr[4]{ color.x, color.y, color.z, color.w };

	DirectX::ScratchImage scrtTex;
	DirectX::Image image;
	image.format = DXGI_FORMAT_R32G32B32_FLOAT;
	image.height = 1;
	image.width = 1;
	image.pixels = reinterpret_cast<uint8_t*>(colArr);
	scrtTex.InitializeFromImage(image);

	CreateShaderResourceView(GraphicsBackend::GetDevice().md3dDevice, scrtTex.GetImage(0, 0, 0), scrtTex.GetImageCount(), scrtTex.GetMetadata(), &mpTextureRV);
#endif
}

void TextureSampler::CreateSampleState(uint32_t filter, uint32_t isotropic_level)
{
#ifdef BACKEND_D3D
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = (D3D11_FILTER)filter;
	sampDesc.MaxAnisotropy = isotropic_level;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = GraphicsBackend::GetDevice().md3dDevice->CreateSamplerState(&sampDesc, &mpSampler);
	assert(SUCCEEDED(hr));
#endif
}

void TextureSampler::SetToContext(int texResSlot, int sampSlot)
{
#ifdef BACKEND_D3D
	GraphicsBackend::GetContext().md3dImmediateContext->PSSetShaderResources(texResSlot, 1, &mpTextureRV);
	GraphicsBackend::GetContext().md3dImmediateContext->PSSetSamplers(sampSlot, 1, &mpSampler);
#endif
}

TextureSampler::~TextureSampler()
{
#ifdef BACKEND_D3D
	ReleaseAndDeleteCOMobject(mpTextureRV);
	ReleaseAndDeleteCOMobject(mpSampler);
#endif
}

void ShaderInterface::BuildShaders(std::string filename)
{
#ifdef BACKEND_D3D
	LPCSTR wVsModel = "vs_4_0";
	LPCSTR wPsModel = "ps_4_0";
	std::wstring wFilestr = stringToWString(filename + GraphicsBackend::GetVertexShaderExt()); // todo - check if this ext exists
	const WCHAR* wFilename = wFilestr.c_str();

	pVSBlob = nullptr;
	HRESULT hr = CompileShaderFromFileD3D(wFilename, "VS", wVsModel, &pVSBlob);
	assert(SUCCEEDED(hr));

	hr = GraphicsBackend::GetDevice().md3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader);
	assert(SUCCEEDED(hr));

	pPSBlob = nullptr;
	hr = CompileShaderFromFileD3D(wFilename, "PS", wPsModel, &pPSBlob);
	assert(SUCCEEDED(hr));

	// Create the pixel shader
	hr = GraphicsBackend::GetDevice().md3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);
	assert(SUCCEEDED(hr));
#endif
}

void ShaderInterface::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layoutdesc, UINT size)
{
#ifdef BACKEND_D3D
	// Create the input layout
	HRESULT hr = GraphicsBackend::GetDevice().md3dDevice->CreateInputLayout(layoutdesc, size, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mpVertexLayout);
	assert(SUCCEEDED(hr));
#endif
}

// Sets the DX context to use these VS, PS and input layout
void ShaderInterface::SetToContext_VS_PS_InputLayout()
{
#ifdef BACKEND_D3D
	assert(GraphicsBackend::GetContext().md3dImmediateContext != nullptr && "ERROR: Context not set.");
	GraphicsBackend::GetContext().md3dImmediateContext->VSSetShader(mpVertexShader, nullptr, 0);
	GraphicsBackend::GetContext().md3dImmediateContext->PSSetShader(mpPixelShader, nullptr, 0);
	GraphicsBackend::GetContext().md3dImmediateContext->IASetInputLayout(mpVertexLayout);
#endif
}

#ifdef BACKEND_D3D
HRESULT ShaderInterface::CompileShaderFromFileD3D(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif


	ID3DBlob* pErrorBlob = nullptr;

	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

ShaderInterface::~ShaderInterface()
{
#ifdef BACKEND_D3D
	ReleaseAndDeleteCOMobject(pVSBlob);
	ReleaseAndDeleteCOMobject(pPSBlob);
	ReleaseAndDeleteCOMobject(mpVertexShader);
	ReleaseAndDeleteCOMobject(mpPixelShader);
	ReleaseAndDeleteCOMobject(mpVertexLayout);
#endif
}

#endif