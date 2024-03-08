//*****************************************************
//
// 爆発攻撃の処理[explosionAttack.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "explosionAttack.h"
#include "collision.h"
#include "animEffect3D.h"
#include "anim3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float DAMAGE = 5.0f;	// ダメージ
const DWORD SPEED_ALPHA = 5;	// アルファテスト値の減る速度
const DWORD LINE_DELETE = 255;	// 消えるしきい値
}

//=====================================================
// コンストラクタ
//=====================================================
CExplAttack::CExplAttack(int nPriority) : CObject(nPriority)
{
	m_fRadius = 0.0f;
	m_pCollision = nullptr;
	m_pAnim = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CExplAttack::~CExplAttack()
{

}

//=====================================================
// 生成処理
//=====================================================
CExplAttack *CExplAttack::Create(D3DXVECTOR3 pos, float fRadius)
{
	CExplAttack *pExplAttack = nullptr;

	if (pExplAttack == nullptr)
	{
		pExplAttack = new CExplAttack;

		if (pExplAttack != nullptr)
		{
			pExplAttack->m_fRadius = fRadius;
			pExplAttack->m_pos = pos;
			pExplAttack->Init();
		}
	}

	return pExplAttack;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CExplAttack::Init(void)
{
	if (m_pCollision == nullptr)
	{// 当たり判定の生成
		m_pCollision = CCollisionSphere::Create(CCollision::TAG_ENEMYBULLET, CCollision::TYPE_SPHERE, this);

		if (m_pCollision != nullptr)
		{
			m_pCollision->SetRadius(m_fRadius * 2);
			m_pCollision->SetPosition(m_pos);
		}
	}

	// アニメーションエフェクト生成
	CAnimEffect3D *pAnimEffect = CAnimEffect3D::GetInstance();

	if (pAnimEffect != nullptr)
	{
		// プラズマ
		m_pAnim = pAnimEffect->CreateEffect(m_pos, CAnimEffect3D::TYPE::TYPE_PLASMA);

		if (m_pAnim != nullptr)
		{
			m_pAnim->SetSize(m_fRadius, m_fRadius);
			m_pAnim->RandStart();
			m_pAnim->SetAlphaTest(70);
			m_pAnim->EnableZtest(false);
		}

		// 衝撃波
		CAnim3D *pInpact = pAnimEffect->CreateEffect(m_pos, CAnimEffect3D::TYPE::TYPE_HIT00);
		
		if (pInpact != nullptr)
		{
			pInpact->SetSize(m_fRadius * 3.0f, m_fRadius * 3.0f);
			pInpact->SetAlphaTest(70);
			pInpact->EnableZtest(false);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CExplAttack::Uninit(void)
{
	if (m_pCollision != nullptr)
	{
		m_pCollision->Uninit();
		m_pCollision = nullptr;
	}

	if (m_pAnim != nullptr)
	{
		m_pAnim->Uninit();
		m_pAnim = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CExplAttack::Update(void)
{
	if (m_pCollision != nullptr)
	{// プレイヤーとの当たり判定
		if (m_pCollision->OnEnter(CCollision::TAG::TAG_PLAYER))
		{
			CObject *pObj = m_pCollision->GetOther();

			if (pObj != nullptr)
			{
				pObj->Hit(DAMAGE);

				m_pCollision->Uninit();
				m_pCollision = nullptr;

				return;
			}
		}
	}

	if (m_pAnim != nullptr)
	{
		// アルファテストのしきい値を上げていく
		DWORD dwAlpha = m_pAnim->GetAlpha();

		dwAlpha += SPEED_ALPHA;

		if (dwAlpha > LINE_DELETE)
		{
			Uninit();

			return;
		}

		m_pAnim->SetAlphaTest(dwAlpha);
	}
	else
	{
		Uninit();

		return;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CExplAttack::Draw(void)
{

}