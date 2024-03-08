//*****************************************************
//
// 位置制限の処理[limit.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _LIMIT_H_
#define _LIMIT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CLimit : public CObject
{
public:
	CLimit(int nPriority = 3);	// コンストラクタ
	~CLimit();	// デストラクタ

	static CLimit *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) { return m_info.rot; }

private:
	struct SInfo
	{
		CObject3D *pGuide;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	void CreateGuide(void);
	void DeleteGuide(void);
	D3DXVECTOR3 CheckLimit(CObject *pObj,float *pDiff = nullptr);
	void SetColGuide(float fDiff);

	SInfo m_info;
};

#endif