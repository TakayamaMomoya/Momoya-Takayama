//*****************************************************
//
// ���̏���[arrow.h]
// Author:���R����
//
//*****************************************************

#ifndef _ARROW_H_
#define _ARROW_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object3D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CArrow : public CObject3D
{
public:
	CArrow(int nPriority = 6);	// �R���X�g���N�^
	~CArrow();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CArrow *Create(D3DXVECTOR3 pos,float width,float height);

private:
};

#endif
