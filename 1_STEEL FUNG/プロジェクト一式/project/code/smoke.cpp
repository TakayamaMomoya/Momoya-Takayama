//*****************************************************
//
// 煙の処理[smoke.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "smoke.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_MOVE = 1.0f;	// 移動速度
const float SPEED_AVOID = 0.01f;	// 消える速度
const float SPEED_APPER = 0.05f;	// 出現速度
}

//=====================================================
// コンストラクタ
//=====================================================
CSmoke::CSmoke(int nPriority) : CObject3D(nPriority)
{
	m_move = { 0.0f,0.0f,0.0f };
	m_bTurn = false;
}

//=====================================================
//	デストラクタ
//=====================================================
CSmoke::~CSmoke()
{

}

//=====================================================
// 生成処理
//=====================================================
CSmoke *CSmoke::Create(D3DXVECTOR3 pos)
{
	CSmoke *pSmoke = nullptr;

	pSmoke = new CSmoke;

	if (pSmoke != nullptr)
	{
		pSmoke->Init();

		pSmoke->SetPosition(pos);
	}

	return pSmoke;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSmoke::Init(void)
{
	CObject3D::Init();

	// 各種設定
	SetMode(MODE::MODE_BILLBOARD);

	int nIdx = Texture::GetIdx("data\\TEXTURE\\EFFECT\\smoke.png");
	SetIdxTexture(nIdx);

	// 移動量設定
	m_move =
	{
		(float)universal::RandRange(10,-10),
		0.0f,
		(float)universal::RandRange(10,-10),
	};

	D3DXVec3Normalize(&m_move, &m_move);

	m_move *= SPEED_MOVE;

	SetSize(500.f, 500.0f);

	SetVtx();

	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSmoke::Uninit(void)
{
	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSmoke::Update(void)
{
	CObject3D::Update();

	// 移動
	D3DXVECTOR3 pos = GetPosition();

	pos += m_move;

	SetPosition(pos);

	// 段々消える処理
	D3DXCOLOR col = GetColor();

	if (!m_bTurn)
	{
		col.a += SPEED_APPER;

		if (col.a >= 1.0f)
			m_bTurn = true;
	}
	else
	{
		col.a -= SPEED_AVOID;

		if (col.a <= 0.0f)
		{
			col.a = 0.0f;

			Uninit();

			return;
		}
	}

	SetColor(col);
}

//=====================================================
// 描画処理
//=====================================================
void CSmoke::Draw(void)
{
	CObject3D::Draw();
}