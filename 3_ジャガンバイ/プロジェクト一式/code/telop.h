//*****************************************************
//
// �e���b�v�̏���[telop.h]
// Author:���}���V
//
//*****************************************************

#ifndef _TELOP_H_
#define _TELOP_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject2D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTelop : public CObject
{
public:
	enum STATE
	{
		STATE_NONE = 0,
		STATE_INIT,
		STATE_END,
		STATE_MAX
	};

	CTelop(int nPriority = 6);	// �R���X�g���N�^
	~CTelop();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTelop* Create(D3DXVECTOR3 pos);

private:
	D3DXVECTOR3 m_pos;
	CObject2D* m_pObjet2D;
	STATE m_state;
	float m_fTimer;
};

#endif

