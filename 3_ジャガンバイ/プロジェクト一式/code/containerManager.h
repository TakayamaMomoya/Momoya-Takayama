//*****************************************************
//
// �R���e�i�}�l�[�W���[[containerManager.h]
// Author:���R����
//
//*****************************************************

#ifndef _CONTAINERMANAGER_H_
#define _CONTAINERMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"
#include "Container.h"
#include <stdio.h>

//*****************************************************
// �萔��`
//*****************************************************
namespace containermanager
{
const int MAX_SAVE = 24;	// �ۑ��ł���R���e�i�ʒu�̐�
}

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CContainerManager : public CObject
{
public:
	CContainerManager();	// �R���X�g���N�^
	~CContainerManager();	// �f�X�g���N�^

	static CContainerManager*Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CContainerManager*GetInstance(void) { return m_pContainerManager; }
	void SavePosition(D3DXVECTOR3 pos);

private:
	struct SInfoSave
	{// �ۑ�������
		float fTimeSpawn;	// �X�|�[������
		D3DXVECTOR3 pos; // �ʒu
	};

	void Load(void);
	void DeleteSaveInfo(int nIdx);

	SInfoSave *m_apInfoSave[containermanager::MAX_SAVE];	// �ۑ����̃|�C���^
	float m_fTimeSpawm;	// �X�|�[���p�x
	static CContainerManager *m_pContainerManager;	// ���g�̃|�C���^
};

#endif
