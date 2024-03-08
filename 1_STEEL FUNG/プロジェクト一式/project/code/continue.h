//*****************************************************
//
// �R���e�B�j���[�̏���[continue.h]
// Author:���R����
//
//*****************************************************

#ifndef _CONTINUE_H_
#define _CONTINUE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject2D;

//*****************************************************
// �N���X��`
//*****************************************************
class CContinue : CObject
{
public:
	CContinue();	// �R���X�g���N�^
	~CContinue();	// �f�X�g���N�^

	static CContinue *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CContinue *GetInstance(void) { return m_pContinue; }

private:
	enum MENU
	{// ���ڂ̎��
		MENU_RETRY_FROM_CHECKPOINT = 0,	// �`�F�b�N�|�C���g����ĊJ
		MENU_RESTART,	// ���g���C
		MENU_QUIT,	// �I��
		MENU_MAX
	};
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_IN,	// �t�F�[�h�C�����
		STATE_OUT,	// �t�F�[�h�A�E�g���
		STATE_MAX
	};

	void ManageState(void);
	void ManageBg(void);
	void Input(void);
	void Fade(MENU menu);

	MENU m_menu;	// �I������
	CObject2D *m_apMenu[MENU_MAX];	// ���j���[���ڂ̂QD�I�u�W�F�N�g
	CObject2D *m_pBg;	// �w�i�QD�I�u�W�F�N�g
	D3DXVECTOR3 m_aPosDest[MENU_MAX];	// �ڕW�̈ʒu
	STATE m_state;	// ���
	static CContinue *m_pContinue;	// ���g�̃|�C���^
	bool m_bSound;			
};

#endif