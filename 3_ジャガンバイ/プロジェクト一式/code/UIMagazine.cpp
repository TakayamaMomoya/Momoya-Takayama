//*****************************************************
//
// 残弾数表示の処理[UIMagazine.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "UIMagazine.h"
#include "UI.h"
#include "number.h"

#include "texture.h"

#include "player.h"
#include "playerManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int DIG_MAG_NUM = 3;		// 装弾数の桁数
}

//=====================================================
// コンストラクタ
//=====================================================
CUIMagazine::CUIMagazine(int nPriority) : CObject(nPriority)
{
	m_pNumDig = nullptr;

	ZeroMemory(&m_info, sizeof(m_info));
}

//=====================================================
// デストラクタ
//=====================================================
CUIMagazine::~CUIMagazine()
{

}

//=====================================================
// 生成処理
//=====================================================
CUIMagazine* CUIMagazine::Create(int nIdx)
{
	CUIMagazine *pMagazine = nullptr;

	pMagazine = new CUIMagazine;

	if (pMagazine != nullptr)
	{
		// プレイヤー番号を設定
		pMagazine->m_info.nIdxPlayer = nIdx;

		// 装弾数枠の生成処理
		CUI* pUIMagazineFrame = CUI::Create();

		if (pUIMagazineFrame != nullptr)
		{
			pMagazine->m_pUIMagazineFrame = pUIMagazineFrame;
		}

		// 装弾数の生成処理
		CUI* pUIMagazine = CUI::Create();

		if (pUIMagazine != nullptr)
		{
			pMagazine->m_pUIMagazine = pUIMagazine;
		}

		// 装弾数（数字）の生成処理
		CNumber* pDigMag = CNumber::Create(DIG_MAG_NUM, 0);

		if (pDigMag != nullptr)
		{
			pMagazine->m_pNumDig = pDigMag;
		}

		// 装弾数UIの初期化
		pMagazine->Init();
	}

	return pMagazine;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CUIMagazine::Init(void)
{
	if (m_pNumDig != nullptr &&
		m_pUIMagazine != nullptr &&
		m_pUIMagazineFrame != nullptr)
	{
		// 初期値設定処理（設定してなかったらこれ）
		m_pUIMagazineFrame->SetSize(50.0f, 100.0f);
		m_pUIMagazineFrame->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pUIMagazine->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pUIMagazine->SetSize(50.0f, 100.0f);
		m_pNumDig->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pNumDig->SetSizeAll(50.0f, 100.0f);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CUIMagazine::Uninit(void)
{
	if (m_pNumDig != nullptr &&
		m_pUIMagazine != nullptr &&
		m_pUIMagazineFrame != nullptr)
	{
		m_pUIMagazineFrame->Uninit();
		m_pUIMagazine->Uninit();
		m_pNumDig->Uninit();
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CUIMagazine::Update(void)
{
	if (m_pNumDig != nullptr)
	{
		SetNumMagazine();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CUIMagazine::Draw(void)
{

}

//=====================================================
// 位置設定処理
//=====================================================
void CUIMagazine::SetPosition(D3DXVECTOR3 posUIMagazine,D3DXVECTOR3 posUIMagazineFrame,D3DXVECTOR3 posNum)
{
	if (m_pNumDig != nullptr &&
		m_pUIMagazine != nullptr &&
		m_pUIMagazineFrame != nullptr)
	{
		m_info.posUIMagazine = posUIMagazine;
		m_info.posUIMagazineFrame = posUIMagazineFrame;
		m_info.posNum = posNum;

		m_pUIMagazine->SetPosition(posUIMagazine);
		m_pUIMagazineFrame->SetPosition(posUIMagazineFrame);
		m_pNumDig->SetPosition(posNum);

		SetVtxGage();
		m_pUIMagazineFrame->SetVtx();
	}
}

//=====================================================
// 大きさ設定処理
//=====================================================
void CUIMagazine::SetSize(float width, float height, float widthNum, float heightNum)
{
	if (m_pNumDig != nullptr)
	{
		m_info.fWidth = width;
		m_info.fHeight = height;

		m_info.fWidthNum = widthNum;
		m_info.fHeightNum = heightNum;

		m_pUIMagazine->SetSize(width, height);
		m_pUIMagazineFrame->SetSize(width, height);
		m_pNumDig->SetSizeAll(widthNum, heightNum);

		SetVtxGage();
		m_pUIMagazineFrame->SetVtx();
	}
}

//=====================================================
// 色設定処理
//=====================================================
void CUIMagazine::SetCol(D3DXCOLOR colMagazine, D3DXCOLOR colMagazineFrame, D3DXCOLOR colNum)
{
	if (m_pNumDig != nullptr)
	{
		m_info.colMagazine = colMagazine;
		m_info.colMagazineFrame = colMagazineFrame;
		m_info.colNum = colNum;

		m_pUIMagazine->SetCol(colMagazine);
		m_pUIMagazineFrame->SetCol(colMagazineFrame);
		m_pNumDig->SetColor(colNum);

		SetVtxGage();
	}
}

//=====================================================
// テクスチャ設定処理
//=====================================================
void CUIMagazine::SetTexture(const char* pFileName)
{
	if (m_pUIMagazine != nullptr &&
		m_pUIMagazineFrame != nullptr)
	{
		CTexture* pTexture = CTexture::GetInstance();

		if (pTexture != nullptr)
		{
			m_info.nIdxTexture = pTexture->Regist(pFileName);
		}

		m_pUIMagazine->SetIdxTexture(m_info.nIdxTexture);
		m_pUIMagazineFrame->SetIdxTexture(m_info.nIdxTexture);

		SetVtxGage();
		m_pUIMagazineFrame->SetVtx();
	}
}

//=====================================================
// 装弾数設定処理
//=====================================================
void CUIMagazine::SetNumMagazine(void)
{
	// ポインタの取得
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	CPlayer* pPlayer = nullptr;
	CWeapon* pWeapon = nullptr;

	int nMaxBullet = 0;

	if (pPlayerManager != nullptr)
	{
		// プレイヤー管理の取得
		pPlayer = pPlayerManager->GetPlayer(m_info.nIdxPlayer);
	}

	if (pPlayer != nullptr)
	{
		// プレイヤーの取得
		pWeapon = pPlayer->GetWeapon();
	}

	if (pWeapon != nullptr)
	{
		// 現在の装弾数と前回の装弾数が違う
		if (m_info.nBullet != pWeapon->GetBullet())
		{
			// プレイヤーの弾を取得
			m_info.nBullet = pWeapon->GetBullet();
			nMaxBullet = pWeapon->GetMaxBullet();

			// 色変更フラグをオン
			m_info.bColorChange = true;
			
			// 現在の装弾数の割合を算出
			float fLifeRatio = (1.0f - ((float)m_info.nBullet / (float)nMaxBullet));

			// 縦幅の減算量を算出
			m_info.fHeightSub = fLifeRatio * m_info.fHeight;

			// 設定処理
			m_pUIMagazine->SetTex(D3DXVECTOR2(0.0f, fLifeRatio), D3DXVECTOR2(1.0f, 1.0f));
			SetVtxGage();
			m_pUIMagazineFrame->SetVtx();
		}
	}
	else
	{
		// 弾は0
		m_info.nBullet = 0;

		// 現在の装弾数の割合を算出
		float fLifeRatio = (1.0f - ((float)m_info.nBullet / (float)nMaxBullet));

		// 縦幅の減算量を算出
		m_info.fHeightSub = fLifeRatio * m_info.fHeight;

		// 設定処理
		m_pUIMagazine->SetTex(D3DXVECTOR2(0.0f, fLifeRatio), D3DXVECTOR2(1.0f, 1.0f));
		SetVtxGage();
		m_pUIMagazineFrame->SetVtx();
	}

	if (m_pNumDig != nullptr)
	{
		// 弾を反映
		m_pNumDig->SetValue(m_info.nBullet, DIG_MAG_NUM);
	}
}

//=====================================================
// ゲージ用の頂点情報設定処理
//=====================================================
void CUIMagazine::SetVtxGage(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	if (m_pUIMagazine != nullptr)
	{
		if (m_pUIMagazine->GetVtxBuff() != nullptr)
		{
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			m_pUIMagazine->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

			// 対角線の角度取得
			float fAngleUp = atan2f(m_info.fWidth, m_info.fHeight);
			// 長さの取得
			float fLengthUp = sqrtf(m_info.fWidth * m_info.fWidth + m_info.fHeight * m_info.fHeight);

			// 減算量の長さを取得
			float fLengthSub = sqrtf(m_info.fWidthSub * m_info.fWidthSub + m_info.fHeightSub * m_info.fHeightSub);

			// 誤差範囲を消す
			if (fLengthUp - fLengthSub <= 1.0f)
			{
				fLengthUp = 0.0f;
				fLengthSub = 0.0f;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3
			(
				m_info.posUIMagazine.x - m_info.fWidth,
				m_info.posUIMagazine.y + cosf(0.0f - D3DX_PI + fAngleUp) * (fLengthUp * 2.0f) + (fLengthSub * 2.0f),
				0.0f
			);
			pVtx[1].pos = D3DXVECTOR3
			(
				m_info.posUIMagazine.x + m_info.fWidth,
				m_info.posUIMagazine.y + cosf(0.0f + D3DX_PI - fAngleUp) * (fLengthUp * 2.0f) + (fLengthSub * 2.0f),
				0.0f
			);
			pVtx[2].pos = D3DXVECTOR3
			(
				m_info.posUIMagazine.x - m_info.fWidth,
				m_info.posUIMagazine.y + cosf(0.0f - fAngleUp),
				0.0f
			);
			pVtx[3].pos = D3DXVECTOR3
			(
				m_info.posUIMagazine.x + m_info.fWidth,
				m_info.posUIMagazine.y + cosf(0.0f + fAngleUp),
				0.0f
			);


			// 頂点バッファのアンロック
			m_pUIMagazine->GetVtxBuff()->Unlock();
		}
	}
}