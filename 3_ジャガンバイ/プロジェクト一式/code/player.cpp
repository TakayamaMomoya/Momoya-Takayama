//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "player.h"
#include "playerManager.h"
#include "motion.h"
#include "debugproc.h"
#include "inputjoypad.h"
#include "inputkeyboard.h"
#include "universal.h"
#include "collision.h"
#include "game.h"
#include "manager.h"
#include "arrow.h"
#include "effect3D.h"
#include "motionDiv.h"
#include "enemyManager.h"
#include "sound.h"
#include "UIManager.h"
#include "record.h"
#include "ghost.h"
#include "rocket.h"
#include "goal.h"
#include "animEffect3D.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* BODY_PATH_LOWER[NUM_PLAYER] =
{// 下半身のパス
	"data\\MOTION\\motionPotatoman01_lower.txt",
	"data\\MOTION\\motionPotatoman02_lower.txt",
	"data\\MOTION\\motionPotatoman03_lower.txt",
	"data\\MOTION\\motionPotatoman04_lower.txt",
};

const char* BODY_PATH_UPPER[NUM_PLAYER] =
{// 上半身のパス
	"data\\MOTION\\motionPotatoman01_upper.txt",
	"data\\MOTION\\motionPotatoman02_upper.txt",
	"data\\MOTION\\motionPotatoman03_upper.txt",
	"data\\MOTION\\motionPotatoman04_upper.txt",
};

const D3DXCOLOR COL_ARROW[NUM_PLAYER] =
{// 矢印の色
	D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,1.0f,1.0f),
	D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),
	D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),
};	// 矢印の色

const float MOVE_LINE = 0.2f;		// 動いている判断のしきい値
const float ARROW_POSY = 5.0f;	// 矢印の位置の高さ
const float ARROW_WIDTH = 30.0f;	// 矢印の幅
const float ARROW_HEIGHT = 50.0f;	// 矢印の高さ
const float GRAVITY = 1.58f;	// 重力
const float POW_PUNCH_UP = 15.0f;	// パンチで飛び上がるジャンプ量
const float SPEED_LIFE_FADE = 0.01f;	// ライフ表示の消える速度
const float SIZE_LIFE = 50.0f;	// ライフ表示のサイズ
const D3DXVECTOR3 POS_LIFE = { 50.0f,50.0f,100.0f };	// ライフ表示の位置

const int HAND_PARTS_NUM = 6;				// 手の番号
const float MOTION_STICK_RUNAWAY = 0.1f;	// スティックの暴走判定

const float LIMID_RANGE_LEFT = -460.0f;
const float LIMID_RANGE_RIGHT = 460.0f;
const float LIMID_RANGE_UP = 460.0f;
const float LIMID_RANGE_DOWN = -460.0f;
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	ZeroMemory(&m_info, sizeof(CPlayer::SInfo));
	ZeroMemory(&m_param, sizeof(CPlayer::SParam));
	m_bSound = false;
	m_bGoalJump = false;
}

