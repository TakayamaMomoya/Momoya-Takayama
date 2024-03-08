//*****************************************************
//
// �G�}�l�[�W���[[enemyManager.h]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"
#include "enemy.h"
#include <iostream>
#include <list>

//*****************************************************
// �O���錾
//*****************************************************
class CUI;
class CFan2D;

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* PATH_BODY_ENEMY[CEnemy::TYPE::TYPE_MAX] =
{// ���[�V�����̃p�X
		nullptr,
		"data\\MOTION\\motionHeli.txt",
		"data\\MOTION\\motionBomb.txt",
		"data\\MOTION\\motionDrone01.txt",
		"data\\MOTION\\motionHeli.txt",
		"data\\MOTION\\motionArms00.txt",
};
}

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyManager : public CObject
{
public:
	CEnemyManager();	// �R���X�g���N�^
	~CEnemyManager();	// �f�X�g���N�^

	CEnemy *CreateEnemy(D3DXVECTOR3 pos, CEnemy::TYPE type);
	static CEnemyManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CEnemy *Lockon(CEnemy *pEnemyExclusive = nullptr);
	void EnableLockTarget(bool bLock);
	bool IsLockTarget(void) { return m_bLockTarget; }
	CEnemy *SwitchTarget(int nAxisX, int nAxisY, CEnemy *pEnemyExclusive = nullptr);
	void CheckDeathLockon(CEnemy *pEnemy);
	CEnemy *GetLockon(void) { return m_pEnemyLockon; }
	void DeleteAll(void);
	void SetEnemyLock(CEnemy *pEnemy) { m_pEnemyLockon = pEnemy; }
	void SpawnGroup(int nIdx);
	bool IsEndSpawn(void) { return m_bEndSpawn; }
	void EnableEndSpawn(bool bEnd) { m_bEndSpawn = bEnd; }
	std::list<CEnemy*> GetListRanking(void) { return m_list; }
	void AddToList(CEnemy *pEnemy);
	void RemoveFromList(CEnemy *pEnemy);
	static CEnemyManager *GetInstance(void) { return m_pEnemyManager; }

private:
	struct SInfoEnemy
	{// �G�̔z�u���
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 posDestInitial;	// �����ڕW�ʒu
		int nType;	// ���
		float fDelaySpawn;	// �X�|�[���f�B���C
	};
	struct SInfoEnemyGroup
	{// �G�W�c�̏��
		int nNumEnemy;	// �G�̐�
		SInfoEnemy *pInfoEnemy;	// �G�̏��
	};

	void Load(void);
	void CreateGauge(void);
	void DeleteGauge(void);
	void ControlGauge(void);

	std::list<CEnemy*> m_list;	// �ꗗ���X�g
	SInfoEnemyGroup *m_pInfoGroup;	// �G�W�c�̏��
	CEnemy *m_pEnemyLockon;	// ���b�N�I�����Ă�G
	bool m_bLockTarget;	// �^�[�Q�b�g�����b�N���Ă��邩�ǂ���
	CUI *m_pCursor;	// ���b�N�I���J�[�\��
	CUI *m_pIsLock;	// ���b�N���Ă邩�̕\��
	CFan2D *m_pObjectGauge;	// �Q�[�W�̃|�C���^
	CFan2D *m_pObjectFrame;	// �t���[���̃|�C���^
	bool m_bEndSpawn;	// �X�|�[���I���t���O
	float m_fTimerSpawn;	// �X�|�[���^�C�}�[
	int m_nCntSpawn;	// �X�|�[���J�E���^�[
	float m_fTimerChange;	// ���b�N�ւ��^�C�}�[

	static CEnemyManager *m_pEnemyManager;	// ���g�̃|�C���^
};

#endif
