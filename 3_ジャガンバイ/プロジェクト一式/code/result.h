//*****************************************************
//
// ���U���g�̏���[result.h]
// Author:���R����
//
//*****************************************************

#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject2D;
class CPlayer;
class CNumber;

//*****************************************************
// �N���X��`
//*****************************************************
class CResult : CObject
{
public:
	CResult();	// �R���X�g���N�^
	~CResult();	// �f�X�g���N�^

	static CResult *Create(bool bWin = false);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetSurvived(CPlayer *pPlayer);

private:
	enum STATE
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_WAIT,	// ���͑ҋ@���
		STATE_CURRENT,	// ���͂��Ă�����
		STATE_MAX
	};
	struct SInfoSuvived
	{// �����ҏ��
		CPlayer *pSuvived;	// �����҂̃|�C���^
		CObject2D *pIcon;	// �v���C���[�̃A�C�R��
		CObject2D *pCaption;	// �L���v�V����
	};

	void Input(void);
	void Create2D(bool bWin);
	void DispSuvived(SInfoSuvived *pInfo);

	CObject2D *m_pBg;	// �w�i�̃|�C���^
	CObject2D *m_p2DResult;	// ���o���̃|�C���^
	STATE m_state;	// ���
	SInfoSuvived m_aInfoSurvived[NUM_PLAYER];	// �����ҏ��
	int m_nNumSuvived;	// �����Ґ�
};

#endif