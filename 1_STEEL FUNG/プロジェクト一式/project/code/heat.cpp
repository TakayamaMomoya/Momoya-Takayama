//*****************************************************
//
// �M�ʕ\���̏���[heat.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "heat.h"
#include "texture.h"
#include "player.h"
#include "UI.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR2 SIZE_FRAME = { 100.0f,50.0f };	// �t���[���̃T�C�Y
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CHeat::CHeat(int nPriority) : CObject(nPriority)
{
	m_fParam = 0.0f;
	m_pos = {};
	m_pFrame = nullptr;
	m_pGauge = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CHeat::~CHeat()
{

}

//=====================================================
// ��������
//=====================================================
CHeat *CHeat::Create(void)
{
	CHeat *pHeat = nullptr;

	if (pHeat == nullptr)
	{
		pHeat = new CHeat;

		if (pHeat != nullptr)
		{
			pHeat->Init();
		}
	}

	return pHeat;
}

//=====================================================
// ����������
//=====================================================
HRESULT CHeat::Init(void)
{
	if (m_pFrame == nullptr)
	{// �t���[���̐���
		m_pFrame = CUI::Create();

		if (m_pFrame != nullptr)
		{
			m_pFrame->SetSize(SIZE_FRAME.x, SIZE_FRAME.y);
			m_pFrame->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\frame00.png");
			m_pFrame->SetIdxTexture(nIdx);
			m_pFrame->SetVtx();
		}
	}

	if (m_pGauge == nullptr)
	{// �Q�[�W�̐���
		m_pGauge = CUI::Create();

		if (m_pGauge != nullptr)
		{
			//m_pGauge->EnableAdd(true);
			m_pGauge->SetCol(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.5f));
			m_pGauge->SetSize(SIZE_FRAME.x, SIZE_FRAME.y * m_fParam);
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\gauge00.png");
			m_pGauge->SetIdxTexture(nIdx);
			m_pGauge->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CHeat::Uninit(void)
{
	if (m_pGauge != nullptr)
	{// �Q�[�W�̔j��
		m_pGauge->Uninit();
		m_pGauge = nullptr;
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CHeat::Update(void)
{
	if (m_pGauge != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		pos.y -= SIZE_FRAME.y * m_fParam - SIZE_FRAME.y;

		m_pGauge->SetPosition(pos);
		m_pGauge->SetTex(D3DXVECTOR2(0.0f,1.0f - m_fParam), D3DXVECTOR2(1.0f, 1.0f));
		m_pGauge->SetSize(SIZE_FRAME.x, SIZE_FRAME.y * m_fParam);
		m_pGauge->SetVtx();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CHeat::Draw(void)
{
	
}

//=====================================================
// �ʒu�ݒ�
//=====================================================
void CHeat::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;

	if (m_pFrame != nullptr)
	{
		m_pFrame->SetPosition(pos);
		m_pFrame->SetVtx();
	}

	if (m_pGauge != nullptr)
	{
		m_pGauge->SetPosition(pos);
		m_pGauge->SetVtx();
	}
}

//=====================================================
// �t���[���̃e�N�X�`�����蓖��
//=====================================================
void CHeat::BindTextureFrame(const char* pPath)
{
	if (m_pFrame != nullptr)
	{
		int nIdx = CTexture::GetInstance()->Regist(pPath);
		m_pFrame->SetIdxTexture(nIdx);
	}
}