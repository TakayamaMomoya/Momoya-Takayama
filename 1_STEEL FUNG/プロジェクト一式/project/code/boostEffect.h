//*****************************************************
//
// �u�[�X�g�G�t�F�N�g����[boostEffect.h]
// Author:���R����
//
//*****************************************************

#ifndef _BOOSTEFFECT_H_
#define _BOOSTEFFECT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CMeshCylinder;
class CAnim3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CBoostEffect : public CObject
{
public:
	CBoostEffect(int nPriority = 3);	// �R���X�g���N�^
	~CBoostEffect();	// �f�X�g���N�^

	static CBoostEffect *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) { return m_info.rot; }
	void SetRadius(float fRadius);
	void SetHeight(float fHeight);
	void SetColor(D3DXCOLOR col);
	float GetRadius(void) { return m_info.fRadius; }
	float GetHeight(void) { return m_info.fHeight; }
	void EnableZtestBoost(bool bZtest);

private:
	struct SInfo
	{
		CAnim3D *pBoost;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXCOLOR col;
		float fRadius;
		float fHeight;
	};

	SInfo m_info;
};

#endif