//=====================================================
// デストラクタ
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// 生成処理
//=====================================================
CPlayer *CPlayer::Create(int nID)
{
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->m_info.nID = nID;

		pPlayer->Init();
	}

	return pPlayer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayer::Init(void)
{
	m_bGoalJump = false;
	
	// 継承クラスの初期化
	CCharacterDiv::Init();

	// パラメーターの受け取り
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager != nullptr)
	{
		SParam param = pPlayerManager->GetPlayerParam();

		m_param = param;

		int nNumAttack = pPlayerManager->GetNumAttack();

		m_info.nNumAttack = nNumAttack;

		AttackInfo *pAttackInfo = pPlayerManager->GetAttackInfo();

		m_info.pAttackInfo = pAttackInfo;
	}
	else
	{
		assert(("プレイヤーパラメーターの取得に失敗",false));
	}

	// パラメーターの受け取り
	CUIManager* pUIManager = CUIManager::GetInstance();

	if (pUIManager != nullptr)
	{
		pUIManager->SetPlayer(m_info.nID);
	}
	else
	{
		assert(("UI管理（プレイヤー）の取得に失敗", false));
	}


	// 当たり判定の生成
	if (m_info.pCollisionSphere == nullptr)
	{
		m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollisionSphere != nullptr)
		{
			m_info.pCollisionSphere->SetRadius(20.0f);
		}
	}

	if (m_info.pClsnAttack == nullptr)
	{// 球の当たり判定生成
		m_info.pClsnAttack = CCollisionSphere::Create(CCollision::TAG_NONE, CCollision::TYPE_SPHERE, this);

		if (m_info.pClsnAttack != nullptr)
		{// 情報の設定
			m_info.pClsnAttack->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_info.pClsnAttack->SetRadius(0.0f);
		}
	}

	if (m_info.pCollisionCube == nullptr)
	{// 当たり判定生成
		m_info.pCollisionCube = CCollisionCube::Create(CCollision::TAG_PLAYER, this);

		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 vtxMax = { 20.0f,100.0f,20.0f };
			D3DXVECTOR3 vtxMin = { -20.0f,0.0f,-20.0f };

			m_info.pCollisionCube->SetPosition(GetPosition());

			m_info.pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	if (m_info.pArrow == nullptr)
	{// 矢印の生成
		m_info.pArrow = CArrow::Create(GetPosition(), ARROW_WIDTH, ARROW_HEIGHT);

		if (m_info.pArrow != nullptr)
		{
			m_info.pArrow->SetColor(COL_ARROW[m_info.nID]);
		}
	}

	m_info.fLife = m_param.fInitialLife;
	m_info.state = STATE_NORMAL;

	// IDに対応したモデルの設定
	CCharacterDiv::Load((char*)BODY_PATH_LOWER[m_info.nID], (char*)BODY_PATH_UPPER[m_info.nID]);

	// 影の有効化
	for (int nCutPath = 0; nCutPath < CCharacterDiv::PARTS_MAX; nCutPath++)
	{
		CMotionDiv* pBody = GetBody();

		if (pBody != nullptr)
		{
			pBody->SetPosShadow(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
			pBody->EnableShadow(true);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager != nullptr)
	{// 保管してある自身のポインタを削除
		pPlayerManager->ReleasePlayer(m_info.nID);
	}

	if (m_info.pCollisionSphere != nullptr)
	{
		m_info.pCollisionSphere->Uninit();
		m_info.pCollisionSphere = nullptr;
	}

	if (m_info.pCollisionCube != nullptr)
	{
		m_info.pCollisionCube->Uninit();
		m_info.pCollisionCube = nullptr;
	}

	if (m_info.pWeapon != nullptr)
	{
		m_info.pWeapon->Uninit();
		m_info.pWeapon = nullptr;
	}
	
	if (m_info.pArrow != nullptr)
	{
		m_info.pArrow->Uninit();
		m_info.pArrow = nullptr;
	}

	if (m_info.pClsnAttack != nullptr)
	{
		m_info.pClsnAttack->Uninit();
		m_info.pClsnAttack = nullptr;
	}

	// ライフ表示の削除
	DeleteLife();

	// 継承クラスの終了
	CCharacterDiv::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
	// 継承クラスの更新
	CCharacterDiv::Update();

	// ロケットに乗り込む
	BoardingRocket();

	// 入力
	Input();

	// 位置の反映
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();
	
	// 前回の位置を保存
	SetPositionOld(pos);

	pos += move;
	SetPosition(pos);

	// 当たり判定の追従
	if (m_info.pCollisionSphere != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posWaist = GetBody()->GetMtxPos(CCharacterDiv::PARTS_UPPER,0);
		
		// 敵との接触判定
		m_info.pCollisionSphere->SetPosition(posWaist);

		bool bHit = m_info.pCollisionSphere->OnEnter(CCollision::TAG_ENEMY);

		if (bHit)
		{
			Hit(5.0f);
		}

		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 posCollision = m_info.pCollisionCube->GetPosition();

			// 判定の追従
			m_info.pCollisionCube->SetPositionOld(posCollision);
			m_info.pCollisionCube->SetPosition(pos);

			// ブロックとの押し出し判定
			m_info.pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);

			pos = GetPosition();
			D3DXVECTOR3 posOld = GetPositionOld();

			if (pos.y <= 0.0f && posOld.y >= 0.0f)
			{
				pos.y = 0.0f;
				move.y = 0.0f;

				SetPosition(pos);

				if (m_info.state == STATE_BLOW)
				{
					m_info.state = STATE_NORMAL;
				}
			}

			SetMove(move);
		}

		if (m_info.pCollisionSphere != nullptr)
		{
			CGame* pGame = CGame::GetInstance();
			CScene::MODE mode = CManager::GetMode();
			pos = GetPosition();

			// 敵との押し出し判定
			m_info.pCollisionSphere->SetPosition(pos);

			if (pGame != nullptr)
			{
				CGame::STATE state = pGame->GetState();

				if (state != CGame::STATE_ESCAPE && state != CGame::STATE_RESULT)
				{// 脱出状態
					m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_PLAYER);
				}
			}
			else if (mode != CScene::MODE_GAME)
			{
				m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_PLAYER);
			}
			m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_ENEMY);

			m_info.pCollisionSphere->SetPositionOld(m_info.pCollisionSphere->GetPosition());
			m_info.pCollisionSphere->SetPosition(pos);

			SetPosition(pos);
		}
	}

	// 移動量の減衰
	if (m_info.state == STATE_BLOW)
	{
		move.x *= 0.6f;
		move.z *= 0.6f;
	}
	else
	{
		move.x *= 0.1f;
		move.z *= 0.1f;
	}
	move.y -= GRAVITY;
	SetMove(move);

	// 状態管理
	ManageState();

	// モーション管理
	ManageMotion();

	// 武器の追従
	if (m_info.pWeapon != nullptr)
	{
		CMotionDiv *pBody = GetBody();

		if (pBody != nullptr)
		{
			pBody->MultiplyMtx();
		}

		m_info.pWeapon->FollowPlayerHand();
	}

	if (m_info.pArrow != nullptr)
	{// 矢印の追従
		D3DXVECTOR3 rot = GetRot();
		D3DXVECTOR3 posArrow = pos;

		posArrow.y = ARROW_POSY;

		m_info.pArrow->SetPosition(posArrow);
		m_info.pArrow->SetRot(rot);
	}

	// 攻撃判定管理
	ManageAttack();

	// ライフ表示の管理
	ManageLife();

	// 行動範囲
	LimidPostion();

// デバッグ処理
#if _DEBUG
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();

	// プレイヤーを死亡させます
	if (pPlayerManager != nullptr && pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_F6))
		{
			float fDamage = pPlayerManager->GetPlayerParam().fInitialLife;	// プレイヤーの初期体力を取得

			// ダメージ処理
			Hit(fDamage);
		}
	}
#endif // _DEBUG
}

