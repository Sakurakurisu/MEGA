#include "D3DMgr.h"
#include "Vertex.h"
#include "DXTrace.h"
#include <d2d1_1.h>
#include "Debug.h"
#include <dxgi.h>

D3DMgr::~D3DMgr()
{
	Uninit();
}

const std::vector<UINT> discreteVendorIDs = {
	0x10DE, // NVIDIA
	0x1002, // AMD
	0x8086  // Intel
};

// 最適なアダプターを選択する関数
ComPtr<IDXGIAdapter> D3DMgr::SelectAdapter()
{
	ComPtr<IDXGIFactory1> factory;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factory.GetAddressOf()));
	if (FAILED(hr))
	{
		Debug::Log("Failed to create DXGI factory.", Debug::LogLevel::ERR);
		return nullptr;
	}

	ComPtr<IDXGIAdapter1> adapter;
	ComPtr<IDXGIAdapter> bestAdapter;
	SIZE_T maxDedicatedVideoMemory = 0;
	UINT adapterIndex = 0;

	// NVIDIAアダプターを優先的に選択
	while (factory->EnumAdapters1(adapterIndex, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		hr = adapter->GetDesc1(&desc);
		if (FAILED(hr))
		{
			adapterIndex++;
			adapter.Reset();
			continue;
		}

		// NVIDIAのアダプターをチェック
		if (desc.VendorId == 0x10DE)
		{
			// より多くの専用ビデオメモリを持つアダプターを選択
			if (desc.DedicatedVideoMemory > maxDedicatedVideoMemory)
			{
				bestAdapter = adapter;
				maxDedicatedVideoMemory = desc.DedicatedVideoMemory;
			}
		}

		adapterIndex++;
		adapter.Reset();
	}

	// NVIDIAアダプターが見つからない場合、最大メモリのアダプターを選択
	if (bestAdapter) return bestAdapter;

	adapterIndex = 0;
	maxDedicatedVideoMemory = 0;
	while (factory->EnumAdapters1(adapterIndex, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		hr = adapter->GetDesc1(&desc);
		if (FAILED(hr))
		{
			adapterIndex++;
			adapter.Reset();
			continue;
		}

		// 最大の専用ビデオメモリを持つアダプターを選択
		if (desc.DedicatedVideoMemory > maxDedicatedVideoMemory)
		{
			bestAdapter = adapter;
			maxDedicatedVideoMemory = desc.DedicatedVideoMemory;
		}

		adapterIndex++;
		adapter.Reset();
	}

	return bestAdapter;
}

HRESULT D3DMgr::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;

	// 最適なアダプターを選択
	ComPtr<IDXGIAdapter> selectedAdapter = SelectAdapter();

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Direct3Dデバイスとスワップチェーンの作成
	hr = D3D11CreateDeviceAndSwapChain(
		selectedAdapter.Get(),
		selectedAdapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&swapChain,
		&d3dDevice,
		&featureLevel,
		&immediateContext
	);

	if (FAILED(hr)) return hr;

	// バックバッファ取得・レンダーターゲットビュー作成
	ComPtr<ID3D11Texture2D> pBackBuffer;
	HR(swapChain->ResizeBuffers(1, SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_B8G8R8A8_UNORM, 0));
	HR(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf())));
	HR(d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &renderTargetView));

	// 深度ステンシルバッファ作成
	ComPtr<ID3D11Texture2D> depthTexture;
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	HR(d3dDevice->CreateTexture2D(&td, nullptr, &depthTexture));

	// 深度ステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	HR(d3dDevice->CreateDepthStencilView(depthTexture.Get(), &dsvd, &depthStencilView));

	// レンダーターゲットと深度ステンシルビューをバインド
	immediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// ビューポート設定
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<FLOAT>(SCREEN_WIDTH);
	vp.Height = static_cast<FLOAT>(SCREEN_HEIGHT);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	immediateContext->RSSetViewports(1, &vp);

	return S_OK;
}

bool D3DMgr::InitDirect2D()
{
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#ifdef _DEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	// Direct2Dファクトリーの作成
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options, reinterpret_cast<void**>(d2dFactory1.GetAddressOf())));

	ComPtr<IDXGIDevice> dxgiDevice;
	// Direct3DデバイスからDXGIデバイスを取得
	HR(d3dDevice.As(&dxgiDevice));

	ComPtr<ID2D1Device> d2dDevice;
	// DXGIデバイスからDirect2Dデバイスを作成
	HRESULT hr = d2dFactory1->CreateDevice(dxgiDevice.Get(), &d2dDevice);

	if (hr == E_NOINTERFACE)
	{
		// D2DとD3Dの相互運用ができない場合の警告
		Debug::Log("The interoperability between Direct2D and Direct3D is limited, and you will not be able to view text information.", Debug::LogLevel::WARN);
		d2dFactory1 = nullptr;
	}
	else if (hr == S_OK)
	{
		// デバイスコンテキストの作成
		HR(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext));

		ComPtr<IDXGISurface> dxgiBackBuffer;
		// スワップチェーンのバックバッファを取得
		HR(swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)));

		// Direct2D用のビットマッププロパティを設定
		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

		ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
		// DXGIサーフェスからDirect2Dビットマップを作成
		HR(d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.Get(), &bitmapProperties, &d2dTargetBitmap));

		// Direct2Dの描画ターゲットを設定
		d2dDeviceContext->SetTarget(d2dTargetBitmap.Get());

		// DirectWriteファクトリーの作成
		HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(dwriteFactory.GetAddressOf())));
		// 白色のブラシを作成
		HR(d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), colorBrush.GetAddressOf()));
		// テキストフォーマットの作成（MS ゴシック、20pt、日本語）
		HR(dwriteFactory->CreateTextFormat(L"MS ゴシック", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
             DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"ja-jp",
             textFormat.GetAddressOf()));
	}

	return true;
}

