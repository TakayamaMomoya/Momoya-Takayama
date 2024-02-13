//*****************************************************
//
// �e���b�v[telop.cpp]
// Author:���}���V
//
//*****************************************************
#include "telop.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* TEX_PATH = "data\\TEXTURE\\UI\\caution.png";
	const D3DXVECTOR2 INIT_SIZE = { 430.0f, 200.0f };	// �����T�C�Y
	const D3DXVECTOR2 END_SIZE = { 200.0f, 80.0f };	// �I���T�C�Y
	const float SPEED = 0.2f;
	const float TIMER = 5.0f;
}

//*****************************************************
// �R���X�g���N�^
//*****************************************************
CTelop::CTelop(int nPriority)
{
	m_pObjet2D = nullptr;
	m_fTimer = 0.0f;
	m_state = STATE_NONE;
}

//*****************************************************
// �f�X�g���N�^
//*****************************************************
CTelop::~CTelop()
{

}

//*****************************************************
// ��������
//*****************************************************
CTelop* CTelop::Create(D3DXVECTOR3 pos)
{
	CTelop* pTelop = nullptr;

	if (pTelop == nullptr)
	{
		pTelop = new CTelop;

		if (pTelop != nullptr)
		{
			pTelop->m_pos = pos;

			pTelop->Init();
		}
	}

	return pTelop;
}

//*****************************************************
// ����������
//*****************************************************
HRESULT CTelop::Init(void)
{
	m_pObjet2D = CObject2D::Create();

	if (m_pObjet2D != nullptr)
	{
		m_pObjet2D->SetSize(INIT_SIZE.x, INIT_SIZE.y);
		m_pObjet2D->SetCol({ 1.0f, 1.0f, 1.0f, 0.0f });
		m_pObjet2D->SetPosition(m_pos);

		int nIdx = CTexture::GetInstance()->Regist(TEX_PATH);
		m_pObjet2D->SetIdxTexture(nIdx);

		m_pObjet2D->SetVtx();
	}

	m_fTimer = TIMER;
	m_state = STATE_INIT;

	EnableNotStop(true);

	return S_OK;
}

//*****************************************************
// �I������
//*****************************************************
void CTelop::Uninit(void)
{
	m_pObjet2D->Uninit();

	Release();
}

//*****************************************************
// �X�V����
//*****************************************************
void CTelop::Update(void)
{
	if (m_pObjet2D == nullptr)
	{
		return;
	}

	D3DXVECTOR2 size = m_pObjet2D->GetSize();
	D3DXCOLOR col = m_pObjet2D->GetCol();

	if (m_state != STATE_END)
	{
		if (col.a < 1.0f)
		{
			col.a += SPEED;
		}
		else
		{
			col.a = 1.0f;
		}

		if ((size.x + size.y) > (END_SIZE.x + END_SIZE.y))
		{
			size.x *= 0.9f;
			size.y *= 0.9f;
		}
		else
		{
			size.x = END_SIZE.x;
			size.y = END_SIZE.y;

			m_state = STATE_END;
		}
	}
	else
	{
		float fTick = CManager::GetTick();

		m_fTimer -= fTick;

		if (m_fTimer < 0)
		{
			if (col.a > 0.0f)
			{
				col.a -= SPEED;
			}
			else
			{
				Uninit();

				return;
			}
		}
	}

	// �F�ύX
	m_pObjet2D->SetCol(col);

	// �傫���̕ύX
	m_pObjet2D->SetSize(size.x, size.y);

	// ���f
	m_pObjet2D->SetVtx();
}

//*****************************************************
// �`�揈��
//*****************************************************
void CTelop::Draw(void)
{
	
}