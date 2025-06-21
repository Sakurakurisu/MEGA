#include "GameApp.h"
#include "D3DMgr.h"
#include "SceneMgr.h"
#include "RenderStates.h"
#include "ResourceMgr.h"
#include "Effects.h"
#include "DOTween.h"
#include "TimerMgr.h"
#include "ShaderMgr.h"
#include "RenderStateMgr.h"

// GameAppコンストラクタ：WinAppを継承
GameApp::GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
	: WinApp(hInstance, windowName, initWidth, initHeight) {}

// 初期化処理
bool GameApp::Init()
{
	// メインウィンドウの初期化
	if (!WinApp::InitMainWindow())
		return false;

	// D3Dマネージャの初期化（Direct3Dのデバイスなどをセットアップ）
	D3DMgr::instance.Init(hAppInst, hMainWnd, true);

	// Direct2Dの初期化（テキスト描画やUI用）
	if (!D3DMgr::instance.InitDirect2D())
		return false;

	// リソースマネージャによるリソースの読み込み（テクスチャ、モデルなど）
	ResourceMgr::instance.Load();

	// 各種レンダーステートの初期化（ブレンド、ラスタライザなど）
	RenderStates::InitAll(D3DMgr::instance.GetDevice());

	// シェーダの読み込みとデバッグ名の設定
	ShaderMgr::instance.LoadAllShaders(D3DMgr::instance.GetDevice());
	ShaderMgr::instance.SetAllDebugNames();

	// エフェクトの初期化（シェーダロジックの一部）
	if (!BasicEffect::instance.InitAll(D3DMgr::instance.GetDevice()))
		return false;

	// 複数のレンダーステートプリセットを読み込み
	RenderStateMgr::instance.LoadRenderStates(D3DMgr::instance.GetDevice());

	// シーンマネージャの初期化
	SceneMgr::instance.Init();

	return true;
}

// 最初のシーンを開始する
void GameApp::StartScene()
{
	SceneMgr::instance.Start();
}

// 物理更新（固定タイムステップの処理）
void GameApp::UpdatePhysics()
{
	SceneMgr::instance.FixedUpdate();
}

// 通常の更新処理
void GameApp::UpdateScene()
{
	TimerMgr::instance.Update();              // 時間の更新
	SceneMgr::instance.Update();             // 通常のシーン更新
	DOTween::UpdateAll();                    // アニメーション更新
	SceneMgr::instance.LateUpdate();         // 遅延更新（Transformの適用など）
	SceneMgr::instance.HandleDestroy();      // 削除予定オブジェクトの処理
}

// 描画処理
void GameApp::DrawScene()
{
	D3DMgr::instance.Clear();                // バッファのクリア

	SceneMgr::instance.Draw();               // シーンの描画

	SceneMgr::instance.PerformSceneChange(); // シーン切り替え処理（必要なときのみ）

	D3DMgr::instance.Present();              // 描画結果を画面に表示
}
