//*****************************************************
//
// レールガンの処理[weaponRailgun.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "weaponRailgun.h"
#include "inputjoypad.h"
#include "bullet.h"
#include "player.h"
#include "sound.h"
#include "animEffect3D.h"
#include "universal.h"
#include "debugproc.h"
#include "playerManager.h"
#include "effect3D.h"
#include "enemyManager.h"
#include "weaponManager.h"
#include "object3D.h"
#include "motion.h"
#include "beam.h"
#include "inpact.h"
#include "record.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int NUM_VTX = 4;	// 当たり判定の頂点数
const D3DXCOLOR COL_RANGE = { 1.0f,1.0f,1.0f,0.4f };	// 範囲表示の色
}

//=====================================================
// コンストラクタ
//=====================================================
CRailgun::CRailgun(int nPriority) : CWeapon(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfoRailgun));
	m_pRange = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CRailgun::~CRailgun()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRailgun::Init(void)
{
	// 継承クラスの初期化
	CWeapon::Init();

	// 固有情報入手
	CWeaponManager *pWeaponManager = CWeaponManager::GetInstance();

	if (pWeaponManager != nullptr)
	{
		CWeaponManager::SInfoRailgun info = pWeaponManager->GetRailgunInfo();

		m_info.fWidth = info.fWidth;
		m_info.fLength = info.fLength;
	}

	// 範囲表示の生成
	if (m_pRange == nullptr)
	{
		m_pRange = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pRange->SetSize(0.0f, 0.0f);
		m_pRange->SetColor(COL_RANGE);
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\range.png");
		m_pRange->SetIdxTexture(nIdx);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRailgun::Uninit(void)
{
	if (m_pRange != nullptr)
	{
		m_pRange->Uninit();
		m_pRange = nullptr;
	}

	// 継承クラスの終了
	CWeapon::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CRailgun::Update(void)
{
	// 継承クラスの更新
	CWeapon::Update();
}

//=====================================================
// 攻撃処理
//=====================================================
void CRailgun::Attack(void)
{
	// 当たり判定の発生
	Shot();
}

//=====================================================
// 射撃処理
//=====================================================
void CRailgun::Shot(void)
{
	// マズルの位置を設定
	D3DXMATRIX* pMtxWeapon = GetPlayer()->GetMatrix();
	D3DXMATRIX mtxMuzzle;
	universal::SetOffSet(&mtxMuzzle, *pMtxWeapon, D3DXVECTOR3(-18.0f, 6.0f, 0.0f));

	D3DXVECTOR3 posMuzzle =
	{
		mtxMuzzle._41,
		mtxMuzzle._42,
		mtxMuzzle._43,
	};

	// 頂点の設定
	D3DXVECTOR3 aPosVtx[NUM_VTX];
	D3DXMATRIX aMtxVtx[NUM_VTX];
	D3DXVECTOR3 aOffset[NUM_VTX] =
	{
		{ m_info.fWidth,0.0f,-m_info.fLength },
		{ -m_info.fWidth,0.0f,-m_info.fLength },
		{ -m_info.fWidth,0.0f,0.0f },
		{ m_info.fWidth,0.0f,0.0f },

	};

	for (int i = 0; i < NUM_VTX; i++)
	{
		universal::SetOffSet(&aMtxVtx[i], mtxMuzzle, aOffset[i]);

		aPosVtx[i] =
		{
			aMtxVtx[i]._41,
			0.0f,
			aMtxVtx[i]._43,
		};

#ifdef _DEBUG
		CEffect3D::Create(aPosVtx[i], 10.0f, 60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
	}

	// 頂点情報設定
	SetVtx(aPosVtx[0], aPosVtx[1], aPosVtx[2], aPosVtx[3]);

	// 入力
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
	{
		return;
	}

	int nBullet = GetBullet();
	int nID = GetID();
	int nCntShot = GetCntShot();

	// 敵との接触を判定する
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		// 先頭オブジェクトを代入
		CEnemy* pEnemy = pEnemyManager->GetHead();

		while (pEnemy != nullptr)
		{
			// 次のアドレスを保存
			CEnemy* pEnemyNext = pEnemy->GetNext();

			if (pEnemy != nullptr)
			{
				D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

				bool bHit = universal::CubeCrossProduct(aPosVtx[0], aPosVtx[1], aPosVtx[2], aPosVtx[3], posEnemy);

				CWeapon::SInfo info = GetInfo();

				if (bHit)
				{
					if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB, nID))
					{// 射撃
						if (nBullet > 0 && nCntShot == 0)
						{// 弾の発射
							pEnemy->Hit(info.fDamage);

							// 情報取得処理
							CPlayer* pPlayer = GetPlayer();
							CRecord* pRecord = CRecord::GetInstance();

							// 破壊数の戦績加算処理
							if (pPlayer != nullptr &&
								pRecord != nullptr)
							{
								pRecord->CheckDeathEnemy(pEnemy, pPlayer->GetID());
							}
						}
						else
						{// 弾切れの場合

						}
					}

					CMotion *pBody = pEnemy->GetBody();

					if (pBody != nullptr)
					{
						pBody->SetAllCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					}
				}
				else
				{
					CMotion *pBody = pEnemy->GetBody();

					if (pBody != nullptr)
					{
						pBody->ResetAllCol();
					}
				}
			}

			// 次のアドレスを代入
			pEnemy = pEnemyNext;
		}
	}

	if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB, nID))
	{// 弾を減らす
		// 弾を減らす
		nBullet--;
		SetBullet(nBullet);

		// 連射カウンターのリセット
		nCntShot = GetRapid();

		SetCntShot(nCntShot);

		// ビームの生成
		CBeam *pBeam = CBeam::Create();

		if (pBeam != nullptr)
		{
			D3DXMATRIX mtxBeam;

			universal::SetOffSet(&mtxBeam, mtxMuzzle, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

			pBeam->SetMtx(mtxBeam);
			pBeam->SetAnimSize(m_info.fWidth, m_info.fLength);
		}

		// 衝撃波の生成
		D3DXMATRIX mtxInpact;

		universal::SetOffSet(&mtxInpact, mtxMuzzle, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));

		CInpact *pInpact = CInpact::Create(0.2f, &mtxInpact);

		if (pInpact != nullptr)
		{
			pInpact->SetSpeedExpand(25.0f);
		}

		universal::SetOffSet(&mtxInpact, mtxMuzzle, D3DXVECTOR3(0.0f, 0.0f, m_info.fLength * -0.5f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
		pInpact = CInpact::Create(0.2f, &mtxInpact);

		if (pInpact != nullptr)
		{
			pInpact->SetSpeedExpand(25.0f);
		}

		// 音の再生
		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_GUNSHOT_03);
		}

		// パラメーター取得
		CWeapon::SInfo info = GetInfo();

		if (pJoypad != nullptr)
		{// コントローラーの振動
			pJoypad->Vibration(nID, CInputJoypad::PADVIB::PADVIB_USE, info.fVibPower, info.nVibTime);
		}
	}
}