// 終了処理
void D3DMgr::Uninit()
{
	if (constantBuffer)      constantBuffer->Release();
	if (renderTargetView)    renderTargetView->Release();
	if (depthStencilView)    depthStencilView->Release();
	if (swapChain)           swapChain->Release();
	if (immediateContext)    immediateContext->Release();
}

// バックバッファクリア
void D3DMgr::Clear()
{
	float clearColor[4] = { 0.192f, 0.302f, 0.475f, 1.0f };
	immediateContext->ClearRenderTargetView(renderTargetView, clearColor);
	immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

// プレゼント
void D3DMgr::Present()
{
	swapChain->Present(0, 0);
}

ID3D11Device* D3DMgr::GetDevice()
{
	return d3dDevice.Get();
}

#include "D3DMgr.h"  
#include "Vertex.h"  
#include "DXTrace.h"  
#include <d2d1_1.h>  
#include "Debug.h"  
#include <dxgi.h>  

// D3DMgrクラスのデストラクタ  
D3DMgr::~D3DMgr()  
{  
Uninit();  
}  

// 使用可能なGPUベンダーIDのリスト  
const std::vector<UINT> discreteVendorIDs = {  
0x10DE, // NVIDIA  
0x1002, // AMD  
0x8086  // Intel  
};  

// 最適なアダプターを選択する関数  
ComPtr<IDXGIAdapter> D3DMgr::SelectAdapter()  
{  
ComPtr<IDXGIFactory1> factory;  
HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factory.GetAddressOf()));  
if (FAILED(hr))  
{  
	Debug::Log("Failed to create DXGI factory.", Debug::LogLevel::ERR);  
	return nullptr;  
}  

ComPtr<IDXGIAdapter1> adapter;  
ComPtr<IDXGIAdapter> bestAdapter;  
SIZE_T maxDedicatedVideoMemory = 0;  
UINT adapterIndex = 0;  

// NVIDIAアダプターを優先的に選択  
while (factory->EnumAdapters1(adapterIndex, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)  
{  
	DXGI_ADAPTER_DESC1 desc;  
	hr = adapter->GetDesc1(&desc);  
	if (FAILED(hr))  
	{  
		adapterIndex++;  
		adapter.Reset();  
		continue;  
	}  

	// NVIDIAのアダプターをチェック  
	if (desc.VendorId == 0x10DE)  
	{  
		if (desc.DedicatedVideoMemory > maxDedicatedVideoMemory)  
		{  
			bestAdapter = adapter;  
			maxDedicatedVideoMemory = desc.DedicatedVideoMemory;  
		}  
	}  

	adapterIndex++;  
	adapter.Reset();  
}  

// NVIDIAアダプターが見つからない場合、最大メモリのアダプターを選択  
if (bestAdapter) return bestAdapter;  

adapterIndex = 0;  
maxDedicatedVideoMemory = 0;  
while (factory->EnumAdapters1(adapterIndex, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)  
{  
	DXGI_ADAPTER_DESC1 desc;  
	hr = adapter->GetDesc1(&desc);  
	if (FAILED(hr))  
	{  
		adapterIndex++;  
		adapter.Reset();  
		continue;  
	}  

	if (desc.DedicatedVideoMemory > maxDedicatedVideoMemory)  
	{  
		bestAdapter = adapter;  
		maxDedicatedVideoMemory = desc.DedicatedVideoMemory;  
	}  

	adapterIndex++;  
	adapter.Reset();  
}  

return bestAdapter;  
}  

// Direct3Dの初期化  
HRESULT D3DMgr::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)  
{  
HRESULT hr = S_OK;  

ComPtr<IDXGIAdapter> selectedAdapter = SelectAdapter();  

DXGI_SWAP_CHAIN_DESC sd = {};  
sd.BufferCount = 1;  
sd.BufferDesc.Width = SCREEN_WIDTH;  
sd.BufferDesc.Height = SCREEN_HEIGHT;  
sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  
sd.BufferDesc.RefreshRate.Numerator = 60;  
sd.BufferDesc.RefreshRate.Denominator = 1;  
sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  
sd.OutputWindow = hWnd;  
sd.SampleDesc.Count = 1;  
sd.SampleDesc.Quality = 0;  
sd.Windowed = TRUE;  

UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;  
#ifdef _DEBUG  
createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;  
#endif  

// Direct3Dデバイスとスワップチェーンの作成  
hr = D3D11CreateDeviceAndSwapChain(  
	selectedAdapter.Get(),  
	selectedAdapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,  
	nullptr,  
	createDeviceFlags,  
	nullptr,  
	0,  
	D3D11_SDK_VERSION,  
	&sd,  
	&swapChain,  
	&d3dDevice,  
	&featureLevel,  
	&immediateContext  
);  

if (FAILED(hr)) return hr;  

ComPtr<ID3D11Texture2D> pBackBuffer;  
HR(swapChain->ResizeBuffers(1, SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_B8G8R8A8_UNORM, 0));  
HR(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf())));  

