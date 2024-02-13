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
	enum MODE
	{
		MODE_CREATE_BLOCK = 0,	// ブロック生成
		MODE_DELTE_BLOCK,	// ブロック削除
		MODE_CREATE_GIMICK,	// ギミック生成
		MODE_DELTE_GIMICK,	// ギミック削除
		MODE_MAX
	};

	int CheckDelete(void);

	MODE m_mode;	// モード
	CObjectX *m_pObjectCursor;	// カーソルのオブジェクトXへのポインタ
	int m_nIdxObject;
	CBlock::TYPE m_type;
	static CEdit *m_pEdit;	// 自身のポインタ
};