//*****************************************************
//
// ����}�l�[�W���[[weaponManager.h]
// Author:���R����
//
//*****************************************************

#ifndef _WEAPONMANAGER_H_
#define _WEAPONMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"
#include "weapon.h"
#include <stdio.h>

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CWeaponManager : public CObject
{
public:
	struct SInfoShotgun
	{
		float fAngleDiffuse;	// �g�U�p�x
		int nNumPellet;	// ��x�ɔ��˂���e�̐�
	};
	struct SInfoRailgun
	{
		float fWidth;	// ��
		float fLength;	// ����
	};

	CWeaponManager();	// �R���X�g���N�^
	~CWeaponManager();	// �f�X�g���N�^

	static CWeaponManager*Create(void);
	HRESULT Init(void);
	void LoadParam(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CWeapon::SInfo GetBaseInfo(int nCnt) { return m_aInfo[nCnt]; }
	SInfoShotgun GetShotgunInfo(void) { return m_infoShotgun; }
	SInfoRailgun GetRailgunInfo(void) { return m_infoRailgun; }
	int GetProb(int nIdx) { return m_anProb[nIdx]; }
	static CWeaponManager*GetInstance(void) { return m_pWeaponManager; }

private:
	void LoadBaseInfo(FILE *pFile, char* pTemp, int nCntParam);
	void LoadShotgunInfo(FILE *pFile, char* pTemp);
	void LoadRailgunInfo(FILE *pFile, char* pTemp);

	CWeapon::SInfo m_aInfo[CWeapon::TYPE_MAX];	// ���̃p�����[�^�[
	SInfoShotgun m_infoShotgun;	// �V���b�g�K���̏��
	SInfoRailgun m_infoRailgun;	// ���[���K���̏��
	int m_anProb[CWeapon::TYPE_MAX];	// �m���̔z��

	static CWeaponManager *m_pWeaponManager;	// ���g�̃|�C���^
};

#endif
