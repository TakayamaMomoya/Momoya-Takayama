//*****************************************************
//
// ライトの処理[light.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_LIGHT				(3)			//ライトの最大数

//*****************************************************
// クラス定義
//*****************************************************
class CLight
{
public:
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetColDest(int nIdx, D3DXCOLOR col);
	void ResetColDest(void);

private:
	struct SInfo
	{
		D3DXCOLOR col;	// 現在の色
		D3DXCOLOR colDest;	// 目標の色
	};

	D3DLIGHT9 m_aLight[MAX_LIGHT];
	SInfo m_aInfo[MAX_LIGHT];
};

#endif