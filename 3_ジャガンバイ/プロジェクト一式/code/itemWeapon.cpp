//*****************************************************
//
// 武器アイテムの処理[ItemWeaponWeapon.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "ItemWeapon.h"
#include "manager.h"
#include "renderer.h"
#include "playerManager.h"
#include "weapon.h"
#include "player.h"
#include "debugproc.h"
#include "object3D.h"
#include "texture.h"
#include "meshcylinder.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float GRAVITY = 0.3f;	// 重力

	D3DXCOLOR WEPONCOL[CWeapon::TYPE_MAX] =
	{// 武器ごとのエフェクトの色
		{1.0f, 1.0f, 1.0f, 0.35f},	// マグナム
		{1.0f, 1.0f, 1.0f, 0.35f},	// マシンガン
		{1.0f, 0.8f, 0.0f, 0.35f},	// ショットガン
		{1.0f, 0.0f, 0.0f, 0.35f},	// レールガン
		{1.0f, 0.0f, 0.0f, 0.35f},	// ミニガン
		{1.0f, 0.0f, 0.0f, 0.35f},	// ロケラン
	};

	const float SPEED_SCALING = 0.1f;	// スケールが大きくなる速度
	const float INITIAL_DESTSCALE = 1.5f;	// 初期の目標スケール
	const float INITIAL_HEIGHT_BASE = 150.0f;	// 初期の基準高さ
	const float RANGE_FLOAT = 50.0f;	// 浮き沈みする範囲
	const float SPEED_ROTATE = 0.01f;	// 回転速度
	const float SPEED_MOVE = 0.2f;	// 追従速度
	const float SPEED_FLOAT = 0.03f;	// 浮き沈みする速度
	const float SIZE_LIGHT = 60.0f;	// 光のサイズ
	const float SIZE_RADIUS = 60;	// シリンダーサイズ(幅)
	const float SIZE_HEIGHT = 50.0f;	// シリンダーサイズ(高)
	const float SPEED_SCALIN = 2.0f;	// スケールが大きくなる速度

}

