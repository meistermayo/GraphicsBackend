#ifndef GRAPHICS_BACKEND_H
#define GRAPHICS_BACKEND_H

#include <string>

#define BACKEND_D3D

#include "src/Graphics/CrazySpaceMeatLand/src/Align16.h"

class Model;
struct StandardVertex;
struct TriangleByIndex;

#ifdef BACKEND_D3D

#include <WindowsX.h>
#include "src/Graphics/CrazySpaceMeatLand/DX Application/Resource.h"
#include <cassert>
#include "src/Graphics/CrazySpaceMeatLand/src/DXApp.h"
#include "src/i Engine/i.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Math/Vect.h"
#include "src/i Engine/Input/Keys.h" // todo -- no no

#endif

struct GraphicsDevice;
struct GraphicsContext;

class GraphicsBackend_Base : public Align16 {
	friend class GraphicsBackend;

protected:
	virtual int privInitialize() = 0;
	virtual void privInitApp() = 0;
	virtual bool privStillOpen() = 0;
	virtual void privPoll() = 0;
	virtual void privPrepare() = 0;
	virtual void privPresent() = 0;
	virtual void privCleanupApp() = 0;
	virtual void privSetClearColor(float r, float g, float b, float a = 1.0f) = 0;
	virtual const GraphicsDevice& privGetDevice() const = 0;
	virtual const GraphicsContext& privGetContext() const = 0;
	virtual const std::string& privGetBackendFolder() const = 0;
	virtual const std::string& privGetVertexShaderExt() const = 0;
	virtual const std::string& privGetFragmentShaderExt() const = 0;
	virtual const float privGetTime() const = 0;
	virtual const bool privGetKeyState(KEY_CODE key) const = 0;

	virtual void privSetPrimitiveTopologyAsTriList() const = 0;
	virtual void privDrawIndexed(int indexCount, int startIndex, int baseVertex) const = 0;
	virtual const int privGetWindowWidth() const = 0;
	virtual const int privGetWindowHeight() const = 0;

public:
	virtual void OnKey(KEY_CODE key, bool down) = 0;
};

struct GraphicsDevice : public Align16
{
#ifdef BACKEND_D3D
	ID3D11Device* md3dDevice = nullptr;
#endif
};

struct GraphicsContext : public Align16
{
#ifdef BACKEND_D3D
	ID3D11DeviceContext* md3dImmediateContext;
#endif
};

struct VertexBufferObject : public Align16
{
	int num;
	StandardVertex* pVerts;
#ifdef BACKEND_D3D
	ID3D11Buffer* mpVertexBuffer;

	~VertexBufferObject();
#endif

	void LoadToGPU();
	void Bind();
};

struct IndexBufferObject : public Align16
{
	int num;
	TriangleByIndex* pTris;
#ifdef BACKEND_D3D
	ID3D11Buffer* mpIndexBuffer;

	~IndexBufferObject();
#endif

	void LoadToGPU();
	void Bind();
};

template <typename T>
struct GenericBufferObject : public Align16
{
#ifdef BACKEND_D3D
	ID3D11Buffer* mpBuffer;
	int index = 0;
#endif

	void CreateBuffer(int inIndex)
	{
#ifdef BACKEND_D3D
		index = inIndex;

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(T);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		HRESULT hr = GraphicsBackend::GetDevice().md3dDevice->CreateBuffer(&bd, nullptr, &mpBuffer);
		assert(SUCCEEDED(hr));
#endif
	}

	void UpdateBuffer(T* t)
	{
#ifdef BACKEND_D3D
		GraphicsBackend::GetContext().md3dImmediateContext->UpdateSubresource(mpBuffer, 0, nullptr, t, 0, 0);
#endif
	}

	void Bind()
	{
#ifdef BACKEND_D3D
		GraphicsBackend::GetContext().md3dImmediateContext->VSSetConstantBuffers(index, 1, &mpBuffer);
		GraphicsBackend::GetContext().md3dImmediateContext->PSSetConstantBuffers(index, 1, &mpBuffer);
#endif
	}

	~GenericBufferObject()
	{
#ifdef BACKEND_D3D
		ReleaseAndDeleteCOMobject(mpBuffer);
#endif
	}
};

