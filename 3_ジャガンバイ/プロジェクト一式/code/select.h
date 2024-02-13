//*****************************************************
//
// �I������[select.h]
// Author:���}���V
//
//*****************************************************
#ifndef _SELECT_H_
#define _SELECT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CBillboard;
class CObject2D;
class CPlayerManager;
class CPlayer;
class CContainer;
class CLift;
class CNumber3D;
class CObjectX;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSelect : public CScene
{
public:
	CSelect();	// �R���X�g���N�^
	~CSelect();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update();
	void Draw();

private:
	enum SELECT_STATE
	{
		STATE_BEFORE = 0,
		STATE_GO,
		STATE_MAX
	};
	// �t�F�[�h�̎��(UI�_�ŗp)
	enum FADE_STATE
	{
		FADE_NONE = 0,	// ����
		FADE_IN,	// �t�F�[�h�C��
		FADE_OUT,	// �t�F�[�h�A�E�g
		FADE_MAX
	};
	// �v���C���[��Ԃ̎��
	enum PLAYER_STATE
	{
		PLAYER_NONE = 0,	// ����
		PLAYER_ENTRY,	// �Q�����
		PLAYER_FREE,	// ���R���
		PLAYER_INGAME,	// �Q�[���֓���
		PLAYER_MAX
	};
	// ���j���[�̍\����
	struct SJoinUi_info
	{
		CBillboard* pUi2D;	//���j���[
		D3DXCOLOR col;	// �F
		FADE_STATE state;	// ���
	};
	// �v���C���[���̍\����
	struct PlayerInfo
	{
		CPlayer* pPlayer;	// �v���C���[
		CObjectX* pLeaf;	// �t����
		PLAYER_STATE state;	// ���
	};
	// �R���e�i���̍\����
	struct CContainerInfo
	{
		CContainer* pContainer;
		float fReSpawnTimer;
	};

	void JoinUiInit(void);	// ���j���[�̏����ݒ�
	void ObjDelete(int nPlayer);	// �폜
	void MenuColorChange(int nPlayer);	// ���j���[�F�̕ύX
	void StartInit(void);	// �X�^�[�g�̏����ݒ�
	void PotatoLeafInit(void);	// �|�e�g�̗t�̏����ݒ�
	void ContainerInit(void);	// �R���e�i�̏����ݒ�
	void ReSetContainer(void);	// �R���e�i�̍Đݒu
	void EntryInput(int nPlayer);	// �Q���̓���
	void MoveLimit(int nPlayer);	// �s������
	void PlayerShowUp(int nPlayer);	// �v���C���[�̓o��

	void Lift(void);	// ���t�g�̐ݒ�

	void LiftInNumberUi(int nPlayer);	// ���t�g�ɓ������l���\��

	SJoinUi_info m_aJoinUiData[NUM_PLAYER];	// �Q��UI
	CObject2D* m_pStartUI;	// Start����
	PlayerInfo m_apPlayerData[NUM_PLAYER];	// �v���C���[���
	CContainerInfo m_aContainerData[NUM_PLAYER];	// �R���e�i
	CLift* m_pLift;	// ���t�g
	CNumber3D* m_apNumber[2];	// ���l���t�g�ɏ���Ă邩�̃e�N�X�`��
	CBillboard* m_pSlash;	// �X���b�V��
	SELECT_STATE m_selectState;	// �l���I���̏��
	bool m_abEntry[NUM_PLAYER];	// �Q���������ǂ���
	bool m_bLiftCamera;	// ���t�g�J�����̐؂�ւ�
	bool m_bLiftUp;	// ���t�g���㏸���邩�ǂ���
	bool m_bSound;	// �T�E���h���Đ����邩�ǂ���
};

#endif