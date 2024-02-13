//*****************************************************
//
// マグナムの処理[MagnumMagnum.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "weaponMagnum.h"
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
CMagnum::CMagnum(int nPriority) : CWeapon(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CMagnum::~CMagnum()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMagnum::Init(void)
{
	// 継承クラスの初期化
	CWeapon::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMagnum::Uninit(void)
{
	// 継承クラスの終了
	CWeapon::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CMagnum::Update(void)
{
	// 継承クラスの更新
	CWeapon::Update();
}

//=====================================================
// 攻撃処理
//=====================================================
void CMagnum::Attack(void)
{
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();

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
			universal::SetOffSet(&mtxMuzzle, *pMtx, D3DXVECTOR3(-50.0f, 20.0f, 0.0f));

			D3DXVECTOR3 posMuzzle =
			{
				mtxMuzzle._41,
				mtxMuzzle._42,
				mtxMuzzle._43,
			};

			D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

			CPlayer* pPlayer = GetPlayer();

			if (pPlayer != nullptr)
			{// プレイヤーの向きに移動量を設定
				D3DXVECTOR3 rot = pPlayer->GetRot();

				int nRange = universal::RandRange(10, -10);

				float fRand = nRange * 0.01f;

				rot.y += fRand;

				move =
				{
					sinf(rot.y) * BULLET_SPEED,
					0.0f,
					cosf(rot.y) * BULLET_SPEED,
				};
			}

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

			CSound* pSound = CSound::GetInstance();

			if (pSound != nullptr)
			{
				// マグナム発砲音
				pSound->Play(pSound->LABEL_SE_GUNSHOT_00);
			}

			if (pJoypad != nullptr)
			{// コントローラーの振動
				pJoypad->Vibration(nID, CInputJoypad::PADVIB::PADVIB_USE, info.fVibPower, info.nVibTime);
			}

			// 弾を減らす
			nBullet--;
			SetBullet(nBullet);

			// 連射カウンターのリセット
			nCntShot = GetRapid();

			SetCntShot(nCntShot);

			// エフェクトの生成
			CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

			if (pAnim3D != nullptr)
			{
				pAnim3D->CreateEffect(posMuzzle, CAnimEffect3D::TYPE::TYPE_MUZZLEFLUSH);
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
void CMagnum::Draw(void)
{
	// 継承クラスの描画
	CWeapon::Draw();
}