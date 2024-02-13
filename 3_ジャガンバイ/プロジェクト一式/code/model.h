//*****************************************************
//
// Xファイルモデルの処理[model.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MODEL_H_
#define _MODEL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_MODEL	(256)	// モデルの数

//*****************************************************
// クラスの定義
//*****************************************************
class CModel
{
public:
	typedef struct
	{// モデルの構造体
		int *pIdxTexture;	// テクスチャ番号へのポインタ
		LPD3DXMESH pMesh;		//メッシュへのポインタ
		LPD3DXBUFFER pBuffMat;	//マテリアルへのバッファ
		DWORD dwNumMat;	//マテリアルの数
	}Model;

	CModel();	// コンストラクタ
	~CModel();	// デストラクタ
	
	static int Load(char *pPath);
	static void Unload(void);
	static void Init(int nIdx);
	static Model *GetModel(int nIdx) { return m_apModel[nIdx]; }
	static int GetNumAll(void) { return m_nNumAll; }

private:
	static Model *m_apModel[NUM_MODEL];	// モデルへのポインタ
	static char m_aPath[NUM_MODEL][256];	// モデルのファイル名
	static int m_nNumAll;	// 総数
};

#endif