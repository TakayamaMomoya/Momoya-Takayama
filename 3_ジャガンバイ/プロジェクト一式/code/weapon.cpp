//*****************************************************
//
// 武器の処理[weapon.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "weapon.h"
#include "weaponMagnum.h"
#include "weaponMachinegun.h"
#include "weaponShotgun.h"
#include "weaponRailgun.h"
#include "weaponMinigun.h"
#include "weaponLauncher.h"
#include "weaponManager.h"
#include "inputjoypad.h"
#include "motion.h"
#include "player.h"
#include "universal.h"
#include "debugproc.h"
#include "sound.h"
#include "animEffect3D.h"
#include "anim3D.h"
#include "particle.h"

#include "motionDiv.h"

//=====================================================
// コンストラクタ
//=====================================================
CWeapon::CWeapon(int nPriority) : CObjectX(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CWeapon::~CWeapon()
{

}

//=====================================================
// 生成処理
//=====================================================
CWeapon *CWeapon::Create(CWeapon::TYPE type, int nIdxhand)
{
	CWeapon *pWeapon = nullptr;
	CSound* pSound = CSound::GetInstance();

	if (pWeapon == nullptr)
	{
		switch (type)
		{
		case CWeapon::TYPE_MAGNUM:
			// マグナムの生成
			pWeapon = new CMagnum;
			
			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_GET_MAGNUM);
			}

			break;
		case CWeapon::TYPE_MACHINEGUN:
			// マシンガンの生成
			pWeapon = new CMachinegun;

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_GET_MACHINEGUN);
			}

			break;
		case CWeapon::TYPE_SHOTGUN:
			// ショットガンの生成
			pWeapon = new CShotgun;

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_GET_SHOTGUN);
			}

			break;
		case CWeapon::TYPE_RAILGUN:
			// レールガンの生成
			pWeapon = new CRailgun;

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_GET_RAILGUN);
			}

			break;
		case CWeapon::TYPE_MINIGUN:
			// ミニガンの生成
			pWeapon = new CMinigun;

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_GET_MINIGUN);
			}

			break;
		case CWeapon::TYPE_LAUNCHER:
			// ランチャーの生成
			pWeapon = new CLauncher;

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_GET_LUNCHER);
			}

			break;
		default:
			break;
		}

		if (pWeapon != nullptr)
		{
			pWeapon->m_info.nIdxHand = nIdxhand;
			pWeapon->m_info.type = type;

			// 初期化
			pWeapon->Init();

			// モデルの読み込み
			char* apPath[CWeapon::TYPE_MAX] =
			{
				"data\\MODEL\\weapon\\magnum.x",
				"data\\MODEL\\weapon\\mac10.x",
				"data\\MODEL\\weapon\\shotgun.x",
				"data\\MODEL\\weapon\\railgun.x",
				"data\\MODEL\\weapon\\minigun_000.x",
				"data\\MODEL\\weapon\\launcher.x",
			};

			int nIdx = CModel::Load(apPath[type]);
			pWeapon->SetIdxModel(nIdx);
			pWeapon->BindModel(nIdx);
		}
	}

	return pWeapon;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CWeapon::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	// 基底パラメーター取得
	CWeaponManager* pWeaponManager = CWeaponManager::GetInstance();

	if (pWeaponManager != nullptr)
	{
		CWeapon::SInfo info = pWeaponManager->GetBaseInfo(m_info.type);

		SetMaxBullet(info.nMaxBullet);
		SetRapid(info.nRapid);
		SetDamage(info.fDamage);
		SetBulletLife(info.fLifeBullet);
		SetVibPower(info.fVibPower);
		SetVibTime(info.nVibTime);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CWeapon::Uninit(void)
{
	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CWeapon::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();

	if (m_info.nCntShot > 0)
	{
		m_info.nCntShot--;
	}
}

//=====================================================
// プレイヤーの手を追従
//=====================================================
void CWeapon::FollowPlayerHand(void)
{
	if (m_info.pPlayer == nullptr)
	{
		return;
	}

	CMotionDiv *pBody = m_info.pPlayer->GetBody();

	if (pBody != nullptr)
	{
		if (m_info.bEnable)
		{// 手に追従
			CParts *pParts = pBody->GetParts(CCharacterDiv::PARTS_UPPER,m_info.nIdxHand)->pParts;

			if (pParts != nullptr)
			{
				D3DXMATRIX *pMtx = GetMatrix();
				D3DXMATRIX *pMtxPart = pParts->GetMatrix();
				D3DXVECTOR3 offset = { -10.0f,0.0f,0.0f };

				universal::SetOffSet(pMtx, *pMtxPart, offset);
			}
		}
		else
		{// 背中に追従
			CParts *pParts = pBody->GetParts(CCharacterDiv::PARTS_UPPER,0)->pParts;

			if (pParts != nullptr)
			{
				D3DXMATRIX *pMtx = GetMatrix();
				D3DXMATRIX *pMtxPart = pParts->GetMatrix();
				D3DXVECTOR3 offset = { 0.0f,0.0f,20.0f };
				D3DXVECTOR3 rot = { 0.0f,0.0f,D3DX_PI * 0.3f };

				universal::SetOffSet(pMtx, *pMtxPart, offset, rot);
			}
		}
	}
}

//=====================================================
// 有効化の切り替え
//=====================================================
void CWeapon::SetEnable(bool bEnable)
{
	m_info.bEnable = bEnable;

	ToggleEnable(bEnable);
}

//=====================================================
// 描画処理
//=====================================================
void CWeapon::Draw(void)
{
	// 継承クラスの描画
	CObjectX::JustDraw();

#ifdef _DEBUG
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\n最大弾数[%d]", m_info.nMaxBullet);
	pDebugProc->Print("\n弾数[%d]", m_info.nNumBullet);
	pDebugProc->Print("\n使用可能[%d]", m_info.bEnable);
	pDebugProc->Print("\n威力[%f]", m_info.fDamage);
#endif
}

//=====================================================
// プレイヤー設定
//=====================================================
void CWeapon::SetPlayer(CPlayer *pPlayer)
{
	m_info.pPlayer = pPlayer;

	if (m_info.pPlayer != nullptr)
	{
		m_info.nIdxJoypad = pPlayer->GetIDJoypad();
	}

	m_info.bEnable = true;
}

//=====================================================
// 弾数設定
//=====================================================
void CWeapon::SetBullet(int nBullet)
{
	m_info.nNumBullet = nBullet;

	if (m_info.nNumBullet <= 0)
	{// 弾切れした場合、破棄
		Break();
	}
	else if (m_info.nNumBullet > m_info.nMaxBullet)
	{// 最大数を越えた場合の補正
		m_info.nNumBullet = m_info.nMaxBullet;
	}
}

//=====================================================
// 壊れる処理
//=====================================================
void CWeapon::Break(void)
{
	CSound* pSound = CSound::GetInstance();

	// プレイヤーから破棄
	if (m_info.pPlayer != nullptr)
	{
		m_info.pPlayer->ReleaseWeapon();
	}

	// サウンド再生
	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_SE_WEAPON_LOST);
	}

	// コントローラー振動
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();

	if (pJoypad != nullptr)
	{
		int nIdx = m_info.pPlayer->GetIDJoypad();

		pJoypad->Vibration(nIdx, CInputJoypad::PADVIB_USE, 1.0f, 60);
	}

	// アニメーションエフェクトの生成
	D3DXMATRIX mtx = *GetMatrix();
	D3DXVECTOR3 pos = { mtx._41,mtx._42,mtx._43 };

	CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

	if (pAnimManager != nullptr)
	{
		CAnim3D *pAnim3D = pAnimManager->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_HIT00);

		if (pAnim3D != nullptr)
		{
			pAnim3D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	// パーティクルの生成
	CParticle::Create(pos, CParticle::TYPE::TYPE_BREAK_WEAPON);

	Uninit();
}

//=====================================================
// 最大弾数設定
//=====================================================
void CWeapon::SetMaxBullet(int nBullet)
{
	m_info.nMaxBullet = nBullet;
	m_info.nNumBullet = nBullet;
}