// class? todo
struct TextureSampler : public Align16
{
	int width;
	int height;

#ifdef BACKEND_D3D
	ID3D11ShaderResourceView* mpTextureRV;
	ID3D11SamplerState* mpSampler;
#endif

#ifdef BACKEND_D3D
	void LoadTexture(std::string filepath, bool ComputeMip = false, size_t miplevel = 0, uint32_t filterflags = DirectX::TEX_FILTER_LINEAR);
	void LoadColorTexture(Vect color);
	void CreateSampleState(uint32_t filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, uint32_t isotropic_level = 4);
#endif BACKEND_D3D

	void SetToContext(int texResSlot, int sampSlot);

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	~TextureSampler();
};

struct ShaderInterface : public Align16
{
#ifdef BACKEND_D3D
	// Actual objects managed by this class
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpVertexLayout;

	// Needed for building shaders
	ID3DBlob* pVSBlob;
	ID3DBlob* pPSBlob;
#endif

	// Compile and send the VS and PS shaders to the GPU
	void BuildShaders(std::string filename);

	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layoutdesc, UINT size);

	// Sets the DX context to use these VS, PS and input layout
	void SetToContext_VS_PS_InputLayout();

#ifdef BACKEND_D3D
	HRESULT CompileShaderFromFileD3D(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
#endif

	void DefineInputLayout() // todo
	{
#ifdef BACKEND_D3D
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);
		CreateInputLayout(layout, numElements);
#endif
	}
	void DefineInputLayoutNormal() {
#ifdef BACKEND_D3D
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);
		this->CreateInputLayout(layout, numElements);
#endif
	} // todo again

	void DefineInputLayoutNormalTex() // todo again
	{
#ifdef BACKEND_D3D
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);
		this->CreateInputLayout(layout, numElements);

#endif
	}
	void DefineInputLayoutTex()
	{
#ifdef BACKEND_D3D
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);
		this->CreateInputLayout(layout, numElements);
#endif
	}
	~ShaderInterface();
};

#ifdef BACKEND_OGL
class OGL_GraphicsBackend : public GraphicsBackend_Base {

	// use these
	const std::string VertexShaderExt = ".vs.glsl";
	const std::string FragmentShaderExt = ".fs.glsl";

public:
};
#endif

#ifdef BACKEND_D3D
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class D3D_GraphicsBackend : public GraphicsBackend_Base
{
	HINSTANCE               hInst = nullptr;
	HWND                    hWnd = nullptr;
	int nCmdShow = 0;

	const std::string mBackendFolder = "D3D/";
	const std::string mVertexShaderExt = ".hlsl";
	const std::string mFragmentShaderExt = ".hlsl";

	Vect BackgroundColor;

	bool keyStates[KEY_CODE::KEY_COUNT] { false };

	// DX application elements
	GraphicsDevice mDev;						// Connects to the graphics card
	GraphicsContext mCon;						// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations

	std::wstring mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	MSG msg = { 0 };

public:
	void SetData(HINSTANCE hInstance, int inCmdShow);
	HWND GetWindowHandle() { return hWnd; }

protected:
	virtual int privInitialize() override;
	virtual void privInitApp() override;
	virtual bool privStillOpen() override;
	virtual void privPoll() override;
	virtual void privPrepare() override;
	virtual void privPresent() override;
	virtual void privCleanupApp() override;
	virtual void privSetClearColor(float r, float g, float b, float a);

	virtual const GraphicsDevice& privGetDevice() const override;
	virtual const GraphicsContext& privGetContext() const override;
	virtual const std::string& privGetBackendFolder() const override;
	virtual const std::string& privGetVertexShaderExt() const override;
	virtual const std::string& privGetFragmentShaderExt() const override;
	virtual const float privGetTime() const override
	{
		static LARGE_INTEGER frequency;
		static bool initialized = false;
		static LARGE_INTEGER start;

		if (!initialized) {
			// Get the frequency of the performance counter
			QueryPerformanceFrequency(&frequency);

			// Get the initial time
			QueryPerformanceCounter(&start);

			initialized = true;
		}

		// Get the current time
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);

		// Calculate elapsed time in seconds
		return static_cast<float>(current.QuadPart - start.QuadPart) / frequency.QuadPart;
	}
	virtual const bool privGetKeyState(KEY_CODE key) const { return keyStates[key]; }
	virtual const int privGetWindowHeight() const override { return mClientHeight; }
	virtual const int privGetWindowWidth() const override { return mClientWidth; }

	virtual void privDrawIndexed(int indexCount, int startIndex, int baseVertex) const override;
	virtual void privSetPrimitiveTopologyAsTriList() const override;

	void InitDirect3D();

