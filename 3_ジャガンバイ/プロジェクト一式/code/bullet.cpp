//*****************************************************
//
// 弾処理[bullet.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "effect3D.h"
#include "particle.h"
#include "sound.h"
#include "collision.h"
#include "orbit.h"
#include "universal.h"
#include "record.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int EDGE_ORBIT = 20;	// 軌跡の辺の数
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CBullet::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CBullet::CBullet(int nPriority) : CObject(nPriority)
{
	m_fLife = 0.0f;
	m_fDamage = 0.0f;
	m_fSize = 0.0f;
	m_type = TYPE_NONE;
	m_bPierce = false;
	m_move = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_pCollisionSphere = nullptr;
	m_pOrbit = nullptr;
	m_nIdxPlayer = -1;

	// 総数カウントアップ
	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CBullet::~CBullet()
{
	m_nNumAll--;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBullet::Init(void)
{
	m_rot.y = atan2f(m_move.x,m_move.z);

	// タイプの設定
	SetType(TYPE_BULLET);

	Draw();

	if (m_pOrbit == nullptr)
	{// 軌跡の生成
		m_pOrbit = COrbit::Create(m_mtxWorld, D3DXVECTOR3(m_fSize, 0.0f, 0.0f), D3DXVECTOR3(-m_fSize, 0.0f, 0.0f), m_col, EDGE_ORBIT);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBullet::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{// 当たり判定の消去
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pOrbit != nullptr)
	{// 軌跡の終了
		m_pOrbit = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBullet::Update(void)
{
	// 変数宣言
	bool bHit = false;

	// 寿命減衰
	float fTIck = CManager::GetTick();

	m_fLife -= fTIck;

	m_posOld = m_pos;

	// 位置の更新
	m_pos += m_move;

	if (m_pCollisionSphere != nullptr)
	{// 当たり判定の管理
		// タグ
		CCollision::TAG tag = CCollision::TAG_PLAYER;

		// 当たり判定の位置設定
		m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());
		m_pCollisionSphere->SetPosition(m_pos);

		switch (m_type)
		{
		case TYPE_PLAYER:
			tag = CCollision::TAG_ENEMY;

			// 木箱との当たり判定
			bHit = BulletHit(CCollision::TAG_BOX);
			break;
		case TYPE_ENEMY:
			tag = CCollision::TAG_PLAYER;
			break;
		default:
			break;
		}

		// キャラクターとの当たり判定
		bHit = BulletHit(tag);

		if (m_pCollisionSphere->TriggerCube(CCollision::TAG_BLOCK))
		{// ブロックとの当たり判定
			if (m_pOrbit != nullptr)
			{
				m_pOrbit->SetEnd(true);
				m_pOrbit = nullptr;
			}

			Death();
		}
	}

	if (bHit == false)
	{
		if (m_fLife < 0)
		{// 自分の削除
			if (m_pOrbit != nullptr)
			{
				m_pOrbit->SetEnd(true);
				m_pOrbit = nullptr;
			}

			Death();
		}
	}
	else
	{
		if (m_bPierce == false)
		{// 貫通しない弾は消える
			if (m_pOrbit != nullptr)
			{
				m_pOrbit->SetEnd(true);
				m_pOrbit = nullptr;
			}

			Death();
		}
	}

	if (m_pOrbit != nullptr)
	{
		m_pOrbit->SetPositionOffset(m_mtxWorld, 0);
	}
}

//=====================================================
// 命中したか確認する処理
//=====================================================
bool CBullet::BulletHit(CCollision::TAG tag)
{
	bool bHit = false;

	if (m_pCollisionSphere->OnEnter(tag))
	{// 対象との当たり判定
		CObject *pObj = m_pCollisionSphere->GetOther();

		if (pObj != nullptr)
		{
			bHit = true;

			// 当たったオブジェクトのヒット処理
			pObj->Hit(m_fDamage);

			// 戦績の取得処理
			CRecord* pRecord = CRecord::GetInstance();

			// 破壊数の戦績加算処理
			if (pRecord != nullptr)
			{
				pRecord->CheckDeathEnemy(pObj, m_nIdxPlayer);
			}
		}
	}

	return bHit;
}

//=====================================================
// 死亡処理
//=====================================================
void CBullet::Death(void)
{
	// 終了処理
	Uninit();
}

//=====================================================
// 描画処理
//=====================================================
void CBullet::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
// 生成処理
//=====================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, float fLife,TYPE type, bool bPierce, float fRadius, float fDamage, D3DXCOLOR col)
{
	CBullet *pBullet = nullptr;

	if (pBullet == nullptr)
	{// インスタンス生成
		pBullet = new CBullet;

		if (pBullet != nullptr)
		{
			pBullet->m_move = move;
			pBullet->m_pos = pos;
			pBullet->m_posOld = pos;
			pBullet->m_fLife = fLife;
			pBullet->m_type = type;
			pBullet->m_bPierce = bPierce;
			pBullet->m_fDamage = fDamage;
			pBullet->m_col = col;
			pBullet->m_fSize = fRadius;

			if (pBullet->m_pCollisionSphere == nullptr)
			{// 当たり判定生成
				switch (type)
				{// 種類ごとにタグの設定
				case TYPE_PLAYER:
					pBullet->m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYERBULLET, CCollision::TYPE_SPHERE, pBullet);
					break;
				case TYPE_ENEMY:
					pBullet->m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ENEMYBULLET, CCollision::TYPE_SPHERE, pBullet);
					break;
				default:
					break;
				}
			}

			if (pBullet->m_pCollisionSphere != nullptr)
			{
				pBullet->m_pCollisionSphere->SetPosition(pBullet->m_pos);

				pBullet->m_pCollisionSphere->SetRadius(fRadius);
			}

			// 初期化処理
			pBullet->Init();
		}
	}

	return pBullet;
}