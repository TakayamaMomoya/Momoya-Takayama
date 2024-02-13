//*****************************************************
//
// 爆発スポナー処理[explosionspawner.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _EXPLSPAWNER_H_
#define _EXPLSPAWNER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラス定義
//*****************************************************
class CExplSpawner : public CObject
{
public:
	CExplSpawner(void);	// コンストラクタ
	~CExplSpawner();	// デストラクタ

	static CExplSpawner *Create(D3DXVECTOR3 pos, float fRadius, int nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); }	// 取得処理

private:
	D3DXVECTOR3 m_pos;	// 位置
	int m_nLife;	// 寿命
	float m_fRadius;	// 半径
	int m_nCntSpawn;	// スポーンカウンター
	int m_nTimerSpawn;	// スポーンタイマー
};

#endif