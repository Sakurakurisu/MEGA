#pragma once

#include "main.h"
#include "BaseManager.h"
#include <wrl\client.h>
#include <d2d1_1.h>
#include <dwrite.h>

using Microsoft::WRL::ComPtr;

class D3DMgr : public BaseManager<D3DMgr>
{
friend class BaseManager<D3DMgr>;
public:
~D3DMgr();

// 初期化処理を行います
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

// Direct2Dの初期化を行います
bool InitDirect2D();

// 解放処理を行います
void Uninit();

// 描画をクリアします
void Clear();

// 描画内容を画面に表示します
void Present();

// Direct3Dデバイスを取得します
ID3D11Device* GetDevice();

// Direct3Dデバイスコンテキストを取得します
ID3D11DeviceContext* GetDeviceContext();

// Direct2Dデバイスコンテキストを取得します
ID2D1DeviceContext* GetD2DDeviceContext();

ComPtr<ID2D1SolidColorBrush> colorBrush;
ComPtr<IDWriteFont> font;
ComPtr<IDWriteTextFormat> textFormat;

ComPtr<ID2D1Factory1> d2dFactory1;
ComPtr<IDWriteFactory> dwriteFactory;

private:
D3DMgr() = default;
D3DMgr(const D3DMgr&) = delete;
D3DMgr& operator=(const D3DMgr&) = delete;

ComPtr<IDXGIAdapter> SelectAdapter();

D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

ComPtr<ID3D11Device> d3dDevice;

ID3D11DeviceContext* immediateContext = nullptr;
IDXGISwapChain* swapChain = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;
ID3D11DepthStencilView* depthStencilView = nullptr;

ID3D11Buffer* constantBuffer = nullptr;

ComPtr<ID2D1DeviceContext> d2dDeviceContext;
};
