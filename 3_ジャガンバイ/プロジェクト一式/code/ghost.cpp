//*****************************************************
//
// �H��̏���[ghost.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "ghost.h"
#include "debugproc.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* BODY_PATH[NUM_PLAYER] =
{// �v���C���[�����ڂ̃p�X
	"data\\MOTION\\motionPotatoman01.txt",
	"data\\MOTION\\motionPotatoman02.txt",
	"data\\MOTION\\motionPotatoman03.txt",
	"data\\MOTION\\motionPotatoman04.txt",
};
const float SPEED_UP = 1.0f;	// �㏸���x
const float TIME_DEATH = 3.0f;	// ������܂ł̎���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CGhost::CGhost(int nPriority) : CMotion(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CGhost::~CGhost()
{

}

//=====================================================
// ��������
//=====================================================
CGhost *CGhost::Create(int nIdxPlayer)
{
	CGhost *pGhost = nullptr;

	if (pGhost == nullptr)
	{
		pGhost = new CGhost;

		if (pGhost != nullptr)
		{
			// �̂̓Ǎ�
			pGhost->Load((char*)BODY_PATH[nIdxPlayer]);

			// ������
			pGhost->Init();
		}
	}

	return pGhost;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGhost::Init(void)
{
	// �p���N���X�̏�����
	CMotion::Init();

	SetMotion(2);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGhost::Uninit(void)
{
	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CGhost::Update(void)
{
	// �p���N���X�̍X�V
	CMotion::Update();

	// �㏸
	D3DXVECTOR3 pos = GetPosition();

	pos.y += SPEED_UP;

	SetPosition(pos);

	// �F�̕ω�
	D3DXCOLOR col = GetColor();

	float fTick = CManager::GetTick();

	float fRate = fTick / TIME_DEATH;

	col.a -= fRate;

	if (col.a <= 0.0f)
	{
		col.a = 0.0f;

		Uninit();
	}

	SetAllCol(col);
}

//=====================================================
// �`�揈��
//=====================================================
void CGhost::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();

#ifdef _DEBUG
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}
#endif
}