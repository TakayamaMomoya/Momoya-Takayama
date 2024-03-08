//*****************************************************
//
// �x���\���̏���[caution.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "caution.h"
#include "UI.h"
#include "texture.h"
#include "debugproc.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SIZE_ICON = 30.0f;	// �A�C�R���̃T�C�Y
const float LIMIT_DIST = 200.0f;	// ��ʒ��S���痣��鐧������
const float INITIAL_LIFE = 5.0f;	// ��������
}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CCaution::CCaution(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CCaution::~CCaution()
{

}

//=====================================================
// ��������
//=====================================================
CCaution *CCaution::Create(D3DXVECTOR3 pos)
{
	CCaution *pCaution = nullptr;

	if (pCaution == nullptr)
	{
		// �C���X�^���X����
		pCaution = new CCaution;

		if (pCaution != nullptr)
		{
			pCaution->m_info.pos = pos;

			// ����������
			pCaution->Init();
		}
	}

	return pCaution;
}

//=====================================================
// ������
//=====================================================
HRESULT CCaution::Init(void)
{
	m_info.fLife = INITIAL_LIFE;

	// �A�C�R���̐���
	m_info.pIcon = CUI::Create();

	if (m_info.pIcon != nullptr)
	{
		m_info.pIcon->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_info.pIcon->SetSize(SIZE_ICON * 3, SIZE_ICON * 3);
		
		int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\caution00.png");
		m_info.pIcon->SetIdxTexture(nIdx);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CCaution::Uninit(void)
{
	if (m_info.pIcon != nullptr)
	{
		m_info.pIcon->Uninit();
		m_info.pIcon = nullptr;
	}

	if (m_info.pArrow != nullptr)
	{
		m_info.pArrow->Uninit();
		m_info.pArrow = nullptr;
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CCaution::Update(void)
{
	// �A�C�R���̍X�V
	UpdateIcon();

	float fDeltaTime = CManager::GetDeltaTime();

	m_info.fLife -= fDeltaTime;

	if (m_info.fLife < 0.0f)
	{
		Uninit();

		return;
	}
}

//=====================================================
// �A�C�R���̍X�V
//=====================================================
void CCaution::UpdateIcon(void)
{
	if (m_info.pIcon == nullptr)
		return;

	// �傫���̕ύX
	float fHeight = m_info.pIcon->GetHeight();
	float fWidth = m_info.pIcon->GetWidth();

	fWidth += (SIZE_ICON - fWidth) * 0.1f;
	fHeight += (SIZE_ICON - fHeight) * 0.1f;

	m_info.pIcon->SetSize(fWidth, fHeight);

	D3DXMATRIX mtx;
	D3DXVECTOR3 posScreen;

	// ��ʓ����W�擾
	bool bInScreen = universal::IsInScreen(m_info.pos, mtx, &posScreen);

	CDebugProc::GetInstance()->Print("\n��ʍ��W[%f,%f,%f]", posScreen.x, posScreen.y, posScreen.z);

	D3DXVECTOR3 vecDiff = SCRN_MID - posScreen;
	float fDiff = D3DXVec3Length(&vecDiff);

	if (!bInScreen || fDiff > LIMIT_DIST || posScreen.z >= 1)
	{
		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= LIMIT_DIST;

		if (posScreen.z >= 1)
		{
			posScreen = SCRN_MID + vecDiff;
		}
		else
		{
			posScreen = SCRN_MID - vecDiff;
		}
	}

	// �ʒu�̐ݒ�
	m_info.pIcon->SetPosition(posScreen);
	m_info.pIcon->SetVtx();
}

//=====================================================
// �`��
//=====================================================
void CCaution::Draw(void)
{

}