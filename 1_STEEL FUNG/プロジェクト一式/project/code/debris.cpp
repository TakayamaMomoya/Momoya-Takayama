//*****************************************************
//
// 破片処理[debris.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "debris.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
	const float GEOWND = 0.0f;	// 床判定の高さ
	const float BOUNCE = 4.0f;	// 跳ね返りの強さ
	const float ROT_VELOCITY = 0.05f;	// 回転速度の制限
	const float SPEED_SHRINK = 0.05f;	// 縮む速度
	const float LINE_SHRINK = 0.1f;	// 縮む状態になるしきい値
};

//=====================================================
// コンストラクタ
//=====================================================
CDebris::CDebris(int nPriority)
{
	m_rotVelocity = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
	m_nLife = 0;
	m_fGravity = 0.0f;
	m_state = STATE_NONE;
	m_nModelType = 0;
	m_bBounce = false;
}

//=====================================================
// デストラクタ
//=====================================================
CDebris::~CDebris()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CDebris::Init(void)
{
	CObjectX::Init();

	// 回転速度の設定
	m_rotVelocity.x = (float)(rand() % 629 - 314) / 100.0f;
	m_rotVelocity.y = (float)(rand() % 629 - 314) / 100.0f;
	m_rotVelocity.z = (float)(rand() % 629 - 314) / 100.0f;

	int nIdx = 0;

	// モデル読込
	if (m_nModelType == TYPE_WALL)
	{
		nIdx = CModel::Load("data\\MODEL\\other\\debris.x");
	}

	BindModel(nIdx);

	// 値の初期化
	m_state = STATE_NORMAL;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CDebris::Uninit(void)
{
	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CDebris::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();

	D3DXVECTOR3 pos = GetPosition();

	// 寿命減衰
	m_nLife--;

	// 重力加算
	m_move.y -= m_fGravity;

	// 移動量の減衰
	m_move.x *= 0.98f;
	m_move.z *= 0.98f;

	// 縮ませるかの判定
	float fSpeed = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);

	if (fSpeed <= LINE_SHRINK)
	{
		m_state = STATE_SHRINK;
	}

	// 位置更新
	SetPosition(pos + m_move);

	if (m_state == STATE_SHRINK)
	{// 縮む処理
		Shrink();
	}
}

//=====================================================
// 縮む処理
//=====================================================
void CDebris::Shrink(void)
{
	float fScale = GetScale();

	// 縮む
	fScale -= SPEED_SHRINK;

	if (fScale <= 0.0f)
	{
		fScale = 0.0f;

		Uninit();
	}

	// スケール設定
	SetScale(fScale);
}

//=====================================================
// 描画処理
//=====================================================
void CDebris::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//=====================================================
// 生成処理
//=====================================================
CDebris* CDebris::Create(D3DXVECTOR3 pos, int nLife, int nModelType, D3DXVECTOR3 move, float fGravity, bool bBounce, int nPriority)
{
	CDebris* pDebrisSpawner = nullptr;

	if (pDebrisSpawner == nullptr)
	{// インスタンス生成
		pDebrisSpawner = new CDebris(nPriority);

		if (pDebrisSpawner != nullptr)
		{
			pDebrisSpawner->SetPosition(pos);

			pDebrisSpawner->m_nModelType = nModelType;

			// 初期化処理
			pDebrisSpawner->Init();

			pDebrisSpawner->m_nLife = nLife;

			pDebrisSpawner->m_move = move;

			pDebrisSpawner->m_fGravity = fGravity;

			pDebrisSpawner->m_bBounce = bBounce;
		}
	}

	return pDebrisSpawner;
}