//=====================================================
// 入力処理
//=====================================================
void CPlayer::Input(void)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state == CGame::STATE_ESCAPE || state == CGame::STATE_RESULT || state == CGame::STATE_END)
		{
			return;
		}
	}

	if (m_info.motionInfo.bEmote == false)
	{// エモート中は操作できない
		// 移動処理
		InputMove();

		// 攻撃処理
		InputAttack();

		// エモートの操作
		InputEmote();
	}

	// エイミングの処理
	Aim();
}

//=====================================================
// 移動の入力
//=====================================================
void CPlayer::InputMove(void)
{
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pJoyPad == nullptr || pKeyboard == nullptr)
	{
		return;
	}

	// プレイヤーID取得
	int nId = m_info.nIDJoypad;

	D3DXVECTOR3 vecStickL = 
	{// スティックのベクトル取得
		pJoyPad->GetJoyStickLX(nId),
		pJoyPad->GetJoyStickLY(nId),
		0.0f,
	};

	if (pKeyboard->GetPress(DIK_W))
	{
		vecStickL.y += m_param.fSpeedMove;
	}
	if (pKeyboard->GetPress(DIK_S))
	{
		vecStickL.y -= m_param.fSpeedMove;
	}
	if (pKeyboard->GetPress(DIK_A))
	{
		vecStickL.x -= m_param.fSpeedMove;
	}
	if (pKeyboard->GetPress(DIK_D))
	{
		vecStickL.x += m_param.fSpeedMove;
	}

	D3DXVECTOR3 vecMove =
	{// 移動ベクトルの代入
		vecStickL.x,
		0.0f,
		vecStickL.y,
	};

	// 移動速度の設定
	D3DXVECTOR3 move = GetMove();

	D3DXVec3Normalize(&vecMove, &vecMove);
	vecMove *= m_param.fSpeedMove;

	move += vecMove;

	SetMove(move);
}

//=====================================================
// 攻撃の入力
//=====================================================
void CPlayer::InputAttack(void)
{
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (pJoyPad == nullptr)
	{
		return;
	}

	// パンチの入力
	m_info.motionInfo.bPunch = pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_LB, GetIDJoypad());

	int nMotionUpper = GetMotion(CCharacterDiv::PARTS_UPPER);

	if (nMotionUpper != MOTION_PUNCH)
	{
		if (m_info.pWeapon != nullptr)
		{// 武器の攻撃
			bool bEnable = m_info.pWeapon->IsEnable();

			if (m_info.pItemRepair != nullptr)
			{// 修理部品を手放す
				if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_RB, GetIDJoypad()))
				{
					m_info.pItemRepair->Detatch();

					// 武器を有効化
					m_info.pWeapon->SetEnable(true);
				}
			}

			if (bEnable)
			{
				m_info.pWeapon->Attack();
			}

			
		}
	}
}

//=====================================================
// エモートの操作
//=====================================================
void CPlayer::InputEmote(void)
{
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (pJoyPad == nullptr)
	{
		return;
	}
	int nID = m_info.nIDJoypad;

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_UP, nID))
	{// 上ボタンのモーション
		SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_EMOTE00);
		SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_EMOTE00);

		m_info.motionInfo.bEmote = true;
		m_bSound = false;

		if (m_info.pWeapon != nullptr)
		{
			m_info.pWeapon->SetEnable(false);
		}
	}

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_LEFT, nID))
	{// 左ボタンのモーション
		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(pSound->LABEL_SE_EMOTE_LEFT);

			m_bSound = true;
		}

		SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_EMOTE01);
		SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_EMOTE01);

		m_info.motionInfo.bEmote = true;
		m_bSound = false;

		if (m_info.pWeapon != nullptr)
		{
			m_info.pWeapon->SetEnable(false);
		}
	}

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_RIGHT, nID))
	{// 右ボタンのモーション
		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(pSound->LABEL_SE_EMOTE_RIGHT);

			m_bSound = true;
		}

		SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_EMOTE02);
		SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_EMOTE02);

		m_info.motionInfo.bEmote = true;
		m_bSound = false;

		if (m_info.pWeapon != nullptr)
		{
			m_info.pWeapon->SetEnable(false);
		}
	}

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_DOWN, nID))
	{// 下ボタンの固有エモート
		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(pSound->LABEL_SE_EMOTE_DOWN);

			m_bSound = true;
		}

		SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_EMOTE_UNIQUE);
		SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_EMOTE_UNIQUE);

		m_info.motionInfo.bEmote = true;
		m_bSound = false;

		if (m_info.pWeapon != nullptr)
		{
			m_info.pWeapon->SetEnable(false);
		}
	}
}

//=====================================================
// インタラクトの入力
//=====================================================
bool CPlayer::InputInteract(void)
{
	bool bTrigger = false;

	CInputJoypad* pJoyPad = CInputJoypad::GetInstance();
	int nID = GetIDJoypad();

	if (pJoyPad == nullptr)
	{
		return bTrigger;
	}

	if (pJoyPad->GetTrigger(CInputJoypad::PADBUTTONS_LB, nID))
	{
		bTrigger = true;
	}

	return bTrigger;
}

//=====================================================
// インタラクトの長押し
//=====================================================
bool CPlayer::InputInteractPress(void)
{
	bool bPress = false;

	CInputJoypad* pJoyPad = CInputJoypad::GetInstance();
	int nID = GetIDJoypad();

	if (pJoyPad == nullptr)
	{
		return bPress;
	}

	if (pJoyPad->GetPress(CInputJoypad::PADBUTTONS_LB, nID))
	{
		bPress = true;
	}

	return bPress;
}

