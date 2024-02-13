//*****************************************************
//
// 影の処理[shadow.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "game.h"
#include "block.h"
#include "debugproc.h"

//=====================================================
// コンストラクタ
//=====================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CShadow::~CShadow()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CShadow::Init(void)
{
	// 継承クラスの初期化
	CObject3D::Init();

	SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CShadow::Uninit(void)
{
	// 継承クラスの終了
	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CShadow::Update(void)
{
	// 継承クラスの更新
	CObject3D::Update();

	// 変数宣言
	float fHeight = 0.0f;

	// ブロックの高さを取得
	fHeight = CBlock::CheckShadow(GetPosition());

	// 影の位置設定
	SetPosition(D3DXVECTOR3(GetPosition().x, fHeight + 0.1f, GetPosition().z));
}

//=====================================================
// 描画処理
//=====================================================
void CShadow::Draw(void)
{
	// 継承クラスの描画
	CObject3D::Draw();

	CDebugProc::GetInstance()->Print("\n影の位置：[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
}

//=====================================================
// 生成処理
//=====================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, float width, float height)
{
	CShadow *pShadow = nullptr;

	if (pShadow == nullptr)
	{
		pShadow = new CShadow;

		if (pShadow != nullptr)
		{
			pShadow->SetPosition(pos);
			pShadow->SetSize(width, height);

			// 初期化
			pShadow->Init();

			// テクスチャの読込
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\effect000.png");
			pShadow->SetIdxTexture(nIdx);

			pShadow->SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
		}
	}

	return pShadow;
}