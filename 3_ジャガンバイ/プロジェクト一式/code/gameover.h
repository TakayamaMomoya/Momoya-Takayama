//=========================================================
//
// �Q�[���I�[�o�[���� [gameover.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _GAMEOVER_H_     // ���̃}�N����`������ĂȂ�������
#define _GAMEOVER_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

//===============================================
// �C���N���[�h
//===============================================
#include "main.h"
#include "motion.h"
#include "object2D.h"

//===============================================
// �萔��`
//===============================================
namespace
{
	const int NUM_ENEMY = 5;		// ��������G�̑���
}

//===============================================
// �Q�[���I�[�o�[�N���X
//===============================================
class CGameover
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CGameover();						// �f�t�H���g�R���X�g���N�^
	~CGameover();						// �f�X�g���N�^

	static CGameover* Create(int nPriority = 3);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BlackOut(void);

	static CGameover* GetInstance(void) { return m_pGameover; }
	void SetDeathPlayer(int nID, bool bDeath) { m_bDeathPlayer[nID] = bDeath; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CGameover* m_pGameover;	// ���g�̃|�C���^
	CMotion* m_apModelPlayer[NUM_PLAYER];		// �v���C���[���f���̃|�C���^
	CMotion* m_apModelEnemy[NUM_ENEMY];			// �G�l�~�[���f���̃|�C���^
	CObject2D* m_pBlack;						// ���|���S��
	CObject2D* m_pLogo;							// �Q�[���I�[�o�[�\��
	CObject2D* m_pContiny;						// �R���e�B�j���[�\��
	bool m_bDeathPlayer[NUM_PLAYER];			// ���S�����v���C���[
	bool m_bFlash;								// �_�Ő؂�ւ�
	float m_fFadeCounter;						// �������\������
	float m_fFlashCounter;						// �_�ł�����
};

#endif