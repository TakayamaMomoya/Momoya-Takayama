//*****************************************************
//
// �r�[���u���[�h�̏���[beamBlade.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "beamBlade.h"
#include "meshcylinder.h"
#include "object3D.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int MESH_V = 6;	// ���b�V���̏c�̕�����
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CBeamBlade::CBeamBlade(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBeamBlade::~CBeamBlade()
{

}

//=====================================================
// ��������
//=====================================================
CBeamBlade *CBeamBlade::Create(void)
{
	CBeamBlade *pBeamBlade = nullptr;

	if (pBeamBlade == nullptr)
	{
		pBeamBlade = new CBeamBlade;

		if (pBeamBlade != nullptr)
		{
			// ������
			pBeamBlade->Init();
		}
	}

	return pBeamBlade;
}

//=====================================================
// ����������
//=====================================================
HRESULT CBeamBlade::Init(void)
{
	m_info.fRadius = 100.0f;
	m_info.fHeight = 400.0f;
	m_info.col = { 1.0f,1.0f,1.0f,1.0f };

	if (m_info.pBlade == nullptr)
	{// �u���[�h�����̐���
		m_info.pBlade = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (m_info.pBlade != nullptr)
		{
			m_info.pBlade->SetMode(CObject3D::MODE::MODE_STRETCHBILLBOARD);
			m_info.pBlade->SetRotation(D3DXVECTOR3(-1.57f, 0.0f, 0.0f));
			m_info.pBlade->SetSize(m_info.fRadius, m_info.fHeight);
			m_info.pBlade->SetPosition(D3DXVECTOR3(0.0f, m_info.fHeight, 0.0f));
			m_info.pBlade->EnableAdd(true);

			int nIdx = Texture::GetIdx("data\\TEXTURE\\EFFECT\\blade.png");

			m_info.pBlade->SetIdxTexture(nIdx);
		}
	}

	if (m_info.pCylinder == nullptr)
	{// �V�����_�[�̐���
		m_info.pCylinder = CMeshCylinder::Create(16, MESH_V);

		if (m_info.pCylinder != nullptr)
		{
			m_info.pCylinder->SetHeight(m_info.fHeight / MESH_V);
			m_info.pCylinder->SetRadius(m_info.fRadius * 0.8f);
			m_info.pCylinder->SetVtx();
			m_info.pCylinder->EnableAdd(true);

			int nIdx = Texture::GetIdx("data\\TEXTURE\\EFFECT\\energy00.png");

			m_info.pCylinder->SetIdxTexture(nIdx);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBeamBlade::Uninit(void)
{
	if (m_info.pBlade != nullptr)
	{
		m_info.pBlade->Uninit();
		m_info.pBlade = nullptr;
	}

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->Uninit();
		m_info.pCylinder = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CBeamBlade::Update(void)
{
	// �V�����_�[���ۂ�����
		// ���擾
	CMeshCylinder *pMesh = m_info.pCylinder;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_info.pCylinder->GetVtxBuff();

	if (pMesh == nullptr || pVtxBuff == nullptr)
	{
		return;
	}

	CMeshCylinder::MeshCylinder *pInfoMesh = pMesh->GetMeshCylinder();

	int nMeshU = pInfoMesh->nMeshU;
	int nMeshV = pInfoMesh->nMeshV;
	float fRadius = pInfoMesh->fRadius;
	float fHeight = pInfoMesh->fHeight;

	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fRot;
	float fSin = D3DX_PI / nMeshV;

	for (int nCountV = 0; nCountV < nMeshV + 1; nCountV++)
	{//���_���W�̐ݒ�
		for (int nCountU = 0; nCountU < nMeshU + 1; nCountU++)
		{
			//�p�x�Z�o
			fRot = nCountU * (D3DX_PI / nMeshU) * 2;
			
			float fAdd = 20.0f * sinf(fSin * nCountV);

			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x = sinf(fRot) * (fRadius + fAdd);
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z = cosf(fRot) * (fRadius + fAdd);
		}
	}

	//���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	// �����ڂ̒Ǐ]
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetPosition(pos);
		m_info.pCylinder->SetRotation(rot);
	}

	if (m_info.pBlade != nullptr)
	{
		pos +=
		{
			sinf(rot.x) * sinf(rot.y) * m_info.fHeight,
				cosf(rot.x) * m_info.fHeight,
				sinf(rot.x) * cosf(rot.y) * m_info.fHeight
		};

		rot.x -= 1.57f;

		m_info.pBlade->SetPosition(pos);
		m_info.pBlade->SetRotation(rot);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CBeamBlade::Draw(void)
{

}

// ���a�ݒ�
void CBeamBlade::SetRadius(float fRadius)
{
	m_info.fRadius = fRadius;

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetRadius(fRadius * 0.8f);
	}

	if (m_info.pBlade != nullptr)
	{
		float fHeight = m_info.pBlade->GetHeight();

		m_info.pBlade->SetSize(fRadius, fHeight);
		m_info.pBlade->SetVtx();
	}
}

// �����ݒ�
void CBeamBlade::SetHeight(float fHeight)
{
	m_info.fHeight = fHeight;

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetHeight(fHeight / MESH_V);
	}

	if (m_info.pBlade != nullptr)
	{
		float fWidth = m_info.pBlade->GetWidth();

		m_info.pBlade->SetSize(fWidth, fHeight);
	}
}

// �F�ݒ�
void CBeamBlade::SetColor(D3DXCOLOR col)
{
	m_info.col = col;

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetCol(col);
	}

	if (m_info.pBlade != nullptr)
	{
		m_info.pBlade->SetColor(col);
	}
}