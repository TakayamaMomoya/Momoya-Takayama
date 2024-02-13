//*****************************************************
//
// 熱量表示の処理[heat.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _HEAT_H_
#define _HEAT_H_

#include "Object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CHeat : public CObject
{
public:
	CHeat(int nPriority = 6);	// コンストラクタ
	~CHeat();	// デストラクタ

	static CHeat *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetParam(float fParam) { m_fParam = fParam; }
	void BindTextureFrame(const char* pPath);

private:
	float m_fParam;	// パラメータ値
	D3DXVECTOR3 m_pos;	// 位置
	CUI *m_pFrame;	// フレーム
	CUI *m_pGauge;	// ゲージ
};

#endif