//=====================================================
// コンストラクタ
//=====================================================
CItemWeapon::CItemWeapon(int nPriority) : CGimmick(nPriority)
{
	m_type = CWeapon::TYPE_MAGNUM;
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CItemWeapon::~CItemWeapon()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CItemWeapon::Init(void)
{
	// 継承クラスの初期化
	CGimmick::Init();

	// 読み込み
	Load();

	// スケール初期設定
	SetScale(0.0f);

	// 横向きにする
	D3DXVECTOR3 rot = GetRot();

	rot.x = D3DX_PI * 0.5f;

	SetRot(rot);

	m_info.fScaleDest = INITIAL_DESTSCALE;

	// 光の生成
	if (m_info.pLight == nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_info.pLight = CObject3D::Create(pos);

		if (m_info.pLight != nullptr)
		{
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\glow.png");
			m_info.pLight->SetIdxTexture(nIdx);
			m_info.pLight->SetColor(WEPONCOL[m_type]);
			m_info.pLight->SetSize(0.0f, 0.0f);
			m_info.pLight->EnableAdd(true);
		}
	
	}
	if (m_info.pCylinder == nullptr)
	{
		m_info.pCylinder = CMeshCylinder::Create();

		if (m_info.pCylinder != nullptr)
		{
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\ring00.png");
			m_info.pCylinder->SetIdxTexture(nIdx);
			m_info.pCylinder->SetCol(WEPONCOL[m_type]);
			m_info.pCylinder->SetRadius(SIZE_RADIUS);
			m_info.pCylinder->SetHeight(SIZE_HEIGHT);
			m_info.pCylinder->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// 読み込み処理
//=====================================================
void CItemWeapon::Load(void)
{
	char* apPath[CWeapon::TYPE_MAX] =
	{
		"data\\MODEL\\weapon\\magnum.x",
		"data\\MODEL\\weapon\\mac10.x",
		"data\\MODEL\\weapon\\shotgun.x",
		"data\\MODEL\\weapon\\railgun.x",
		"data\\MODEL\\weapon\\minigun_000.x",
		"data\\MODEL\\weapon\\launcher.x",
	};

	// モデルの読込
	int nIdx = CModel::Load(apPath[m_type]);
	SetIdxModel(nIdx);
	BindModel(nIdx);
}

//=====================================================
// 終了処理
//=====================================================
void CItemWeapon::Uninit(void)
{
	if (m_info.pLight != nullptr)
	{
		m_info.pLight->Uninit();
		m_info.pLight = nullptr;
	}
	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->Uninit();
		m_info.pCylinder = nullptr;
	}

	// 継承クラスの終了
	CGimmick::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CItemWeapon::Update(void)
{
	// 継承クラスの更新
	CGimmick::Update();

	// スケールの管理
	ManageScale();

	// 回す処理
	ManageTransform();

	// 位置情報の取得
	D3DXVECTOR3 pos = GetPosition();
	pos.y = 1.0f;

	// 光の追従
	if (m_info.pLight != nullptr)
	{
		m_info.pLight->SetPosition(pos);
	}
	// シリンダーの追従・回転
	if (m_info.pCylinder != nullptr)
	{
		D3DXVECTOR3 rot = m_info.pCylinder->GetRot();
		rot.y += SPEED_ROTATE;
		universal::LimitRot(rot);

		m_info.pCylinder->SetPosition(pos);
		m_info.pCylinder->SetRot({ rot.x, rot.y, rot.z });
	}
}

//=====================================================
// スケールの管理
//=====================================================
void CItemWeapon::ManageScale(void)
{
	// 武器のスケール設定
	float fScale = GetScale();

	fScale += (m_info.fScaleDest - fScale) * SPEED_SCALING;

	SetScale(fScale);

	// エフェクトのスケール設定
	if (m_info.pLight != nullptr)
	{
		fScale = m_info.pLight->GetWidth();
		fScale += (SIZE_RADIUS - fScale) * SPEED_SCALING;

		m_info.pLight->SetSize(fScale, fScale);
	}
	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetRadius(fScale);
		m_info.pCylinder->SetVtx();
	}
}

//=====================================================
// トランスフォームの管理
//=====================================================
void CItemWeapon::ManageTransform(void)
{
	// 回転
	D3DXVECTOR3 rot = GetRot();

	rot.y += SPEED_ROTATE;

	universal::LimitRot(&rot.y);

	SetRot(rot);

	// 目標位置に移動
	D3DXVECTOR3 pos = GetPosition();
	pos.y = 0.0f;
	D3DXVECTOR3 posDest = m_info.posDest + pos;

	pos += (posDest - pos) * SPEED_MOVE;

	SetPosition(pos);

	// 浮き沈みさせる
	m_info.fTimer += SPEED_FLOAT;

	universal::LimitRot(&m_info.fTimer);

	float fDiff = sinf(m_info.fTimer) * RANGE_FLOAT;

	m_info.posDest.y = INITIAL_HEIGHT_BASE + fDiff;
}

//=====================================================
// アイテム入手時の処理
//=====================================================
void CItemWeapon::Interact(CObject *pObj)
{
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
			{// プレイヤー検出
				bool bGet = pPlayer->InputInteract();

				if (bGet)
				{
					// 効果の付与
					ApplyEffect(pPlayer);

					Uninit();
				}
			}
		}
	}
}

//=====================================================
// 効果を適用する処理
//=====================================================
void CItemWeapon::ApplyEffect(CPlayer* pPlayer)
{
	if (pPlayer == nullptr)
	{
		return;
	}

	pPlayer->SetWeapon(m_type);
}

//=====================================================
// 描画処理
//=====================================================
void CItemWeapon::Draw(void)
{
	// 継承クラスの描画
	CGimmick::Draw();
}

//=====================================================
// 生成処理
//=====================================================
CItemWeapon *CItemWeapon::Create(CWeapon::TYPE type)
{
	if (type >= CWeapon::TYPE_MAX || type < 0)
	{
		assert(("想定外の武器を出そうとしてます",false));
	}

	CItemWeapon *pItemWeapon = nullptr;

	if (pItemWeapon == nullptr)
	{
		pItemWeapon = new CItemWeapon;

		if (pItemWeapon != nullptr)
		{
			pItemWeapon->m_type = type;

			// 初期化
			pItemWeapon->Init();
		}
	}

	return pItemWeapon;
}