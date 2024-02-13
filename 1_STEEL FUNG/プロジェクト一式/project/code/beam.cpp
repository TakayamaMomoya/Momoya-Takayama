//*****************************************************
//
// ビームの処理[beam.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "beam.h"
#include "anim3D.h"
#include "animEffect3D.h"
#include "universal.h"
#include "collision.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int NUM_VTX = 4;	// 当たり判定の頂点数
const float INITIAL_WIDTH_ANIM = 50.0f;	// アニメーションの幅
const float INITIAL_WIDTH_LENGTH = 300.0f;	// アニメーションの長さ
const float INITIAL_SPEED_EXPAND = 40.f;	// 膨らむ速度
const float INITIAL_SPEED_SHRINK = 5.0f;	// 縮む速度
const float INITIAL_SPEED_EXTEND = 200.0f;	// 伸びる速度
}

//=====================================================
// コンストラクタ
//=====================================================
CBeam::CBeam(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CBeam::~CBeam()
{

}

//=====================================================
// 生成処理
//=====================================================
CBeam *CBeam::Create(void)
{
	CBeam *pBeam = nullptr;

	if (pBeam == nullptr)
	{
		pBeam = new CBeam;

		if (pBeam != nullptr)
		{
			// 初期化
			pBeam->Init();
		}
	}

	return pBeam;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBeam::Init(void)
{
	if (m_info.pAnim == nullptr)
	{// アニメーションエフェクトの生成
		CAnimEffect3D *pAnimEffect = CAnimEffect3D::GetInstance();

		if (pAnimEffect != nullptr)
		{
			m_info.pAnim = pAnimEffect->CreateEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAnimEffect3D::TYPE::TYPE_BEAM);

			if (m_info.pAnim != nullptr)
			{
				m_info.pAnim->EnableZtest(false);
				m_info.pAnim->SetMode(CObject3D::MODE::MODE_STRETCHBILLBOARD);
			}
		}
	}

	// 値の初期化
	m_info.fWidthAnimDest = INITIAL_WIDTH_ANIM;
	m_info.fLengthAnimDest = INITIAL_WIDTH_LENGTH;
	m_info.fSpeedExpand = INITIAL_SPEED_EXPAND;
	m_info.fSpeedShrink = INITIAL_SPEED_SHRINK;
	m_info.fSpeedExtend = INITIAL_SPEED_EXTEND;

	// 当たり判定の生成
	if (m_info.pCollision == nullptr)
	{
		m_info.pCollision = CCollisionSphere::Create(CCollision::TAG_ENEMYBULLET, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollision != nullptr)
		{
			m_info.pCollision->SetRadius(500.0f);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBeam::Uninit(void)
{
	if (m_info.pAnim != nullptr)
	{
		m_info.pAnim->Uninit();
		m_info.pAnim = nullptr;
	}

	if (m_info.pCollision != nullptr)
	{
		m_info.pCollision->Uninit();
		m_info.pCollision = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBeam::Update(void)
{
	// サイズの管理
	ManageSize();

	// アニメーションの頂点位置設定
	SetAnimVtx();
}

//=====================================================
// サイズの管理
//=====================================================
void CBeam::ManageSize(void)
{
	if (m_info.fLengthAnimDest > m_info.fLengthAnim)
	{
		// 伸びる処理
		m_info.fLengthAnim += m_info.fSpeedExtend;

		if (m_info.fLengthAnimDest <= m_info.fLengthAnim)
		{// 長さ制限
			m_info.fLengthAnim = m_info.fLengthAnimDest;
		}
	}

	if (m_info.bFinish == false)
	{
		if (m_info.fWidthAnimDest > m_info.fWidthAnim)
		{
			// 膨らむ処理
			m_info.fWidthAnim += m_info.fSpeedExpand;

			if (m_info.fWidthAnimDest <= m_info.fWidthAnim)
			{// 幅の制限
				m_info.fWidthAnim = m_info.fWidthAnimDest;

				m_info.bFinish = true;
			}
		}
	}
	else
	{
		// 縮む処理
		m_info.fWidthAnim -= m_info.fSpeedShrink;
		
		if (m_info.fWidthAnim <= 0.0f)
		{// 縮みきったら終了
			m_info.fWidthAnim = 0.0f;

			Uninit();
		}
	}
}

//=====================================================
// アニメーションの頂点位置設定
//=====================================================
void CBeam::SetAnimVtx(void)
{
	if (m_info.pAnim == nullptr)
	{
		return;
	}

	CAnim3D *pAnim = m_info.pAnim;

	D3DXVECTOR3 pos =
	{
		sinf(m_info.rot.x - D3DX_PI * 0.5f) * sinf(m_info.rot.y) * m_info.fLengthAnim,
		cosf(m_info.rot.x - D3DX_PI * 0.5f) * m_info.fLengthAnim,
		sinf(m_info.rot.x - D3DX_PI * 0.5f) * cosf(m_info.rot.y) * m_info.fLengthAnim
	}; 

	D3DXVECTOR3 posMiddle = m_info.pos + pos;

	pAnim->SetSize(m_info.fWidthAnim, m_info.fLengthAnim);
	pAnim->SetPosition(posMiddle);
	pAnim->SetVtx();

	if (m_info.pCollision != nullptr)
	{
		m_info.pCollision->SetPositionOld(m_info.pCollision->GetPosition());
		m_info.pCollision->SetPosition(m_info.pos + pos * 2);

		if (m_info.pCollision->IsTriggerEnter(CCollision::TAG_PLAYER))
		{
			CObject *pObj = m_info.pCollision->GetOther();

			if (pObj != nullptr)
			{
				// 当たったオブジェクトのヒット処理
				pObj->Hit(10.0f);
			}
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CBeam::Draw(void)
{

}

//=====================================================
// 位置設定
//=====================================================
void CBeam::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;

	if (m_info.pAnim != nullptr)
	{
		m_info.pAnim->SetPosition(pos);
	}
}

//=====================================================
// 向き設定
//=====================================================
void CBeam::SetRotation(D3DXVECTOR3 rot)
{
	m_info.rot = rot;

	if (m_info.pAnim != nullptr)
	{
		m_info.pAnim->SetRotation(rot);
	}
}

//=====================================================
// 色設定
//=====================================================
void CBeam::SetCol(D3DXCOLOR col)
{
	if (m_info.pAnim != nullptr)
	{
		m_info.pAnim->SetColor(col);
	}
}

//=====================================================
// サイズ設定
//=====================================================
void CBeam::SetAnimSize(float fWidth, float fLength)
{
	m_info.fWidthAnimDest = fWidth; 
	m_info.fLengthAnimDest = fLength;

	if (m_info.pCollision != nullptr)
	{
		m_info.pCollision->SetRadius(fWidth * 0.5f);
	}
}