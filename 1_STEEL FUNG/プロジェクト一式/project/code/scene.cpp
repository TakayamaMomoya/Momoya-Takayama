//*****************************************************
//
// シーン処理[scene.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"
#include "tutorial.h"
#include "game.h"
#include "title.h"
#include "ranking.h"
#include "manager.h"
#include "renderer.h"

//=====================================================
// コンストラクタ
//=====================================================
CScene::CScene()
{
	m_nTimerTrans = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CScene::~CScene()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CScene::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CScene::Uninit(void)
{

}

//=====================================================
// 更新処理
//=====================================================
void CScene::Update(void)
{
	// レンダラーの取得
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		// レンダラーの更新
		pRenderer->Update();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CScene::Draw(void)
{

}

//=====================================================
// シーンの生成処理
//=====================================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = nullptr;

	switch (mode)
	{
	case CScene::MODE_TITLE:

		pScene = new CTitle;

		break;
	case CScene::MODE_TUTORIAL:

		pScene = new CTutorial;

		break;
	case CScene::MODE_GAME:

		pScene = new CGame;

		break;
	case CScene::MODE_RANKING:

		pScene = new CRanking;

		break;
	default:

		pScene = new CGame;

		break;
	}

	if (pScene != nullptr)
	{// シーン生成
		pScene->Init();
	}

	return pScene;
}