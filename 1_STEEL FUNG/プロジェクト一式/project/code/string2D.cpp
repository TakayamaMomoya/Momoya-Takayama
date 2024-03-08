//*****************************************************
//
// 2D������̏���[string2D.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "string2D.h"
#include "UI.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* TEXTURE_PATH = "data\\TEXTURE\\UI\\char.png";	// �����\�̃p�X
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CString2D::CString2D(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
//	�f�X�g���N�^
//=====================================================
CString2D::~CString2D()
{

}

//=====================================================
// ��������
//=====================================================
CString2D *CString2D::Create(void)
{
	CString2D *pString2D = nullptr;

	pString2D = new CString2D;

	if (pString2D != nullptr)
	{
		// ����������
		pString2D->Init();
	}

	return pString2D;
}

//=====================================================
// ����������
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
// �I������
//=====================================================
void CString2D::Uninit(void)
{
	DeleteChar();

	Release();
}

void CString2D::DeleteChar(void)
{// �����̔j��
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
// �X�V����
//=====================================================
void CString2D::Update(void)
{

}

//=====================================================
// �ʒu�ݒ菈��
//=====================================================
void CString2D::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;
}

//=====================================================
// ������̐ݒ�
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
// �����T�C�Y�̐ݒ�
//=====================================================
void CString2D::SetSizeChar(float fSize)
{
	m_info.fSizeChar = fSize;
}