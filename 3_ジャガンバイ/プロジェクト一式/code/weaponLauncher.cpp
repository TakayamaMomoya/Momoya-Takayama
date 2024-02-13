//*****************************************************
//
// ランチャーの処理[weaponLauncher.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "weaponLauncher.h"
#include "inputjoypad.h"
#include "missile.h"
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
CLauncher::CLauncher(int nPriority) : CWeapon(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CLauncher::~CLauncher()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CLauncher::Init(void)
{
	// 継承クラスの初期化
	CWeapon::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CLauncher::Uninit(void)
{
	// 継承クラスの終了
	CWeapon::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CLauncher::Update(void)
{
	// 継承クラスの更新
	CWeapon::Update();
}

//=====================================================
// 攻撃処理
//=====================================================
void CLauncher::Attack(void)
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
			CWeapon::SInfo info = GetInfo();

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_SE_GUNSHOT_04);
			}

			if (pJoypad != nullptr)
			{// コントローラーの振動
				pJoypad->Vibration(nID, CInputJoypad::PADVIB::PADVIB_USE, info.fVibPower, info.nVibTime);
			}

			// マズルの位置を設定
			D3DXMATRIX mtxMuzzle;
			universal::SetOffSet(&mtxMuzzle, *pMtx, D3DXVECTOR3(-50.0f, 20.0f, 0.0f));

			D3DXVECTOR3 posMuzzle =
			{
				mtxMuzzle._41,
				mtxMuzzle._42,
				mtxMuzzle._43,
			};

			CPlayer* pPlayer = GetPlayer();
			D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

			if (pPlayer != nullptr)
			{// プレイヤーの向きに移動量を設定
				rot = pPlayer->GetRot();

				// 弾を発射
				CMissile *pMissile = CMissile::Create();

				if (pMissile != nullptr)
				{
					pMissile->SetPosition(posMuzzle);
					pMissile->SetRot(rot);
					pMissile->SetIdxPlayer(pPlayer->GetID());
				}
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
void CLauncher::Draw(void)
{
	// 継承クラスの描画
	CWeapon::Draw();
}