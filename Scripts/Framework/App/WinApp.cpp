#include <sstream>
#include "WinApp.h"
#include "GameTime.h"

// WinAppクラスのコンストラクタ
WinApp::WinApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
	: hAppInst(hInstance),
	mainWndCaption(windowName),
	clientWidth(initWidth),
	clientHeight(initHeight) {}

// メインウィンドウの初期化
bool WinApp::InitMainWindow()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;  // ウィンドウプロシージャ設定
	wc.lpszClassName = mainWndCaption.c_str();
	wc.hInstance = hAppInst;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	wc.lpszClassName = L"D3DWndClassName";

	// ウィンドウクラスの登録
	if (!RegisterClass(&wc))
	{
		MessageBox(nullptr, L"RegisterClass Failed.", nullptr, 0);
		return false;
	}

	// ウィンドウサイズをクライアント領域に合わせて調整
	RECT R = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX), FALSE);

	// ウィンドウの作成
	hMainWnd = CreateWindow(
		L"D3DWndClassName",
		mainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT,
		R.right - R.left, R.bottom - R.top,
		nullptr, nullptr,
		hAppInst, nullptr
	);

	// ウィンドウに自分自身のポインタを渡す（WndProcで使う）
	SetWindowLongPtr(hMainWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	if (!hMainWnd)
	{
		MessageBox(nullptr, L"CreateWindow Failed.", nullptr, 0);
		return false;
	}

	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);

	return true;
}

// メインウィンドウのハンドル取得
HWND WinApp::GetMainWnd() const
{
	return hMainWnd;
}

// アプリケーションインスタンスの取得
HINSTANCE WinApp::GetAppInst() const
{
	return hAppInst;
}

// アスペクト比（幅÷高さ）を取得
float WinApp::GetAspectRatio() const
{
	return static_cast<float>(clientWidth) / clientHeight;
}

// アプリケーションのメインループ
int WinApp::Run()
{
	MSG msg = { nullptr };
	Time::Reset();  // 時間システム初期化
	float accumulator = 0.0f;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Time::Tick();  // 経過時間を計測
			float dt = Time::GetDeltaTime();
			accumulator += dt;

			CalculateFrameStats();  // FPSなどの計算と表示

			StartScene();

			// 固定更新：accumulatorが1フレーム分たまったら処理
			while (accumulator >= Time::GetFixedDeltaTime())
			{
				UpdatePhysics();  // 物理処理（一定間隔で実行）
				accumulator -= Time::GetFixedDeltaTime();
			}

			UpdateScene();  // 通常更新
			DrawScene();    // 描画
		}
	}
	return (int)msg.wParam;
}

// アプリ全体の初期化（必要ならここをオーバーライド）
bool WinApp::Init()
{
	if (!InitMainWindow())
		return false;

	return true;
}

// ウィンドウプロシージャ
LRESULT CALLBACK WinApp::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ウィンドウに保存したクラスポインタを取得
	WinApp* pThis = reinterpret_cast<WinApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (pThis)
	{
		switch (uMsg)
		{
			case WM_KEYDOWN:
				if (wParam == VK_ESCAPE)
					SendMessage(hWnd, WM_CLOSE, 0, 0);  // ESCで終了確認ダイアログを表示
				break;

			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
				// キー入力処理（コメントアウトされている）
				break;

			case WM_CLOSE:
				// ウィンドウを閉じる前に確認ダイアログを出す
				if (MessageBox(hWnd, L"Are you sure you want to exit?", L"Sure", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
					DestroyWindow(hWnd);
				else
					return 0;  // キャンセルされたら何もしない
				break;

			case WM_DESTROY:
				PostQuitMessage(0);  // 終了メッセージを送信
				break;

			default:
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// フレームレートとフレーム時間の計算とウィンドウタイトルへの表示
void WinApp::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// 1秒ごとにFPSとフレーム時間を更新
	if ((Time::GetTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(hMainWnd, outs.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
