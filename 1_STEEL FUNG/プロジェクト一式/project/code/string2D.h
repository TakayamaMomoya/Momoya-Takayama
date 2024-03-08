//*****************************************************
//
// 2D文字列の処理[string2D.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _STRING2D_H_
#define _STRING2D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include <iostream>
#include <list>

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラス定義
//*****************************************************
class CString2D : public CObject
{
public:
	CString2D(int nPriority = 0);	// コンストラクタ
	~CString2D();	// デストラクタ

	static CString2D *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {}
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	void SetString(std::string string);
	void SetSizeChar(float fSize);

private:
	struct SInfo
	{
		D3DXVECTOR3 pos;	// 全体の位置
		float fSizeChar;	// 文字のサイズ
		int nLength;	// 文字列の長さ
	};

	void DeleteChar(void);

	SInfo m_info;
	std::list<CUI*> m_listChar;	// 文字のリスト
	std::string m_string;	// 文字列
};

#endif