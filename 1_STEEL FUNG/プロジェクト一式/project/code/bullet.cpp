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
#include "object3D.h"
#include "debugproc.h"
#include "slow.h"
#include "anim3D.h"
#include "animEffect3D.h"

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
	m_pObject3D = nullptr;
	m_nIdxPlayer = -1;
	m_fLengthDest = 0.0f;

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
	m_rot.y = atan2f(m_move.x, m_move.z);
	float fLengthMove = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);

	m_rot.x = atan2f(fLengthMove,m_move.y) - D3DX_PI * 0.5f;

	universal::LimitRot(&m_rot.x);

	// タイプの設定
	SetType(TYPE_BULLET);

	m_col = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);

	// 見た目の生成
	if (m_pObject3D == nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 rot = GetRotation();

		m_pObject3D = CObject3D::Create(pos, rot);

		if (m_pObject3D != nullptr)
		{
			m_pObject3D->SetSize(m_fSize, 0.0f);
			m_fLengthDest = m_fSize * 5.0f;
			m_pObject3D->SetMode(CObject3D::MODE::MODE_STRETCHBILLBOARD);
			m_pObject3D->SetVtx();

			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\bullet000.png");
			m_pObject3D->SetIdxTexture(nIdx);
			m_pObject3D->SetColor(m_col);
			m_pObject3D->EnableAdd(true);
			m_pObject3D->SetFactSB(100.0f);
			m_pObject3D->SetAlphaTest(60);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBullet::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pObject3D != nullptr)
	{
		m_pObject3D->Uninit();
		m_pObject3D = nullptr;
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
	float fDeltaTime = CManager::GetDeltaTime();

	m_fLife -= fDeltaTime;

	m_posOld = m_pos;

	CSlow *pSlow = CSlow::GetInstance();

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		// 位置の更新
		m_pos += m_move * fScale;
	}
	else
	{
		// 位置の更新
		m_pos += m_move;
	}

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

			if (m_pCollisionSphere->OnEnter(CCollision::TAG_ROCKET))
			{// 対象との当たり判定
				CObject *pObj = m_pCollisionSphere->GetOther();

				if (pObj != nullptr)
				{
					bHit = true;

					// 当たったオブジェクトのヒット処理
					pObj->Hit(m_fDamage);
				}
			}

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
			Death();
		}
	}

	if (bHit == false)
	{
		if (m_fLife < 0)
		{// 自分の削除
			Death();
		}
	}
	else
	{
		if (m_bPierce == false)
		{// 貫通しない弾は消える
			Death();
		}
	}

	if (m_pObject3D != nullptr)
	{// 見た目の追従
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 rot = GetRotation();
		D3DXVECTOR2 size = D3DXVECTOR2(m_pObject3D->GetWidth(), m_pObject3D->GetHeight());

		size.y += (m_fLengthDest - size.y) * 0.2f;

		m_pObject3D->SetSize(size.x, size.y);
		m_pObject3D->SetPosition(pos);
		m_pObject3D->SetRotation(rot);
		m_pObject3D->SetVtx();
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

			// ダメージエフェクトの生成
			CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

			if (pAnim3D != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();

				pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_HIT00);
			}

			Sound::Play(CSound::LABEL_SE_HIT00);
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