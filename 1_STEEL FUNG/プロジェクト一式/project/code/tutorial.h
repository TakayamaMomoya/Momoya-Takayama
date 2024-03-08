//*****************************************************
//
// �`���[�g���A������[tutorial.h]
// Author:���R����
//
//*****************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTutorial : public CScene
{
public:
	CTutorial();	// �R���X�g���N�^
	~CTutorial();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

private:
	void SpawnEnemy(void);
	void InputSkip(void);

	float m_fSpawnCnt;
	float m_fCntSkip;	// �X�L�b�v�J�E���^�[
	CUI *m_pSkipGauge;	// �X�L�b�v�Q�[�W
};

#endif