//*****************************************************
//
// ビームブレードの処理[beamBlade.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BEAMBLADE_H_
#define _BEAMBLADE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMeshCylinder;
class CObject3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CBeamBlade : public CObject
{
public:
	CBeamBlade(int nPriority = 3);	// コンストラクタ
	~CBeamBlade();	// デストラクタ

	static CBeamBlade *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) { return m_info.rot; }
	void SetRadius(float fRadius);
	void SetHeight(float fHeight);
	void SetColor(D3DXCOLOR col);
	float GetRadius(void) { return m_info.fRadius; }
	float GetHeight(void) { return m_info.fHeight; }

private:
	struct SInfo
	{
		CMeshCylinder *pCylinder;
		CObject3D *pBlade;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXCOLOR col;
		float fRadius;
		float fHeight;
	};

	SInfo m_info;
};

#endif