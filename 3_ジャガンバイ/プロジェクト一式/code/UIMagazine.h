//*****************************************************
//
// 残弾数UIの処理[UIMagazine.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _UI_MAGAZINE_H_
#define _UI_MAGAZINE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************

class CUI;
class CNumber;

//*****************************************************
// クラスの定義
//*****************************************************
class CUIMagazine : public CObject
{
public:

	CUIMagazine(int nPriority = 7);	// コンストラクタ
	~CUIMagazine();	// デストラクタ

	static CUIMagazine* Create(int nIdx);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 posUIMagazine, D3DXVECTOR3 posUIMagazineFrame, D3DXVECTOR3 posNum);
	void SetSize(float width, float height, float widthNum, float heightNum);
	void SetCol(D3DXCOLOR colMagazine, D3DXCOLOR colMagazineFrame, D3DXCOLOR colNum);
	void SetTexture(const char* pFileName);
	void SetNumMagazine(void);
	void SetVtxGage(void);

	D3DXCOLOR GetColMagazine(void) { return m_info.colMagazine; }
	D3DXCOLOR GetColMagazineFrame(void) { return m_info.colMagazineFrame; }
	D3DXCOLOR GetColNum(void) { return m_info.colNum; }

	bool GetColorChange(void) { return m_info.bColorChange; }
	void SetColorChange(bool bColChange) { m_info.bColorChange = bColChange; }

private:

	struct Sinfo
	{
		int nIdxPlayer;		// 参照するプレイヤーの番号
		int nBullet;		// 残弾数

		D3DXVECTOR3 posUIMagazine;			// 位置装弾数
		D3DXVECTOR3 posUIMagazineFrame;		// 位置装弾数枠
		D3DXVECTOR3 posNum;					// 位置数字

		float fWidth;						// 横幅
		float fHeight;						// 縦幅
		float fWidthNum;					// 横幅数字
		float fHeightNum;					// 縦幅数字

		D3DXCOLOR colMagazine;				// 色装弾数
		D3DXCOLOR colMagazineFrame;			// 色装弾数枠
		D3DXCOLOR colNum;					// 色数字
		bool bColorChange;					// 色変更フラグ

		float fWidthSub;					// 減算する横幅
		float fHeightSub;					// 減算する縦幅

		int nIdxTexture;					// テクスチャ番号
		static LPDIRECT3DTEXTURE9 pTexture;	// テクスチャへのポインタ
	};

	CUI* m_pUIMagazineFrame;	// マガジンUI枠のポインタ
	CUI* m_pUIMagazine;			// マガジンUIのポインタ
	CNumber* m_pNumDig;			// 装弾数（数字）のポインタ
	Sinfo m_info;				// 自身の情報
};

#endif