//=====================================================
// 武器の有効可
//=====================================================
void CPlayer::EnableWeapon(bool bEnable)
{
	if (m_info.pWeapon != nullptr)
	{
		m_info.pWeapon->SetEnable(bEnable);
	}
}

//=====================================================
// エイミングの処理
//=====================================================
void CPlayer::Aim(void)
{
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();
	
	if (pJoyPad == nullptr)
	{
		return;
	}

	// プレイヤーID取得
	int nId = m_info.nIDJoypad;

	D3DXVECTOR3 vecStickR =
	{// スティックのベクトル取得
		pJoyPad->GetJoyStickRX(nId),
		pJoyPad->GetJoyStickRY(nId),
		0.0f,
	};

	float fLength = D3DXVec3Length(&vecStickR);

	float fAngleDest = 0.0f;

	if (fLength >= 0.3f)
	{// 右スティックを動かしている場合
		// 角度の取得
		fAngleDest = atan2f(vecStickR.x, vecStickR.y);
	}
	else
	{// 移動だけしている場合
		D3DXVECTOR3 move = GetMove();

		float fLengthMove = D3DXVec3Length(&move);

		if (fLengthMove >= 2.0f)
		{
			// 角度の取得
			fAngleDest = atan2f(move.x, move.z);
		}
		else
		{
			return;
		}
	}

	// 角度補正
	D3DXVECTOR3 rot = GetRot();

	universal::LimitRot(&fAngleDest);

	universal::FactingRot(&rot.y, fAngleDest + D3DX_PI, m_param.fSpeedRot);

	SetRot(rot);
}

//=====================================================
// 状態管理
//=====================================================
void CPlayer::ManageState(void)
{
	switch (m_info.state)
	{
	case STATE_NORMAL:
	{// 通常状態

	}
		break;
	case STATE_DAMAGE:
	{// ダメージ状態
		float fTick = CManager::GetTick();

		m_info.fTimerState -= fTick;

		if (m_info.fTimerState <= 0.0f)
		{
			m_info.state = STATE_NORMAL;

			for (int nCutPath = 0; nCutPath < CCharacterDiv::PARTS_MAX; nCutPath++)
			{
				CMotionDiv* pBody = GetBody();

				if (pBody != nullptr)
				{
					pBody->ResetAllCol();
				}
			}
		}
	}
		break;
	case STATE_DEATH:
	{// 死亡状態

	}
		break;
	default:
		break;
	}
}

