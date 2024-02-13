//*****************************************************
//
// �e�̏���[shadow.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "arrow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "game.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* TEX_PATH = "data\\TEXTURE\\UI\\arrow.png";	// �e�N�X�`���̃p�X
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CArrow::CArrow(int nPriority) : CObject3D(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CArrow::~CArrow()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CArrow::Init(void)
{
	// �p���N���X�̏�����
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CArrow::Uninit(void)
{
	// �p���N���X�̏I��
	CObject3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CArrow::Update(void)
{
	// �p���N���X�̍X�V
	CObject3D::Update();
}

//=====================================================
// �`�揈��
//=====================================================
void CArrow::Draw(void)
{
	// �p���N���X�̕`��
	CObject3D::Draw();
}

//=====================================================
// ��������
//=====================================================
CArrow *CArrow::Create(D3DXVECTOR3 pos, float width, float height)
{
	CArrow *pShadow = nullptr;

	if (pShadow == nullptr)
	{
		pShadow = new CArrow;

		if (pShadow != nullptr)
		{
			// ������
			pShadow->Init();

			pShadow->SetPosition(pos);
			pShadow->SetSize(width, height);

			// �e�N�X�`���̓Ǎ�
			int nIdx = CTexture::GetInstance()->Regist(TEX_PATH);
			pShadow->SetIdxTexture(nIdx);

			pShadow->SetVtx();
		}
	}

	return pShadow;
}