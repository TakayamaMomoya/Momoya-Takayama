//*****************************************************
//
// レーダーの処理[rader.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "shockwave.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "debugproc.h"
#include "universal.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MESHCYLINDER_TEX_FILE		"data\\TEXTURE\\BG\\sky000.png"				// テクスチャファイル名
#define MESH_LENGTH					(230.0f)									// メッシュの一辺の長さ
#define MESH_U						(256)										// 横のブロック数
#define MESH_V						(1)	// 縦のブロック数
#define SPLIT_TEX_U					(3)	// 横のテクスチャ分割数
#define SPLIT_TEX_V					(1)	// 縦のテクスチャ分割数
#define MESH_HEIGHT	(10.0f)	// メッシュの高さ
#define ADD_HEIGHT	(100.0f)	// 目標からの高さ
#define WAVE_ANGLE	(D3DX_PI * 0.1f)	// 波打つ範囲の角度
#define WAVE_HEIGHT	(50.0f)	// 波打つ高さ
#define WAVE_LENGTH	(100.0f)	// 波打つ範囲の距離
#define CHANGE_COl	(0.15f)	// 色の変わる割合
#define RAND_HEIGHT	(5.0f)	// 振れる幅
#define WAVE_SIZE	(0.04f)	// 波の大きさ
#define SHAKE_SIZE	(0.005f)	// 揺れの大きさ

//=====================================================
// コンストラクタ
//=====================================================
CShockWave::CShockWave(void)
{

}

//=====================================================
// デストラクタ
//=====================================================
CShockWave::~CShockWave()
{

}

//=====================================================
// 生成処理
//=====================================================
CShockWave *CShockWave::Create(void)
{
	CShockWave *pShockWave = nullptr;

	if (pShockWave == nullptr)
	{
		pShockWave = new CShockWave;

		if (pShockWave != nullptr)
		{
			pShockWave->Init();
		}
	}

	return pShockWave;
}

//=====================================================
//初期化処理
//=====================================================
HRESULT CShockWave::Init(void)
{
	// 継承クラスの初期化
	CMeshCylinder::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CShockWave::Uninit(void)
{
	// 継承クラスの終了
	CMeshCylinder::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CShockWave::Update(void)
{
	// 継承クラスの更新
	CMeshCylinder::Update();

	// 波形に動く処理
	Wave();
}

//=====================================================
// 波形に動く処理
//=====================================================
void CShockWave::Wave(void)
{
	// 頂点バッファ取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CMeshCylinder::GetVtxBuff();

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MESH_U + 1; nCnt++)
	{// 全頂点をチェック
		D3DXVECTOR3 pos;

		pVtx[nCnt].tex.y += 0.03f;
		pVtx[nCnt + MESH_U + 1].tex.y += 0.03f;

		//pos = pVtx[nCnt].pos;
		//D3DXVec3Normalize(&pos,&pos);
		//pVtx[nCnt].pos.x += pos.x;
		//pVtx[nCnt].pos.z += pos.z;

		//pos = pVtx[nCnt + MESH_U + 1].pos;
		//D3DXVec3Normalize(&pos, &pos);
		//pVtx[nCnt + MESH_U + 1].pos.x += pos.x;
		//pVtx[nCnt + MESH_U + 1].pos.z += pos.z;
	}

	// 後ろの二点をくっつける
	pVtx[MESH_U].pos = pVtx[0].pos;
	pVtx[MESH_U * 2 + 1].pos = pVtx[MESH_U + 1].pos;

	// 頂点バッファをアンロック
	pVtxBuff->Unlock();
}

//=====================================================
// 描画処理
//=====================================================
void CShockWave::Draw(void)
{
	CMeshCylinder::Draw();
}