HR(d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &renderTargetView));  

ComPtr<ID3D11Texture2D> depthTexture;  
D3D11_TEXTURE2D_DESC td = {};  
td.Width = sd.BufferDesc.Width;  
td.Height = sd.BufferDesc.Height;  
td.MipLevels = 1;  
td.ArraySize = 1;  
td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  
td.SampleDesc = sd.SampleDesc;  
td.Usage = D3D11_USAGE_DEFAULT;  
td.BindFlags = D3D11_BIND_DEPTH_STENCIL;  
td.CPUAccessFlags = 0;  
td.MiscFlags = 0;  

HR(d3dDevice->CreateTexture2D(&td, nullptr, &depthTexture));  

D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};  
dsvd.Format = td.Format;  
dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;  

HR(d3dDevice->CreateDepthStencilView(depthTexture.Get(), &dsvd, &depthStencilView));  

immediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);  

D3D11_VIEWPORT vp = {};  
vp.Width = static_cast<FLOAT>(SCREEN_WIDTH);  
vp.Height = static_cast<FLOAT>(SCREEN_HEIGHT);  
vp.MinDepth = 0.0f;  
vp.MaxDepth = 1.0f;  
vp.TopLeftX = 0;  
vp.TopLeftY = 0;  
immediateContext->RSSetViewports(1, &vp);  

return S_OK;  
}  

// Direct2Dの初期化  
bool D3DMgr::InitDirect2D()  
{  
D2D1_FACTORY_OPTIONS options;  
ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));  

#ifdef _DEBUG  
options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;  
#else  
options.debugLevel = D2D1_DEBUG_LEVEL_NONE;  
#endif  

HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options, reinterpret_cast<void**>(d2dFactory1.GetAddressOf())));  

ComPtr<IDXGIDevice> dxgiDevice;  
HR(d3dDevice.As(&dxgiDevice));  

ComPtr<ID2D1Device> d2dDevice;  
HRESULT hr = d2dFactory1->CreateDevice(dxgiDevice.Get(), &d2dDevice);  

if (hr == E_NOINTERFACE)  
{  
	Debug::Log("The interoperability between Direct2D and Direct3D is limited, and you will not be able to view text information.", Debug::LogLevel::WARN);  
	d2dFactory1 = nullptr;  
}  
else if (hr == S_OK)  
{  
	HR(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext));  

	ComPtr<IDXGISurface> dxgiBackBuffer;  
	HR(swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)));  

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(  
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,  
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));  

	ComPtr<ID2D1Bitmap1> d2dTargetBitmap;  
	HR(d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.Get(), &bitmapProperties, &d2dTargetBitmap));  

	d2dDeviceContext->SetTarget(d2dTargetBitmap.Get());  

	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(dwriteFactory.GetAddressOf())));  
	HR(d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), colorBrush.GetAddressOf()));  
	HR(dwriteFactory->CreateTextFormat(L"MS ゴシック", nullptr, DWRITE_FONT_WEIGHT_NORMAL,  
            DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"ja-jp",  
            textFormat.GetAddressOf()));  
}  

return true;  
}  

// 終了処理  
void D3DMgr::Uninit()  
{  
if (constantBuffer)      constantBuffer->Release();  
if (renderTargetView)    renderTargetView->Release();  
if (depthStencilView)    depthStencilView->Release();  
if (swapChain)           swapChain->Release();  
if (immediateContext)    immediateContext->Release();  
}  

// バックバッファクリア  
void D3DMgr::Clear()  
{  
float clearColor[4] = { 0.192f, 0.302f, 0.475f, 1.0f };  
immediateContext->ClearRenderTargetView(renderTargetView, clearColor);  
immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  
}  

// プレゼント  
void D3DMgr::Present()  
{  
swapChain->Present(0, 0);  
}  

// Direct3Dデバイスを取得  
ID3D11Device* D3DMgr::GetDevice()  
{  
return d3dDevice.Get();  
}  

// Direct3Dデバイスコンテキストを取得  
ID3D11DeviceContext* D3DMgr::GetDeviceContext()  
{  
return immediateContext;  
}  

// Direct2Dデバイスコンテキストを取得  
ID2D1DeviceContext* D3DMgr::GetD2DDeviceContext()  
{  
return d2dDeviceContext.Get();  
}
ID3D11DeviceContext* D3DMgr::GetDeviceContext()
{
	return immediateContext;
}

ID2D1DeviceContext* D3DMgr::GetD2DDeviceContext()
{
	return d2dDeviceContext.Get();
}