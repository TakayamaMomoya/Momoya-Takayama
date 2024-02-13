//*****************************************************
//
// メイン処理[main.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "object.h"
#include <time.h>

//*****************************************************
// マクロ定義
//*****************************************************
#define CLASS_NAME				"WindowClass"					// ウィンドウクラスの名前
#define WINDOW_NAME				"RAYLEIGH"				// ウィンドウの名前(キャプション)

//*****************************************************
// プロトタイプ宣言
//*****************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************
// グローバル変数宣言
//*****************************************************
int g_nCountFPS;	// FPSカウンター

//=====================================================
// メイン関数
//=====================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//乱数シード値の設定
	srand((unsigned int)time(0));

	// マネージャーの宣言
	CManager *pManager = nullptr;
	
	// 変数宣言
	DWORD dwCurrentTime;				//現在時刻
	DWORD dwExecLastTime;				//最後に処理した時刻
	DWORD dwFrameCount;					//フレームカウント
	DWORD dwFPSLastTime;				//最後にFPSを計測した時間

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//フォントポインタ・FPSカウンタの初期化
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr,
		CLASS_NAME,
		LoadIcon(nullptr,IDI_APPLICATION),
	};

	HWND hWnd;
	MSG msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域をウィンドウサイズに合わせる
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウを生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	if (pManager == nullptr)
	{
		// インスタンス生成
		pManager = new CManager;

		// マネージャーの初期化
		pManager->Init(hInstance, hWnd, TRUE);
	}

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while(true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{// Windowsの処理
			if (msg.message == WM_QUIT)
			{// WM＿QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DeirectXの処理
			
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPS計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				//FPSを測定した時間を保存する
				dwFPSLastTime = dwCurrentTime;

				//フレームカウントのクリア
				dwFrameCount = 0;
			}


			//現在時刻の取得
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				//処理開始の時刻を計算
				dwExecLastTime = dwCurrentTime;

				if (pManager != nullptr)
				{
					// 更新処理
					pManager->Update();

					// 描画処理
					pManager->Draw();
				}

				//FPSカウンタ経過
				dwFrameCount++;
			}

			
		}
	}

	if (pManager != nullptr)
	{
		// 終了処理
		pManager->Uninit();

		// インスタンスの破棄
		delete pManager;
		pManager = nullptr;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//=====================================================
// ウィンドウプロシージャ
//=====================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
		// ウィンドウ破棄メッセージを受け取った場合
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:

		// 確認メッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{// イエスの場合
			// ウィンドウ破棄
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}

	case WM_KEYDOWN:
		switch (wParam)
		{
			// [ESC]キーが押された
		case VK_ESCAPE:

			// 確認メッセージ
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{// イエスの場合
				// ウィンドウ破棄
				DestroyWindow(hWnd);
			}
			break;
		}

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=====================================================
// FPSの取得
//=====================================================
int GetFPS(void)
{
	return g_nCountFPS;
}