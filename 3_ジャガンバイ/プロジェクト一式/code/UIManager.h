//*****************************************************
//
// UI�}�l�[�W���[[UIManager.h]
// Author:���R����
//
//*****************************************************

#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************

#include "object.h"
#include "weapon.h"

//*****************************************************
// �O���錾
//*****************************************************

class CUI;
class CLife;
class CUIMagazine;

//*****************************************************
// �萔��`
//*****************************************************
namespace LIFE
{
	const float MAX_SIZE = 30.0f;
	const float WIDTH_MAX = 0.8f * MAX_SIZE;
	const float HEIGHT_MAX = 0.7f * MAX_SIZE;
}

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CUIManager : public CObject
{
public:

	CUIManager();	// �R���X�g���N�^
	~CUIManager();	// �f�X�g���N�^

	static CUIManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUIManager *GetInstance(void) { return m_pUIManager; }
	bool IsDisp(void) { return m_bDisp; }
	void EnableDisp(bool bDisp) { m_bDisp = bDisp; }
	void SetPlayer(int nIdx);

private:

	void CreateFace(int nIdx);
	void CreateWeapon(int nIdx);
	void CreateLife(int nIdx);
	void CreateUIMagazine(int nIdx);

	void UpdateUIWeapon(void);
	void UpdateUISubColor(void);
	void UpdateUIChangeColor(void);

	struct SInfo
	{
		CWeapon::TYPE weaponType;	// ����̎��
		bool bIsWeaponNull;			// ����̗L��

		float fUIColorAlpha;		// UI�̃A���t�@�l
		int nCntColorChange;		// �ύX����܂ł̃J�E���g

		CUI *pUiFace;				// ���UI			
		CUI *pUiWeapon;				// �����UI

		CLife *pLife;				// ���C�tUI
		CUIMagazine* pUIMagazine;	// ���e��UI
	};

	static CUIManager *m_pUIManager;	// ���g�̃|�C���^
	bool m_bDisp;				// UI��\�����邩�ǂ���
	SInfo m_aInfo[NUM_PLAYER];	// ���g�̏��
};

#endif
