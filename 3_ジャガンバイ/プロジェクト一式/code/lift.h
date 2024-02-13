//*****************************************************
//
// ���t�g����[lift.h]
// Author:���}���V
//
//*****************************************************

#ifndef _STARTLOCATION_H_
#define _STARTLOCATION_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CLift : public CObjectX
{
public:
	enum STATE
	{
		STATE_NONE = 0,
		STATE_IN,
		STATE_UP,
		STATE_END,
		STATE_MAX
	};

public:
	CLift(int nPriority = 6);	// �R���X�g���N�^
	~CLift();	// �f�X�g���N�^

	static CLift* Create(D3DXVECTOR3 pos, int nPriority = 3);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);	// �Ǎ�

	static void SetjoinPlayer(int nPlayer) { m_nNumJoinPlayer = nPlayer; }
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }

	static int GetInPlayer(void);
	static bool GetIsIn(void);

private:
	D3DXVECTOR3 m_pos;
	static int m_nNumJoinPlayer;
	static int m_nInCnt;
	static bool m_abJoin[NUM_PLAYER];
	STATE m_state;

};

#endif