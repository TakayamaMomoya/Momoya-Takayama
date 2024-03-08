//*****************************************************
//
// 2D文字列の処理[string2D.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "string2D.h"
#include "UI.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* TEXTURE_PATH = "data\\TEXTURE\\UI\\char.png";	// 文字表のパス
}

//=====================================================
// コンストラクタ
//=====================================================
CString2D::CString2D(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
//	デストラクタ
//=====================================================
CString2D::~CString2D()
{

}

//=====================================================
// 生成処理
//=====================================================
CString2D *CString2D::Create(void)
{
	CString2D *pString2D = nullptr;

	pString2D = new CString2D;

	if (pString2D != nullptr)
	{
		// 初期化処理
		pString2D->Init();
	}

	return pString2D;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CString2D::Init(void)
{
	m_info.pos = SCRN_MID;
	m_info.fSizeChar = 25.0f;

	std::string string = "test";

	SetString(string);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CString2D::Uninit(void)
{
	DeleteChar();

	Release();
}

void CString2D::DeleteChar(void)
{// 文字の破棄
	if (!m_listChar.empty())
	{
		for (auto it = m_listChar.begin(); it != m_listChar.end();)
		{
			auto *pChar = *it;

			pChar->Uninit();

			it = m_listChar.erase(it);
		}
	}
}

//=====================================================
// 更新処理
//=====================================================
void CString2D::Update(void)
{

}

//=====================================================
// 位置設定処理
//=====================================================
void CString2D::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;
}

//=====================================================
// 文字列の設定
//=====================================================
void CString2D::SetString(std::string string)
{
	DeleteChar();

	m_string = string;
	m_info.nLength = m_string.length();

	for (int i = 0; i < m_info.nLength; i++)
	{
		CUI *pChar = CUI::Create();

		m_listChar.push_back(pChar);
	}
}

//=====================================================
// 文字サイズの設定
//=====================================================
void CString2D::SetSizeChar(float fSize)
{
	m_info.fSizeChar = fSize;
}