public:
	virtual void OnKey(KEY_CODE key, bool down) override { keyStates[key] = down; }

protected:
	HRESULT InitWindow()
	{
		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInst;
		wcex.hIcon = LoadIcon(hInst, (LPCTSTR)IDI_TUTORIAL1);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = (LPCSTR)L"MainlWindowClass";
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
		if (!RegisterClassEx(&wcex))
			return E_FAIL;

		// Create window
		RECT rc = { 0, 0, 1280, 720}; // Initial window dimentions. 

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindow((LPCSTR)L"MainlWindowClass", (LPCSTR)L"Main Window",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInst,
			nullptr);
		if (!hWnd)
			return E_FAIL;

		// Note: We won't bother dealing with resizing/fullscreen, etc

		ShowWindow(hWnd, nCmdShow);

		return S_OK;
	}
};
#endif

class GraphicsBackend : public Align16 {
	static GraphicsBackend_Base* pInst;

public:
	static GraphicsBackend_Base* Instance()
	{
		if (pInst == nullptr)
		{
#ifdef BACKEND_D3D
			pInst = new D3D_GraphicsBackend();
#endif
#ifdef BACKEND_OGL
			pInst = new OGL_GraphicsBackend();
#endif
		}

		return pInst;
	}

	static int Initialize() { return Instance()->privInitialize(); }
	static void InitApp() { Instance()->privInitApp(); }
	static bool StillOpen() { return Instance()->privStillOpen(); }
	static void Poll() { Instance()->privPoll(); }
	static void Prepare() { Instance()->privPrepare(); }
	static void Present() { Instance()->privPresent(); }
	static void CleanupApp() { Instance()->privCleanupApp(); }
	static void SetClearColor(float r, float g, float b, float a = 1.0f) { Instance()->privSetClearColor(r, g, b, a); }
	static const GraphicsDevice& GetDevice() { return Instance()->privGetDevice(); }
	static const GraphicsContext& GetContext() { return Instance()->privGetContext(); }
	static const std::string& GetBackendFolder() { return Instance()->privGetBackendFolder(); }
	static const std::string& GetVertexShaderExt() { return Instance()->privGetVertexShaderExt(); }
	static const std::string& GetFragmentShaderExt() { return Instance()->privGetFragmentShaderExt(); }
	static const float GetTime() { return Instance()->privGetTime(); }
	static const bool GetKeyState(KEY_CODE key) { return Instance()->privGetKeyState(key); }
	static const int GetWindowHeight() { return Instance()->privGetWindowHeight(); }
	static const int GetWindowWidth() { return Instance()->privGetWindowWidth(); }

	static void SetPrimitiveTopologyAsTriList() { Instance()->privSetPrimitiveTopologyAsTriList(); }
	static void DrawIndexed(int indexCount, int startIndex, int baseVertex) { Instance()->privDrawIndexed(indexCount, startIndex, baseVertex); }
};

#ifdef BACKEND_D3D
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	D3D_GraphicsBackend* pBackend = (D3D_GraphicsBackend*)GraphicsBackend::Instance();
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		// Early exit using ESC key: very useful during development
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
		pBackend->OnKey(static_cast<KEY_CODE>(wParam), true);
		break;

	case WM_KEYUP:
		pBackend->OnKey(static_cast<KEY_CODE>(wParam), false);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		SetCapture(pBackend->GetWindowHandle());
		i::BackendOnMouseButton(wParam, 1);
		break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		ReleaseCapture();
		i::BackendOnMouseButton(wParam, 0);
		break;

	case WM_MOUSEMOVE:
		i::BackendOnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSEWHEEL:
		i::BackendOnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
#endif

#endif