//=====================================================
// モーション管理
//=====================================================
void CPlayer::ManageMotion(void)
{
	if (GetBody() == nullptr)
	{
		return;
	}
	
	CInputJoypad* pJoyPad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (pJoyPad == nullptr)
	{
		return;
	}

	int nId = m_info.nIDJoypad;	// プレイヤーID取得

	// 移動量
	D3DXVECTOR3 move = GetMove();
	float fSpeed = sqrtf(move.x * move.x + move.z * move.z);

	// 向き
	float fRotPlayer = GetRot().y;
	float fRotMove = atan2f(-move.x, -move.z);
	float fRot = fRotMove - fRotPlayer;
	universal::LimitRot(&fRot);

	// ゲームパッドのスティック
	D3DXVECTOR3 vecStickL =
	{// スティックのベクトル取得
		pJoyPad->GetJoyStickLX(nId),
		pJoyPad->GetJoyStickLY(nId),
		0.0f,
	};
	float fLength = D3DXVec3Length(&vecStickL);
	if (fLength >= 0.1f)
	{
		// 暴走入力防止を設定
		m_info.motionInfo.bRunawayProtect = true;
	}

	// モーションの番号を取得
	int nMotionLower = GetMotion(CCharacterDiv::PARTS_LOWER);
	int nMotionUpper = GetMotion(CCharacterDiv::PARTS_UPPER);

	//CDebugProc::GetInstance()->Print("\nスピード[%f]",fSpeed);

	// 下半身のモーション
	{
		bool bFinish = GetBody()->IsFinish(CCharacterDiv::PARTS_LOWER);

		if (m_info.motionInfo.bEmote)
		{// エモートの他に通らない

		}
		else if (m_info.motionInfo.bDoorPress)
		{// ドア開けるモーション
			if (nMotionLower != MOTION_OPEN_DOOR)
			{
				SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_OPEN_DOOR);
			}
		}
		// 歩きモーション
		else
		{
			// 各方向指定移動
			if (fSpeed > MOVE_LINE)
			{
				if (fRot >= 0.0f && fRot <= (D3DX_PI * 0.25f) ||
					fRot <= 0.0f && fRot >= -(D3DX_PI * 0.25f))
				{// 上
					if (nMotionLower != MOTION_MAGNUM_WALK_FRONT)
					{
						SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_WALK_FRONT);
					}
				}
				else if (fRot >= (D3DX_PI * 0.75f) && fRot <= D3DX_PI ||
					fRot <= -(D3DX_PI * 0.75f) && fRot >= -D3DX_PI)
				{// 下
					if (nMotionLower != MOTION_MAGNUM_WALK_BACK)
					{
						SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_WALK_BACK);
					}
				}
				else if (fRot >= (D3DX_PI * 0.25f) && fRot <= (D3DX_PI * 0.75f))
				{// 右
					if (nMotionLower != MOTION_MAGNUM_WALK_RIGHT)
					{
						SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_WALK_RIGHT);
					}
				}
				else if (fRot <= -(D3DX_PI * 0.25f) && fRot >= -(D3DX_PI * 0.75f))
				{// 左
					if (nMotionLower != MOTION_MAGNUM_WALK_LEFT)
					{
						SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_WALK_LEFT);
					}
				}
			}
			else
			{
				if (nMotionLower != MOTION_MAGNUM_NEUTRAL)
				{// 待機
					SetMotion(CCharacterDiv::PARTS_LOWER, MOTION_MAGNUM_NEUTRAL);
				}
			}
		}
	}

	// 上半身のモーション
	{
		bool bFinish = GetBody()->IsFinish(CCharacterDiv::PARTS_UPPER);
		int nKey = GetBody()->GetKey(CCharacterDiv::PARTS_UPPER);
		int nNumKey = GetBody()->GetMotionInfo(CCharacterDiv::PARTS_UPPER, nMotionUpper).nNumKey;

		if (m_info.motionInfo.bEmote)
		{// エモートの他に通らない
			if (bFinish)
			{
				m_info.motionInfo.bEmote = false;

				if (m_info.pWeapon != nullptr)
				{
					m_info.pWeapon->SetEnable(true);
				}
			}
		}
		// ドア開けるモーション
		else if (m_info.motionInfo.bDoorPress)
		{
			if (nMotionUpper != MOTION_OPEN_DOOR)
			{
				SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_OPEN_DOOR);
			}
		}
		// 物拾う
		else if (m_info.motionInfo.bItemTrigger ||
				 nMotionUpper == MOTION_ITEM_PICK_UP && bFinish == false && nKey != nNumKey)
		{
			if (nMotionUpper != MOTION_ITEM_PICK_UP)
			{
				SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_PICK_UP);
			}
		}
		// 所有修理アイテム
		else if (m_info.pItemRepair != nullptr)
		{
			// 各方向指定移動
			if (fSpeed > MOVE_LINE)
			{
				if (fRot >= 0.0f && fRot <= (D3DX_PI * 0.25f) ||
					fRot <= 0.0f && fRot >= -(D3DX_PI * 0.25f) ||
					m_info.motionInfo.bRunawayProtect)
				{// 上
					if (nMotionLower != MOTION_ITEM_WALK_FRONT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_WALK_FRONT);
					}
				}
				else if (fRot >= (D3DX_PI * 0.75f) && fRot <= D3DX_PI ||
					fRot <= -(D3DX_PI * 0.75f) && fRot >= -D3DX_PI)
				{// 下
					if (nMotionLower != MOTION_ITEM_WALK_BACK)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_WALK_BACK);
					}
				}
				else if (fRot >= (D3DX_PI * 0.25f) && fRot <= (D3DX_PI * 0.75f))
				{// 右
					if (nMotionLower != MOTION_ITEM_WALK_RIGHT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_WALK_RIGHT);
					}
				}
				else if (fRot <= -(D3DX_PI * 0.25f) && fRot >= -(D3DX_PI * 0.75f))
				{// 左
					if (nMotionLower != MOTION_ITEM_WALK_LEFT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_WALK_LEFT);
					}
				}
			}
			else
			{
				// 待機
				if (nMotionUpper != MOTION_ITEM_NEUTRAL)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_ITEM_NEUTRAL);
				}
			}
		}
		else if (m_info.motionInfo.bPunch || (nMotionUpper == MOTION_PUNCH && bFinish == false))
		{// パンチのモーション
			if (nMotionUpper != MOTION_PUNCH)
			{
				SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_PUNCH);

				if (pSound != nullptr)
				{
					pSound->Play(pSound->LABEL_SE_SWING);
				}
			}
		}
		// 所有武器
		else if (m_info.pWeapon != nullptr)
		{
			if (m_info.motionInfo.bEmote == false)
			{
				m_info.pWeapon->SetEnable(true);
			}

			switch (m_info.pWeapon->GetType())
			{
				// マグナム
			case CWeapon::TYPE_MAGNUM:

				if (nMotionUpper != MOTION_MAGNUM_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_MAGNUM_ATTACK);
				}

				break;

				// マシンガン
			case CWeapon::TYPE_MACHINEGUN:

				if (nMotionUpper != MOTION_MAGNUM_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_MAGNUM_ATTACK);
				}

				break;

				// ショットガン
			case CWeapon::TYPE_SHOTGUN:

				if (nMotionUpper != MOTION_RIFLE_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_RIFLE_ATTACK);
				}

				break;
				// レールガン
			case CWeapon::TYPE_RAILGUN:

				if (nMotionUpper != MOTION_RIFLE_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_RIFLE_ATTACK);
				}

				break;
				// ミニガン
			case CWeapon::TYPE_MINIGUN:

				if (nMotionUpper != MOTION_RIFLE_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_RIFLE_ATTACK);
				}

				break;
				// ランチャー
			case CWeapon::TYPE_LAUNCHER:

				if (nMotionUpper != MOTION_RIFLE_ATTACK)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_RIFLE_ATTACK);
				}

				break;
			}
		}
		// 何もなし
		else
		{
			// 各方向指定移動
			if (fSpeed > MOVE_LINE)
			{
				if (fRot >= 0.0f && fRot <= (D3DX_PI * 0.25f) ||
					fRot <= 0.0f && fRot >= -(D3DX_PI * 0.25f))
				{// 上
					if (nMotionUpper != MOTION_WALK_FRONT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_WALK_FRONT);
					}
				}
				else if (fRot >= (D3DX_PI * 0.75f) && fRot <= D3DX_PI ||
						 fRot <= -(D3DX_PI * 0.75f) && fRot >= -D3DX_PI)
				{// 下
					if (nMotionUpper != MOTION_WALK_BACK)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_WALK_BACK);
					}
				}
				else if (fRot >= (D3DX_PI * 0.25f) && fRot <= (D3DX_PI * 0.75f))
				{// 右
					if (nMotionUpper != MOTION_WALK_RIGHT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_WALK_RIGHT);
					}
				}
				else if (fRot <= -(D3DX_PI * 0.25f) && fRot >= -(D3DX_PI * 0.75f))
				{// 左
					if (nMotionUpper != MOTION_WALK_LEFT)
					{
						SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_WALK_LEFT);
					}
				}
			}
			else
			{
				// 待機
				if (nMotionUpper != MOTION_NEUTRAL)
				{
					SetMotion(CCharacterDiv::PARTS_UPPER, MOTION_NEUTRAL);
				}
			}
		}
	}

	// モーション情報の初期化
	m_info.motionInfo.bDoorPress = false;			// ドアへの入力情報
	m_info.motionInfo.bItemTrigger = false;			// 物への入力情報
	m_info.motionInfo.bRunawayProtect = false;		// 暴走入力情報
}

