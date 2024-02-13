//*****************************************************
//
// コンテナの処理[container.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "container.h"
#include "containerManager.h"
#include "manager.h"
#include "renderer.h"
#include "playerManager.h"
#include "weapon.h"
#include "player.h"
#include "universal.h"
#include "itemWeapon.h"
#include "weaponManager.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const char* BODY_PATH = "data\\MODEL\\gimmick\\MysteryBox_Down.x";	// 本体のパス
	const char* CAP_PATH = "data\\MODEL\\gimmick\\MysteryBox_Up.x";	// 蓋のパス
	const float TIME_DEATH = 1.5f;	// 死亡までの時間
	const float TIME_DEPLOY = TIME_DEATH * 0.1f;	// アイテムを設置するまでのラグ
	const float ROLL_SPEED = 0.3f;	// 蓋の回転速度
	const float OPEN_ANGLE = D3DX_PI * 0.7f;	// 回転制限
	const float WEAPON_JUMP = 10.0f;	// 武器設置時のジャンプ力
}

//=====================================================
// コンストラクタ
//=====================================================
CContainer::CContainer(int nPriority) : CGimmick(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CContainer::~CContainer()
{

}

//=====================================================
// 生成処理
//=====================================================
CContainer *CContainer::Create(void)
{
	CContainer *pContainer = nullptr;

	if (pContainer == nullptr)
	{
		pContainer = new CContainer;

		if (pContainer != nullptr)
		{
			// 初期化
			pContainer->Init();
		}
	}

	return pContainer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CContainer::Init(void)
{
	// 継承クラスの初期化
	CGimmick::Init();

	// 読み込み
	Load();

	m_info.state = STATE_NORMAL;
	m_info.fTimerDeath = TIME_DEATH;

	return S_OK;
}

//=====================================================
// 読み込み処理
//=====================================================
void CContainer::Load(void)
{
	// 本体の読込
	int nIdx = CModel::Load((char*)BODY_PATH);
	SetIdxModel(nIdx);
	BindModel(nIdx);

	if (m_info.pCap == nullptr)
	{// 蓋の生成
		m_info.pCap = CObjectX::Create();

		if (m_info.pCap != nullptr)
		{
			// 蓋の読み込み
			int nIdx = CModel::Load((char*)CAP_PATH);
			m_info.pCap->SetIdxModel(nIdx);
			m_info.pCap->BindModel(nIdx);
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CContainer::Uninit(void)
{
	// スポーン情報の保存
	CContainerManager *pContainerManager = CContainerManager::GetInstance();

	if (pContainerManager != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		pContainerManager->SavePosition(pos);
	}

	if (m_info.pCap != nullptr)
	{
		m_info.pCap->Uninit();
		m_info.pCap = nullptr;
	}

	// 継承クラスの終了
	CGimmick::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CContainer::Update(void)
{
	// 継承クラスの更新
	CGimmick::Update();

	if (m_info.state == STATE_OPEN)
	{// 開いている状態の更新
		UpdateOpen();
	}
}

//=====================================================
// 開いている状態の更新
//=====================================================
void CContainer::UpdateOpen(void)
{	
	// 蓋を開く処理
	if (m_info.pCap != nullptr)
	{
		D3DXVECTOR3 rot = m_info.pCap->GetRot();

		rot.x += ROLL_SPEED;

		if (rot.x >= OPEN_ANGLE)
		{
			rot.x = OPEN_ANGLE;
		}

		m_info.pCap->SetRot(rot);
	}

	// 経過時間取得
	float fTick = CManager::GetTick();

	m_info.fTimerDeath -= fTick;

	// アイテムを出現させる処理
	if (TIME_DEATH - TIME_DEPLOY > m_info.fTimerDeath)
	{
		if (m_info.pWeapon == nullptr)
		{
			// 武器の種類をランダムで設定
			

			CWeapon::TYPE type = (CWeapon::TYPE)WeaponRand();

			// 武器の設置
			m_info.pWeapon = CItemWeapon::Create(type);

			if (m_info.pWeapon != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();

				m_info.pWeapon->SetPosition(pos);
			}
		}
	}

	// 箱を消す処理
	if (m_info.fTimerDeath <= 0.0f)
	{
		m_info.fTimerDeath = 0.0f;
		m_info.state = STATE_END;

		Uninit();
	}
}

//=====================================================
// インタラクト
//=====================================================
void CContainer::Interact(CObject *pObj)
{
	if (m_info.state != STATE_NORMAL)
	{
		return;
	}

	CPlayerManager *pPlayerManager = CPlayerManager::GetInstance();

	if (pObj == nullptr || pPlayerManager == nullptr)
	{
		return;
	}

	// プレイヤー取得
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		CPlayer *pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			if ((CObject*)pPlayer == pObj)
			{// プレイヤーのインタラクト検出
				bool bGet = pPlayer->InputInteract();

				if (bGet)
				{// 箱を開く
					Open();

					// サウンドインスタンスの取得
					CSound* pSound = CSound::GetInstance();

					if (pSound != nullptr)
					{
						pSound->Play(pSound->LABEL_SE_OPEN_BOX);
					}

					SetEnable(false);
				}
			}
		}
	}
}

//=====================================================
// 箱を開く処理
//=====================================================
void CContainer::Open(void)
{
	// 箱を開く状態
	m_info.state = STATE_OPEN;
}

//=====================================================
// 武器のランダムで設定する処理
//=====================================================
int CContainer::WeaponRand(void)
{
	int anProb[CWeapon::TYPE::TYPE_MAX];	// 確率の保存用
	int nMinProb = INT_MAX;
	int nMax = 0;

	ZeroMemory(&anProb[0], sizeof(int) * CWeapon::TYPE::TYPE_MAX);

	// 武器マネージャーから各武器の確率を取得、最大値の設定
	CWeaponManager *pWeaponManager = CWeaponManager::GetInstance();

	if (pWeaponManager != nullptr)
	{
		for (int i = 0; i < CWeapon::TYPE::TYPE_MAX; i++)
		{
			anProb[i] = pWeaponManager->GetProb(i);

			if (anProb[i] > nMax)
			{
				nMax = anProb[i];
			}
		}
	}

	// 乱数の設定
	
	int nProb = universal::RandRange(nMax,1);	// 期待値の合計を最大値として乱数を振る
	int nIdxWeapon = -1;

	for (int i = 0; i < CWeapon::TYPE::TYPE_MAX; i++)
	{
		if (nProb <= anProb[i])
		{// 確率内のとき
			if (anProb[i] < nMinProb)
			{// 他の当たった確率より低い場合のみ適用
				nIdxWeapon = i;
				nMinProb = anProb[i];
			}
		}
	}

	if (nIdxWeapon == -1)
	{
		assert(("武器の設定に失敗", false));
	}

	return nIdxWeapon;
}

//=====================================================
// 位置の設定
//=====================================================
void CContainer::SetPosition(D3DXVECTOR3 pos)
{
	// 継承クラスの設置
	CObjectX::SetPosition(pos);

	if (m_info.pCap != nullptr)
	{// 蓋の追従
		D3DXVECTOR3 pos = GetPosition();

		m_info.pCap->SetPosition(pos);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CContainer::Draw(void)
{
	// 継承クラスの描画
	CGimmick::Draw();
}