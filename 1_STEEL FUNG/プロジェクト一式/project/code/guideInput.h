//*****************************************************
//
// ����K�C�h�̏���[guideInput.h]
// Author:���R����
//
//*****************************************************

#ifndef _GUIDEINPUT_H_
#define _GUIDEINPUT_H_

#include "Object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGuideInput : public CObject
{
public:
	enum INPUT
	{// �\��������͂̎��
		INPUT_JUMP = 0,	// �W�����v
		INPUT_SHOT,	// �ˌ�
		INPUT_MELEE,	// �ߐڍU��
		INPUT_GRAB,	// �͂ݍU��
		INPUT_DODGE,	// ���
		INPUT_LOCK,	// ���b�N�I��
		INPUT_MAX
	};

	CGuideInput(int nPriority = 0);	// �R���X�g���N�^
	~CGuideInput();	// �f�X�g���N�^

	static CGuideInput *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	struct SInfoGuide
	{
		CUI *pFrame;	// �t���[��
		float fTime;	// ���鎞��
	};

	void CheckInput(void);

	SInfoGuide m_aGuide[INPUT_MAX];	// �K�C�h�\���̏��
};

#endif
