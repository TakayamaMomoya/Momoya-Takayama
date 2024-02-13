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
	CEnemy *GetHead(void) { return m_pHead; }
	CEnemy *GetTail(void) { return m_pTail; }
	void SetHead(CEnemy *pEnemy) { m_pHead = pEnemy; }
	void SetTail(CEnemy *pEnemy) { m_pTail = pEnemy; }
	void ProgressTimeSpawn(bool bAdd);
	void SetTimeScale(float fScale) { m_fScaleSpawnTime = fScale; }
	static CEnemyManager *GetInstance(void) { return m_pEnemyManager; }

private:
	void Load(void);
	void SpawnNormal(void);
	void SpawnThief(void);

	CEnemy *m_pHead;	// �擪�̃A�h���X
	CEnemy *m_pTail;	// �Ō���̃A�h���X
	float m_fTimerSpawn;	// �ʏ�G�X�|�[���J�E���^�[
	float m_fTimerThief;	// �D�_�G�X�|�[���^�C�}�[
	float m_fTimeSpawnThief;	// �D�_�G���X�|�[������܂ł̎���
	int m_nMinTimeSpawnThief;	// �D�_�G�̃X�|�[���^�C�}�[�̍ŏ�
	int m_nMaxTimeSpawnThief;	// �D�_�G�̃X�|�[���^�C�}�[�̍ő�
	float m_afTime[NUM_PLAYER];	// �G���o������p�x
	float m_fDistSpawn;	// �X�|�[������
	float *m_pAngleSpawn;	// �X�|�[���p�x�̃|�C���^
	int m_nNumSpawnAngle;	// �X�|�[���p�x�̐�
	float m_fRateProgress;	// �i�s�ɂ���ďo���p�x�����炷����
	int m_nMaxEnemy;	// �ő�̓G��
	float m_fScaleSpawnTime;	// �^�C���̃X�P�[��

	static CEnemyManager *m_pEnemyManager;	// ���g�̃|�C���^
};

#endif