//=====================================================
// 攻撃判定の管理
//=====================================================
void CPlayer::ManageAttack(void)
{
	if (m_info.pClsnAttack == nullptr)
	{// 判定のエラー
		return;
	}

	CMotionDiv *pBody = GetBody();

	if (pBody == nullptr)
	{// 判定のエラー
		return;
	}

	// 戦績の取得処理
	CRecord* pRecord = CRecord::GetInstance();

	for (int i = 0; i < m_info.nNumAttack; i++)
	{
		if (pBody->GetMotion(CCharacterDiv::PARTS_UPPER) == m_info.pAttackInfo[i].nIdxMotion)
		{// 攻撃モーション中の判定
			int nFrame = pBody->GetFrame(CCharacterDiv::PARTS_UPPER);
			int nKey = pBody->GetKey(CCharacterDiv::PARTS_UPPER);
			D3DXVECTOR3 pos;

			if (nFrame == m_info.pAttackInfo[i].nFrame && nKey == m_info.pAttackInfo[i].nKey)
			{// 当たり判定の設定
				bool bHit = false;
				D3DXMATRIX mtx;
				D3DXMATRIX mtxPart = *pBody->GetMatrix();

				universal::SetOffSet(&mtx, mtxPart, m_info.pAttackInfo[i].pos);

				pos =
				{
					mtx._41,
					mtx._42,
					mtx._43
				};

				// 位置設定
				m_info.pClsnAttack->SetPosition(pos);

				// 半径の設定
				m_info.pClsnAttack->SetRadius(m_info.pAttackInfo[i].fRadius);

#ifdef _DEBUG
				CEffect3D::Create(pos, m_info.pAttackInfo[i].fRadius, 10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
				// 敵との判定
				bHit = m_info.pClsnAttack->OnEnter(CCollision::TAG_ENEMY);

				// 命中したオブジェクトの取得
				CObject *pObj = m_info.pClsnAttack->GetOther();

				if (bHit == true && pObj != nullptr)
				{// 敵のヒット処理
					// アニメーションエフェクトの生成
					CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

					if (pAnimManager != nullptr)
					{
						pAnimManager->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_HIT00);
					}

					// 敵をノックバックさせる
					BlowEnemy(pObj);

					// ヒット処理
					pObj->Hit(m_param.fDamagePunch);

					// 破壊数の戦績加算処理
					if (pRecord != nullptr)
					{
						pRecord->CheckDeathEnemy(pObj, GetID());
					}
				}

				// 木箱との判定
				bHit = m_info.pClsnAttack->OnEnter(CCollision::TAG_BOX);

				pObj = m_info.pClsnAttack->GetOther();

				if (bHit == true && pObj != nullptr)
				{// 木箱のヒット処理
					pObj->Hit(0.0f);
				}

				// プレイヤーとの判定
				bHit = m_info.pClsnAttack->OnEnter(CCollision::TAG_PLAYER);

				pObj = m_info.pClsnAttack->GetOther();

				if (bHit == true && pObj != nullptr)
				{// プレイヤーを吹き飛ばす
					// アニメーションエフェクトの生成
					CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

					// 破壊数の戦績加算処理
					if (pRecord != nullptr)
					{
						pRecord->AddMedman(GetID());
					}

					if (pAnimManager != nullptr)
					{
						pAnimManager->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_HIT00);
					}

					BlowPlayer(pObj);
				}
			}
		}
	}
}

//=====================================================
// ライフ表示の管理
//=====================================================
void CPlayer::ManageLife(void)
{
	if (m_info.pLife == nullptr || m_info.pLifeFrame == nullptr)
	{
		return;
	}

	// 位置の設定
	D3DXVECTOR3 pos = GetPosition();
	pos += POS_LIFE;

	m_info.pLife->SetPosition(pos);
	m_info.pLifeFrame->SetPosition(pos);

	// 色の設定
	D3DXCOLOR col = m_info.pLife->GetColor();

	col.a -= SPEED_LIFE_FADE;

	m_info.pLife->SetColor(col);
	m_info.pLifeFrame->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, col.a));

	// サイズ調整
	ResizeLife();

	if (col.a <= 0.0f)
	{// 透明になったら削除
		DeleteLife();
	}
}

//=====================================================
// ライフ表示のサイズ調整
//=====================================================
void CPlayer::ResizeLife(void)
{
	if (m_info.pLife == nullptr)
	{
		return;
	}

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr)
	{
		return;
	}

	float fInitialLife = pPlayerManager->GetPlayerParam().fInitialLife;

	float fRate = m_info.fLife / fInitialLife;

	m_info.pLife->SetSize(SIZE_LIFE * fRate, SIZE_LIFE * fRate);
}

