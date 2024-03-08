//*****************************************************
//
// �`�F�b�N�|�C���g��Behavior[checkpointBehavior.h]
// Author:���R����
//
//*****************************************************

#ifndef _CHECKPOINTBEHAVIOR_H_
#define _CHECKPOINTBEHAVIOR_H_

//*****************************************************
// �O���錾
//*****************************************************
class CCheckPointManager;
class CLimit;
class CUI;

//*****************************************************
// �萔��`
//*****************************************************
namespace CheckPointBehavior
{
const int NUM_LIMIT = 2;	// �ʒu�����̐�
}

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CCheckPointBehavior
{
public:
	CCheckPointBehavior();
	virtual ~CCheckPointBehavior();

	virtual void Init(CCheckPointManager *pCheckPoint) = 0;
	virtual void Update(CCheckPointManager *pCheckPoint) = 0;
	virtual void Uninit(CCheckPointManager *pCheckPoint) = 0;
private:

};

class CCheckPointMove : public CCheckPointBehavior
{// �ړ��p�[�g
public:
	CCheckPointMove();
	~CCheckPointMove() override;

private:
	void Init(CCheckPointManager *pCheckPoint) override;
	void Update(CCheckPointManager *pCheckPoint) override;
	void Uninit(CCheckPointManager *pCheckPoint) override;

	CUI *m_pCursor;	// �J�[�\���̃|�C���^
};

class CCheckPointBattle : public CCheckPointBehavior
{// �퓬�p�[�g
public:
	CCheckPointBattle();
	~CCheckPointBattle() override;

private:
	void Init(CCheckPointManager *pCheckPoint) override;
	void Update(CCheckPointManager *pCheckPoint) override;
	void Uninit(CCheckPointManager *pCheckPoint) override;
	CLimit *m_apLimit[CheckPointBehavior::NUM_LIMIT];	// �ʒu�����̃|�C���^
};

#endif