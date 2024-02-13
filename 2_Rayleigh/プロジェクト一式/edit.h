//*****************************************************
//
// エディターの処理[edit.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "block.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObjectX;

//*****************************************************
// クラスの定義
//*****************************************************
class CEdit : public CObject
{
public:
	CEdit();	// コンストラクタ
	~CEdit();	// デストラクタ

	static CEdit *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEdit *GetInstatnce(void) { return m_pEdit; }

private:
	CObjectX *m_pObjectCursor;	// カーソルのオブジェクトXへのポインタ
	int m_nIdxObject;
	CBlock::TYPE m_type;
	static CEdit *m_pEdit;	// 自身のポインタ
};