//=====================================================
// 敵を吹き飛ばす処理
//=====================================================
void CPlayer::BlowEnemy(CObject *pObj)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr || pObj == nullptr)
	{
		return;
	}

	CEnemy *pEnemy = pEnemyManager->GetHead();

	while (pEnemy != nullptr)
	{
		CEnemy *pEnemyNext = pEnemy->GetNext();

		if ((CObject*)pEnemy == pObj)
		{
			// 敵とプレイヤーの位置の差分を取得
			D3DXVECTOR3 posEnemy = pEnemy->GetPosition();
			D3DXVECTOR3 moveEnemy = pEnemy->GetMove();
			D3DXVECTOR3 posPlayer = GetPosition();
			D3DXVECTOR3 vecDiff = posEnemy - posPlayer;

			// 差分ベクトルをふきとばす力に変換
			D3DXVECTOR3 vecBlow;

			D3DXVec3Normalize(&vecBlow, &vecDiff);

			vecBlow *= m_param.fPowBlow;

			// 吹き飛ばしベクトルを移動量に加算
			moveEnemy += vecBlow;

			pEnemy->SetMove(moveEnemy);
		}

		pEnemy = pEnemyNext;
	}
}

//=====================================================
// プレイヤーを吹き飛ばす処理
//=====================================================
void CPlayer::BlowPlayer(CObject *pObj)
{
	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr || pObj == nullptr)
	{
		return;
	}

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			if ((CObject*)pPlayer == pObj)
			{
				// 相手と自分の位置の差分を取得
				D3DXVECTOR3 posOther = pPlayer->GetPosition();
				D3DXVECTOR3 moveOther = pPlayer->GetMove();
				D3DXVECTOR3 posOwn = GetPosition();
				D3DXVECTOR3 vecDiff = posOther - posOwn;

				// 差分ベクトルをふきとばす力に変換
				D3DXVECTOR3 vecBlow;

				D3DXVec3Normalize(&vecBlow, &vecDiff);

				vecBlow *= m_param.fPowBlow;
				vecBlow.y = POW_PUNCH_UP;

				// 吹き飛ばしベクトルを移動量に加算
				moveOther += vecBlow;

				pPlayer->SetMove(moveOther);

				pPlayer->SetState(STATE_BLOW);
			}
		}
	}
}

//=====================================================
// ライフの表示
//=====================================================
void CPlayer::DispLife(void)
{
	D3DXVECTOR3 pos = GetPosition();
	pos += POS_LIFE;

	if (m_info.pLifeFrame == nullptr)
	{
		m_info.pLifeFrame = CObject3D::Create(pos);

		if (m_info.pLifeFrame != nullptr)
		{
			m_info.pLifeFrame->EnableZtest(true);
			m_info.pLifeFrame->EnableBillboard(true);
			m_info.pLifeFrame->SetSize(SIZE_LIFE, SIZE_LIFE);
			m_info.pLifeFrame->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			m_info.pLifeFrame->SetPosition(pos);

			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\life000.png");
			m_info.pLifeFrame->SetIdxTexture(nIdx);
		}
	}

	if (m_info.pLife == nullptr)
	{
		m_info.pLife = CObject3D::Create(pos);

		if (m_info.pLife != nullptr)
		{
			m_info.pLife->EnableZtest(true);
			m_info.pLife->EnableBillboard(true);
			m_info.pLife->SetSize(SIZE_LIFE, SIZE_LIFE);
			m_info.pLife->SetPosition(pos);

			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\life000.png");
			m_info.pLife->SetIdxTexture(nIdx);

			// 初期サイズ調整
			ResizeLife();
		}
	}
}

//=====================================================
// ライフ表示の削除
//=====================================================
void CPlayer::DeleteLife(void)
{
	if (m_info.pLife != nullptr)
	{
		m_info.pLife->Uninit();
		m_info.pLife = nullptr;
	}

	if (m_info.pLifeFrame != nullptr)
	{
		m_info.pLifeFrame->Uninit();
		m_info.pLifeFrame = nullptr;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CPlayer::Draw(void)
{
	// 継承クラスの描画
	CCharacterDiv::Draw();

	// デバッグ表示
	Debug();
}

//=====================================================
// 位置設定
//=====================================================
void CPlayer::SetPosition(D3DXVECTOR3 pos)
{
	CCharacterDiv::SetPosition(pos);

	if (m_info.pArrow != nullptr)
	{// 矢印の追従
		D3DXVECTOR3 rot = GetRot();
		D3DXVECTOR3 posArrow = pos;

		posArrow.y = ARROW_POSY;

		m_info.pArrow->SetPosition(posArrow);
		m_info.pArrow->SetRot(rot);
	}
}

//=====================================================
// 武器設定
//=====================================================
void CPlayer::SetWeapon(CWeapon::TYPE type)
{
	if (m_info.pWeapon != nullptr)
	{
		m_info.pWeapon->Uninit();
		m_info.pWeapon = nullptr;
	}

	m_info.pWeapon = CWeapon::Create(type, HAND_PARTS_NUM);

	if (m_info.pWeapon != nullptr)
	{
		m_info.pWeapon->SetPlayer(this);
	}
}

//=====================================================
// ヒット処理
//=====================================================
void CPlayer::Hit(float fDamage)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		CGame::STATE state = pGame->GetState();

		if (state != CGame::STATE::STATE_NORMAL)
		{
			return;
		}
	}

	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();

	if (m_info.state == STATE_NORMAL)
	{
		m_info.fLife -= fDamage;

		if (m_info.fLife <= 0.0f)
		{// 死亡判定
			m_info.fLife = 0.0f;

			m_info.state = STATE_DEATH;

			// 死亡したプレイヤーの番号を渡す
			if (pPlayerManager != nullptr)
			{
				pPlayerManager->SetDeathPlayer(m_info.nID, true);
			}

			Uninit();

			int nIdx = GetID();

			// 幽霊の生成
			CGhost *pGhost = CGhost::Create(nIdx);

			if (pGhost != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();

				pGhost->SetPosition(pos);
			}
		}
		else
		{// ダメージ判定
			m_info.state = STATE_DAMAGE;

			CSound* pSound = CSound::GetInstance();

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_DAMAGE);
			}

			m_info.fTimerState = m_param.fTimeDamage;

			for (int nCutPath = 0; nCutPath < CCharacterDiv::PARTS_MAX; nCutPath++)
			{
				CMotionDiv* pBody = GetBody();

				if (pBody != nullptr)
				{
					pBody->SetAllCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}
			}

			// コントローラーの振動
			CInputJoypad *pJoypad = CInputJoypad::GetInstance();

			if (pJoypad != nullptr)
			{
				int nIdxJoypad = GetIDJoypad();

				pJoypad->Vibration(nIdxJoypad,CInputJoypad::PADVIB::PADVIB_USE,1.0f,20);
			}

			// 残りライフの表示
			DispLife();
		}
	}
}

