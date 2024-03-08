//*****************************************************
//
// �x���\���̏���[caution.h]
// Author:���R����
//
//*****************************************************

#ifndef _CAUTION_H_
#define _CAUTION_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CCaution	: public CObject
{
public:
	CCaution(int nPriority = 0);
	~CCaution();

	static CCaution *Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f });	// ��������
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }

private:
	enum STATE
	{
		STATE_NONE = 0,
		STATE_NORMAL,	// �ʏ���
		STATE_FADEOUT,	// ���X�ɏ�������
		STATE_MAX
	};
	struct SInfo
	{
		D3DXVECTOR3 pos;	// �x����3D���W
		CUI *pIcon;	// �A�C�R��
		CUI *pArrow;	// ���
		float fLife;	// ����
	};
	void UpdateIcon(void);

	SInfo m_info;
};

#endif