//*****************************************************
//
// �c�e��UI�̏���[UIMagazine.h]
// Author:���R����
//
//*****************************************************

#ifndef _UI_MAGAZINE_H_
#define _UI_MAGAZINE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************

class CUI;
class CNumber;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CUIMagazine : public CObject
{
public:

	CUIMagazine(int nPriority = 7);	// �R���X�g���N�^
	~CUIMagazine();	// �f�X�g���N�^

	static CUIMagazine* Create(int nIdx);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 posUIMagazine, D3DXVECTOR3 posUIMagazineFrame, D3DXVECTOR3 posNum);
	void SetSize(float width, float height, float widthNum, float heightNum);
	void SetCol(D3DXCOLOR colMagazine, D3DXCOLOR colMagazineFrame, D3DXCOLOR colNum);
	void SetTexture(const char* pFileName);
	void SetNumMagazine(void);
	void SetVtxGage(void);

	D3DXCOLOR GetColMagazine(void) { return m_info.colMagazine; }
	D3DXCOLOR GetColMagazineFrame(void) { return m_info.colMagazineFrame; }
	D3DXCOLOR GetColNum(void) { return m_info.colNum; }

	bool GetColorChange(void) { return m_info.bColorChange; }
	void SetColorChange(bool bColChange) { m_info.bColorChange = bColChange; }

private:

	struct Sinfo
	{
		int nIdxPlayer;		// �Q�Ƃ���v���C���[�̔ԍ�
		int nBullet;		// �c�e��

		D3DXVECTOR3 posUIMagazine;			// �ʒu���e��
		D3DXVECTOR3 posUIMagazineFrame;		// �ʒu���e���g
		D3DXVECTOR3 posNum;					// �ʒu����

		float fWidth;						// ����
		float fHeight;						// �c��
		float fWidthNum;					// ��������
		float fHeightNum;					// �c������

		D3DXCOLOR colMagazine;				// �F���e��
		D3DXCOLOR colMagazineFrame;			// �F���e���g
		D3DXCOLOR colNum;					// �F����
		bool bColorChange;					// �F�ύX�t���O

		float fWidthSub;					// ���Z���鉡��
		float fHeightSub;					// ���Z����c��

		int nIdxTexture;					// �e�N�X�`���ԍ�
		static LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`���ւ̃|�C���^
	};

	CUI* m_pUIMagazineFrame;	// �}�K�W��UI�g�̃|�C���^
	CUI* m_pUIMagazine;			// �}�K�W��UI�̃|�C���^
	CNumber* m_pNumDig;			// ���e���i�����j�̃|�C���^
	Sinfo m_info;				// ���g�̏��
};

#endif
