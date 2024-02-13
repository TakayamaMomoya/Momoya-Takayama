//*****************************************************
//
// �v���C���[�}�l�[�W���[[PlayerManager.h]
// Author:���R����
//
//*****************************************************

#ifndef _PLAYERMANAGER_H_
#define _PLAYERMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"
#include "player.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayerManager
{
public:
	CPlayerManager();	// �R���X�g���N�^
	~CPlayerManager();	// �f�X�g���N�^

	static CPlayerManager *Create(void);
	void CreatePlayer(void);
	CPlayer *BindPlayer(int nIdxJoypad);
	HRESULT Init(void);
	void Uninit(void);
	void ReleasePlayer(int nIdx);
	CPlayer *GetPlayer(int nIdx) { return m_apPlayer[nIdx]; }
	static CPlayerManager *GetInstance(void) { return m_pPlayerManager; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	CPlayer::SParam GetPlayerParam(void) { return m_paramPlayer; }
	int GetNumAttack(void) { return m_nNumAttack; }
	CPlayer::AttackInfo *GetAttackInfo(void) { return m_pInfoAttack; }
	void SetDeathPlayer(int nID, bool bDeath) { m_abDeathPlayer[nID] = bDeath; }
	bool GetDeathPlayer(int nID) { return m_abDeathPlayer[nID]; }

private:
	void LoadParamPlayer(void);

	int m_nNumPlayer;	// �v���C���[�̐�
	int m_anIDJoypad[NUM_PLAYER];	// �v���C���[�̃R���g���[���[�ԍ�
	CPlayer *m_apPlayer[NUM_PLAYER];	// �v���C���[�̔z��
	static CPlayerManager *m_pPlayerManager;	// ���g�̃|�C���^
	CPlayer::SParam m_paramPlayer;	// �ۑ��p�v���C���[�p�����[�^�[
	CPlayer::AttackInfo *m_pInfoAttack;	// �U�����̃|�C���^
	int m_nNumAttack;	// �U������̐�
	bool m_abDeathPlayer[NUM_PLAYER];		// �v���C���[�����S���Ă��邩
};

#endif
