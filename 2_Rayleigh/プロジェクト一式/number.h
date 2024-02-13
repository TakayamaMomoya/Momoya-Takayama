//*****************************************************
//
// 数字の処理[number.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_PLACE	(10)	// 最大桁数

class CNumber : public CObject
{
public:
	CNumber(int nPriority = 5);	// コンストラクタ
	~CNumber();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {}
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); }	// 取得処理
	void SetValue(int nValue, int nNumPlace);
	static CNumber *Create(int nNumPlace, int nValue);
	void SetSizeAll(float width,float height);
	float GetWidth(void) { return 0.0f; }	// サイズ取得
	float GetHeight(void) { return 0.0f; }	// サイズ取得
	void SetColor(D3DXCOLOR col);

private:
	CUI *m_apObject[MAX_PLACE];	// ２Ⅾオブジェクトのポインタ
	D3DXVECTOR3 m_pos;	// 位置
	int m_nNumPlace;	// 桁数
	int m_nValue;	// 値
	int m_nNumAll;	// 総数
	int m_nIdxTexture;	// テクスチャ番号
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
};

#endif