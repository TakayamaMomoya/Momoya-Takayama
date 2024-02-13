//*****************************************************
//
// ライフ表示の処理[life.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CLife : public CObject
{
public:

	enum LIFE_STATE
	{
		LIFE_STATE_NORMAL = 0,	// 通常
		LIFE_STATE_DEC,			// 減っている
		LIFE_STATE_DEC_VERY,	// とても減っている
		LIFE_STATE_DEC_PINCH,	// 死にかけ
		LIFE_STATE_MAX
	};

	CLife(int nPriority = 7);	// コンストラクタ
	~CLife();	// デストラクタ

	static CLife *Create(int nIdx);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetLife(void);
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(float width, float height);
	void SetCol(D3DXCOLOR colLife, D3DXCOLOR colLifeFrame);
	void SetTexture(const char* pFileName);

	D3DXCOLOR GetColLife(void) { return m_info.colLife; }
	D3DXCOLOR GetColLifeFrame(void) { return m_info.colLifeFrame; }
	void SetColorChange(bool bColorChange) { m_info.bColorChange = bColorChange; }
	bool GetColorChange(void) { return m_info.bColorChange; }

private:

	struct Sinfo
	{
		int nIdxPlayer;				// 参照するプレイヤーの番号
		float fLife;				// 体力
		LIFE_STATE state;			// 体力状態

		D3DXVECTOR3 posLife;		// 位置
		float fWidth;				// 横幅
		float fHeight;				// 縦幅
		D3DXCOLOR colLife;			// 色
		D3DXCOLOR colLifeFrame;		// 色枠
		bool bColorChange;			// 色変更フラグ

		int nIdxTexture;					// テクスチャ番号
		static LPDIRECT3DTEXTURE9 pTexture;	// テクスチャへのポインタ
	};

	CUI* m_pUILife;			// ライフのUI
	CUI* m_pUILifeFrame;	// ライフの枠UI
	Sinfo m_info;			// 自身の情報
};

#endif
