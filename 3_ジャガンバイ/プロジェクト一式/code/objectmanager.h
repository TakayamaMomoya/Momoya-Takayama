//*****************************************************
//
// オブジェクト管理[objectmanager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;
class CTutorialPlayer;
class CMeshField;
class CScore;
class CTimer;

//*****************************************************
// クラスの定義
//*****************************************************
class CObjectManager
{
public:
	CObjectManager();	// コンストラクタ
	~CObjectManager();	// デストラクタ

	CPlayer *GetPlayer(void) { return m_pPlayer; }
	CMeshField *GetMeshField(void) { return m_pMeshField; }
	CScore *GetScore(void) { return m_pScore; }
	CTimer *GetTimer(void) { return m_pTimer; }
	
	void BindPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }
	void BindTutorialPlayer(CTutorialPlayer *pPlayer) { m_pTutorialPlayer = pPlayer; }
	void BindMeshField(CMeshField *pMeshField) { m_pMeshField = pMeshField; }

	void ReleasePlayer(void);
	void ReleaseMeshField(void) { m_pMeshField = nullptr; }
	void ReleaseScore(void);
	void ReleaseTimer(void);
	
private:
	CPlayer *m_pPlayer;	// プレイヤーのクラス
	CTutorialPlayer *m_pTutorialPlayer;	// チュートリアルプレイヤーのクラス
	CMeshField *m_pMeshField;	// メッシュフィールドへのポインタ
	CScore *m_pScore;	// スコアのポインタ
	CTimer *m_pTimer;	// タイマーのポインタ
};

#endif