//=====================================================
// ポリゴンの頂点設定
//=====================================================
void CRailgun::SetVtx(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4)
{
	if (m_pRange == nullptr)
	{
		return;
	}

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRange->GetVtxBuff();

	if (pVtxBuff == nullptr)
	{
		return;
	}

	//頂点情報のポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = vtx1;
	pVtx[1].pos = vtx2;
	pVtx[2].pos = vtx4;
	pVtx[3].pos = vtx3;

	pVtx[0].pos.y = 2.0f;
	pVtx[1].pos.y = 2.0f;
	pVtx[2].pos.y = 2.0f;
	pVtx[3].pos.y = 2.0f;

	//頂点バッファをアンロック
	pVtxBuff->Unlock();
}

//=====================================================
// 有効化切り替え
//=====================================================
void CRailgun::ToggleEnable(bool bEnable)
{
	if (bEnable)
	{
		if (m_pRange == nullptr)
		{// 範囲表示の生成
			m_pRange = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_pRange->SetSize(0.0f, 0.0f);
			m_pRange->SetColor(COL_RANGE);

			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\range.png");
			m_pRange->SetIdxTexture(nIdx);
		}
	}
	else
	{
		if (m_pRange != nullptr)
		{
			m_pRange->Uninit();
			m_pRange = nullptr;
		}
	}

	CWeapon::ToggleEnable(bEnable);
}

//=====================================================
// 描画処理
//=====================================================
void CRailgun::Draw(void)
{
	// 継承クラスの描画
	CWeapon::Draw();
}