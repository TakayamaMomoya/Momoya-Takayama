//*****************************************************
//
// �M�ʕ\���̏���[heat.h]
// Author:���R����
//
//*****************************************************

#ifndef _HEAT_H_
#define _HEAT_H_

#include "Object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CHeat : public CObject
{
public:
	CHeat(int nPriority = 6);	// �R���X�g���N�^
	~CHeat();	// �f�X�g���N�^

	static CHeat *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetParam(float fParam) { m_fParam = fParam; }
	void BindTextureFrame(const char* pPath);

private:
	float m_fParam;	// �p�����[�^�l
	D3DXVECTOR3 m_pos;	// �ʒu
	CUI *m_pFrame;	// �t���[��
	CUI *m_pGauge;	// �Q�[�W
};

#endif
