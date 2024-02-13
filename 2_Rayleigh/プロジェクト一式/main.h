//*****************************************************
//
// メイン処理[main.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************
// インクルード
//*****************************************************
#include <windows.h>
#include"d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)						//ビルド時の警告対処用マクロ
#include "dinput.h"
#include "xaudio2.h"

//*****************************************************
// ライブラリ
//*****************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

//*****************************************************
//マクロ定義
//*****************************************************
#define SCREEN_WIDTH			(1280)									// ウィンドウの幅
#define SCREEN_HEIGHT			(720)									// ウィンドウの高さ
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// 頂点フォーマット設定
#define FVF_VERTEX_3D			(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//頂点フォーマット設定

//*****************************************************
// 構造体定義
//*****************************************************
typedef struct
{// 2Dの頂点情報
	D3DXVECTOR3 pos;			//頂点座標
	float rhw;					//座標変換用係数（１．０ｆで固定）
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;			//テクスチャ座標
}VERTEX_2D;

typedef struct
{// ３Dの頂点情報
	D3DXVECTOR3 pos;			//頂点座標
	D3DXVECTOR3 nor;			//法線ベクトル
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;			//テクスチャ座標
}VERTEX_3D;

//*****************************************************
// プロトタイプ宣言
//*****************************************************
int GetFPS(void);

#endif