//*****************************************************
//
// ショットガンの処理[weaponShotgun.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "weaponShotgun.h"
#include "inputjoypad.h"
#include "bullet.h"
#include "player.h"
#include "sound.h"
#include "animEffect3D.h"
#include "universal.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define BULLET_SPEED	(10.0f)	// 弾の速度

//=====================================================
// コンストラクタ
//=====================================================
CShotgun::CShotgun(int nPriority) : CWeapon(nPriority)
{
	m_fAngleDiffuse = 0.0f;
	m_nNumPellet = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CShotgun::~CShotgun()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CShotgun::Init(void)
{
	// 継承クラスの初期化
	CWeapon::Init();

	// ショットガンの情報入手
	CWeaponManager *pWeaponManager = CWeaponManager::GetInstance();

	if (pWeaponManager != nullptr)
	{
		CWeaponManager::SInfoShotgun info = pWeaponManager->GetShotgunInfo();

		m_nNumPellet = info.nNumPellet;
		m_fAngleDiffuse = info.fAngleDiffuse;
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CShotgun::Uninit(void)
{
	// 継承クラスの終了
	CWeapon::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CShotgun::Update(void)
{
	// 継承クラスの更新
	CWeapon::Update();
}

//=====================================================
// 攻撃処理
//=====================================================
void CShotgun::Attack(void)
{
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();
	
	if (pJoypad == nullptr)
	{
		return;
	}

	int nBullet = GetBullet();
	int nID = GetID();

	if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB,nID))
	{// 射撃
		int nCntShot = GetCntShot();

		if (nBullet > 0 && nCntShot == 0)
		{// 弾の発射
			D3DXMATRIX* pMtx = GetMatrix();

			// マズルの位置を設定
			D3DXMATRIX mtxMuzzle;
			universal::SetOffSet(&mtxMuzzle, *pMtx, D3DXVECTOR3(-60.0f, 5.0f, 0.0f));

			D3DXVECTOR3 posMuzzle =
			{
				mtxMuzzle._41,
				mtxMuzzle._42,
				mtxMuzzle._43,
			};

			D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

			CPlayer* pPlayer = GetPlayer();

			if (pPlayer != nullptr)
			{
				for (int i = 0; i < m_nNumPellet; i++)
				{// 一度に設定された分の弾を発射
					D3DXVECTOR3 rot = pPlayer->GetRot();

					// 移動角度をずらす
					int nRange = universal::RandRange((int)(m_fAngleDiffuse * 0.5f), (int)(-m_fAngleDiffuse * 0.5f));

					float fRand = D3DXToRadian((float)nRange);

					rot.y += fRand;

					move =
					{
						sinf(rot.y) * BULLET_SPEED,
						0.0f,
						cosf(rot.y) * BULLET_SPEED,
					};

					// パラメーター取得
					CWeapon::SInfo info = GetInfo();

					// 弾を発射
					CBullet* pBullet = CBullet::Create(posMuzzle, -move, info.fLifeBullet, CBullet::TYPE_PLAYER, false, 2.0f, info.fDamage);

					// 弾のプレイヤー番号設定処理
					if (pBullet != nullptr)
					{
						CPlayer* pPlayer = GetPlayer();

						if (pPlayer != nullptr)
						{
							pBullet->SetIdxPlayer(pPlayer->GetID());
						}
					}

					if (pSound != nullptr)
					{
						// ショットガン発砲音
						pSound->Play(pSound->LABEL_SE_GUNSHOT_02);
					}

					if (pJoypad != nullptr)
					{// コントローラーの振動
						pJoypad->Vibration(nID, CInputJoypad::PADVIB::PADVIB_USE, info.fVibPower, info.nVibTime);
					}

					// 連射カウンターのリセット
					nCntShot = GetRapid();

					SetCntShot(nCntShot);
				}

				// エフェクトの生成
				CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

				if (pAnim3D != nullptr)
				{
					pAnim3D->CreateEffect(posMuzzle, CAnimEffect3D::TYPE::TYPE_MUZZLEFLUSH);
				}

				// 弾を減らす
				nBullet--;
				SetBullet(nBullet);
			}
		}
		else
		{// 弾切れの場合

		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CShotgun::Draw(void)
{
	// 継承クラスの描画
	CWeapon::Draw();
}