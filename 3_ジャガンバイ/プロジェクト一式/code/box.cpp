//*****************************************************
//
// 木箱の処理[box.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "box.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "playerManager.h"
#include "weapon.h"
#include "player.h"
#include "billboard.h"
#include "texture.h"
#include "debrisSpawner.h"
#include "itemRepair.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace 
{
	const char* MODEL_PATH = "data\\MODEL\\item\\WoodBox.x";	// モデルのパス
}

//=====================================================
// コンストラクタ
//=====================================================
CBox::CBox(int nPriority) : CObjectX(nPriority)
{
	m_pCollisionSphere = nullptr;
	m_pCollisionCube = nullptr;
	m_type = TYPE_NONE;
}

//=====================================================
// デストラクタ
//=====================================================
CBox::~CBox()
{

}

//=====================================================
// 生成処理
//=====================================================
CBox *CBox::Create(TYPE type)
{
	CBox *pBox = nullptr;

	if (pBox == nullptr)
	{
		pBox = new CBox;

		if (pBox != nullptr)
		{
			pBox->SetType(type);

			// 初期化
			pBox->Init();
		}
	}

	return pBox;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBox::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	// モデルの読込
	int nIdx = CModel::Load((char*)MODEL_PATH);
	SetIdxModel(nIdx);
	BindModel(nIdx);

	if (m_pCollisionSphere == nullptr)
	{// 当たり判定の生成
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_BOX, CCollision::TYPE_SPHERE, this);

		if (m_pCollisionSphere != nullptr)
		{
			m_pCollisionSphere->SetRadius(50.0f);
		}
	}

	if (m_pCollisionCube == nullptr)
	{// 当たり判定生成
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 vtxMax = GetVtxMax();
		D3DXVECTOR3 vtxMin = GetVtxMin();

		m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_BLOCK, this);

		if (m_pCollisionCube != nullptr)
		{
			m_pCollisionCube->SetPosition(pos);

			m_pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	// 影の有効化
	EnableShadow(true);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBox::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{
		m_pCollisionSphere->Uninit();
		m_pCollisionSphere = nullptr;
	}

	if (m_pCollisionCube != nullptr)
	{
		m_pCollisionCube->Uninit();
		m_pCollisionCube = nullptr;
	}

	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CBox::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();

	if (m_pCollisionSphere != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_pCollisionSphere->SetPosition(pos);
	}

	if (m_pCollisionCube != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_pCollisionCube->SetPosition(pos);
	}
}

//=====================================================
// ヒット処理
//=====================================================
void CBox::Hit(float fDamage)
{
	D3DXVECTOR3 pos = GetPosition();

	// 破片の放出
	CDebrisSpawner::Create(pos,CDebrisSpawner::TYPE::TYPE_WALL);
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Play(pSound->LABEL_SE_BOX_BROKEN);
	}

	switch (m_type)
	{
	case CBox::TYPE_REPAIR:
	{
		CItemRepair *pRepair = CItemRepair::Create();

		if (pRepair != nullptr)
		{
			pRepair->SetPosition(pos);
		}
	}
		break;
	case CBox::TYPE_RANDOM:
		break;
	default:
		assert(("木箱の種類に不正な値が入っているか設定していない処理を呼んでいます", false));
		break;
	}

	Uninit();
}

//=====================================================
// 種類の設定
//=====================================================
void CBox::SetType(TYPE type)
{
	if (type <= TYPE_NONE || type >= TYPE_MAX)
	{
		assert(("木箱の種類に不正な値をいれています", false));
	}
	else
	{
		m_type = type;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CBox::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}