//=====================================================
// プレイヤーの移動制限
//=====================================================
void CPlayer::LimidPostion(void)
{
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		CPlayer* pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			D3DXVECTOR3 pos = pPlayer->GetPosition();

			if (pos.x <= LIMID_RANGE_LEFT)
			{
				pos.x = LIMID_RANGE_LEFT;
			}
			else if (pos.x >= LIMID_RANGE_RIGHT)
			{
				pos.x = LIMID_RANGE_RIGHT;
			}
			else if (pos.z >= LIMID_RANGE_UP)
			{
				pos.z = LIMID_RANGE_UP;
			}
			else if (pos.z <= LIMID_RANGE_DOWN)
			{
				pos.z = LIMID_RANGE_DOWN;
			}

			pPlayer->SetPosition(pos);
		}
	}
}

//=====================================================
// ロケットに乗り込む
//=====================================================
void CPlayer::BoardingRocket(void)
{
	// インスタンスを取得
	CGame* pGame = CGame::GetInstance();
	CRocket* pRocket = CRocket::GetInstance();
	CGoal* pGoal = CGoal::GetInstance();

	if (pGoal != nullptr)
	{// ゴール内にプレイヤーがいるか
		// 座標の差分から距離を計算
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posGoal = pGoal->GetPosition();
		D3DXVECTOR3 vecDiff = pos - posGoal;
		float fDiff = D3DXVec3Length(&vecDiff);
		float fRadius = pGoal->GetRadius();

		if (fDiff < fRadius)
		{
			if (pGame != nullptr)
			{
				CGame::STATE state = pGame->GetState();

				if (state == CGame::STATE_ESCAPE)
				{// 脱出状態
					if (pRocket != nullptr)
					{
						// 移動量の設定
						D3DXVECTOR3 posRocket = pRocket->GetPosition();
						D3DXVECTOR3 vecDiff = posRocket - pos;

						pos.x += vecDiff.x * 0.2f;
						pos.z += vecDiff.z * 0.2f;

						D3DXVec3Normalize(&vecDiff, &vecDiff);

						vecDiff *= 0.08f;

						SetPosition(pos);

						// 向きを目標方向に補正
						float fAngleDist = atan2f(vecDiff.x, vecDiff.z);
						D3DXVECTOR3 rot = GetRot();

						fAngleDist += D3DX_PI;

						universal::FactingRot(&rot.y, fAngleDist, 0.1f);

						SetRot(rot);

						D3DXVECTOR3 vecDiffDelete = pos - posRocket;
						float fLenth = D3DXVec3Length(&vecDiffDelete);

						if (fLenth < 20.0f)
						{// ロケットに近い
							SetPosition({ pos.x, -100.0f, pos.z });		// 埋める
						}

						if(m_bGoalJump == false)
						{// ジャンプさせる
							pos.y = posRocket.y + 250.0f - pos.y;
							SetPosition(pos);
							D3DXVECTOR3 move = GetMove();
							SetMove({ move.x, 0.0f, move.z });
							m_bGoalJump = true;
						}
					}

					if (m_info.pArrow != nullptr)
					{// 矢印の削除
						m_info.pArrow->Uninit();
						m_info.pArrow = nullptr;
					}
				}
			}
		}
	}
}

//=====================================================
// デバッグ表示
//=====================================================
void CPlayer::Debug(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\nプレイヤー番号[%d]", m_info.nID);
	pDebugProc->Print("\nプレイヤー状態カウンタ[%f]", m_info.fTimerState);
	pDebugProc->Print("\nプレイヤーの位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\nプレイヤーの移動量[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);

	if (GetBody() != nullptr)
	{
		int nMotion = GetBody()->GetMotion(CCharacterDiv::PARTS_LOWER);

		pDebugProc->Print("\n下半身のモーション[%d]", nMotion);
	}

	if (GetBody() != nullptr)
	{
		int nMotion = GetBody()->GetMotion(CCharacterDiv::PARTS_UPPER);

		pDebugProc->Print("\n上半身のモーション[%d]", nMotion);
	}
}