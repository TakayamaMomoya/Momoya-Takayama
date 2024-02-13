//*****************************************************
//
// �����X�|�i�[����[explosionspawner.h]
// Author:���R����
//
//*****************************************************

#ifndef _EXPLSPAWNER_H_
#define _EXPLSPAWNER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CExplSpawner : public CObject
{
public:
	CExplSpawner(void);	// �R���X�g���N�^
	~CExplSpawner();	// �f�X�g���N�^

	static CExplSpawner *Create(D3DXVECTOR3 pos, float fRadius, int nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); }	// �擾����

private:
	D3DXVECTOR3 m_pos;	// �ʒu
	int m_nLife;	// ����
	float m_fRadius;	// ���a
	int m_nCntSpawn;	// �X�|�[���J�E���^�[
	int m_nTimerSpawn;	// �X�|�[���^�C�}�[
};

#endif