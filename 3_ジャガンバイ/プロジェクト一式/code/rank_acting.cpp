//*****************************************************
//
// ランキングの処理[Ranking.cpp]
// Author:酒井南勝
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************

#include "rank_acting.h"
#include "main.h"

#include <stdio.h>

#include "manager.h"
#include "camera.h"
#include "record.h"

#include "object2D.h"
#include "number.h"

#include "object3D.h"
#include "motion.h"

#include "texture.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputmouse.h"
#include "sound.h"

#include "fade.h"

//*****************************************************
// マクロ定義
//*****************************************************

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	// フィールド
	const float FIELD_WIDTH = 1000.0f;		// フィールドの幅
	const float FIELD_HEIGHT = 500.0f;		// フィールドの高さ
	const char* FIELD_FILE_NAME = "data\\TEXTURE\\MATERIAL\\WodenWall_00.jpg";	// フィールドのパス

	// ウォール
	const float WALL_WIDTH = 800.0f;		// ウォールの幅
	const float WALL_HEIGHT = 500.0f;		// ウォールの高さ
	const char* WALL_FILE_NAME = "data\\TEXTURE\\MATERIAL\\concrete_00.jpg";	// ウォールのパス

	// カメラ
	const D3DXVECTOR3 CAMERA_POS_V = D3DXVECTOR3(0.0f, 200.0f, -800.0f);
	const D3DXVECTOR3 CAMERA_POS_R = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	const D3DXVECTOR3 CAMERA_MOVE = D3DXVECTOR3(0.0f, 0.0f, 7.0f);

	// 役者
	const char* PLAYER_BODY_PATH[CRankActing::ACTOR_TYPE_MAX] =
	{// 役者の体のパス[役者番号]
		"data\\MOTION\\motionRanking01.txt",
		"data\\MOTION\\motionRanking02.txt",
		"data\\MOTION\\motionRanking03.txt",
		"data\\MOTION\\motionRanking04.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
		"data\\MOTION\\motionEnemy00.txt",
	};
	const D3DXVECTOR3 PLAYER_POS[CRankActing::ACTING_TYPE_MAX][CRankActing::ACTOR_TYPE_MAX] =
	{// 役者の位置[役者番号][演技番号]

		// 行進
		D3DXVECTOR3(500.0f, 0.0f, 0.0f),
		D3DXVECTOR3(600.0f, 0.0f, 0.0f),
		D3DXVECTOR3(700.0f, 0.0f, 0.0f),
		D3DXVECTOR3(800.0f, 0.0f, 0.0f),
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし

		// 逃げる
		D3DXVECTOR3(500.0f, 0.0f, 0.0f),
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(600.0f, 0.0f, 0.0f),
		D3DXVECTOR3(650.0f, 0.0f, 30.0f),
		D3DXVECTOR3(675.0f, 0.0f, -30.0f),

		// 追いかける
		D3DXVECTOR3(-600.0f, 0.0f, 0.0f),
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(1000.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(1050.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(-500.0f, 0.0f,  0.0f),

	};
	const D3DXVECTOR3 PLAYER_ROT[CRankActing::ACTING_TYPE_MAX][CRankActing::ACTOR_TYPE_MAX] =
	{// 役者の向き[役者番号][演技番号]

		// 行進
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし

		// 逃げる
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),

		// 追いかける
		D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f),

	};
	const D3DXVECTOR3 PLAYER_MOVE[CRankActing::ACTING_TYPE_MAX][CRankActing::ACTOR_TYPE_MAX] =
	{// 役者の移動量[役者番号][演技番号]

		// 行進
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし

		// 逃げる
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-4.0f, 0.0f, 0.0f),

		// 追いかける
		D3DXVECTOR3(4.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 出番なし
		D3DXVECTOR3(4.0f, 0.0f, 0.0f),
	};

	const int CHANGE_COUNT = 300;	// 演技を変えるまでの時間
}

//=====================================================
// コンストラクタ
//=====================================================
CRankActing::CRankActing()
{
	ZeroMemory(&m_infoVisualObj, sizeof(m_infoVisualObj));
	m_typeActing = (ACTING_TYPE)0;
	m_nActingCnt = 0;


}

//=====================================================
// デストラクタ
//=====================================================
CRankActing::~CRankActing()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRankActing::Init(void)
{
	// オブジェクトの初期化処理
	if (InitObj() != S_OK)
	{
		return E_FAIL;
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRankActing::Uninit(void)
{

}

//=====================================================
// 更新処理
//=====================================================
void CRankActing::Update(void)
{
	// 演技の更新
	for (int nCount = 0; nCount < ACTOR_TYPE_MAX; nCount++)
	{
		if (m_infoVisualObj.apModelActor[nCount] != nullptr)
		{
			D3DXVECTOR3 playerMove = m_infoVisualObj.apModelActor[nCount]->GetPosition() + PLAYER_MOVE[m_typeActing][nCount];

			m_infoVisualObj.apModelActor[nCount]->SetPosition(playerMove);
		}
	}

	// 演技切り替えカウント処理
	CountActing();
}

//=====================================================
// 描画処理
//=====================================================
void CRankActing::Draw(void)
{

}

//=====================================================
// 生成処理
//=====================================================
CRankActing* CRankActing::Create(void)
{
	CRankActing* pRankActing = new CRankActing;

	if (pRankActing != nullptr)
	{
		// ランクの初期化
		if (pRankActing->Init() != S_OK)
		{
			return nullptr;
		}
	}

	return pRankActing;
}

//=====================================================
// オブジェクトの初期化処理
//=====================================================
HRESULT CRankActing::InitObj(void)
{
	// 情報取得
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		// ランキングの設定
		pCamera->SetRanking();
	}
	else if (pCamera == nullptr)
	{
		return E_FAIL;
	}

	// 地面の生成
	m_infoVisualObj.pField = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_infoVisualObj.pField != nullptr)
	{
		m_infoVisualObj.pField->SetSize(FIELD_WIDTH, FIELD_HEIGHT);
		int nIdx = CTexture::GetInstance()->Regist(FIELD_FILE_NAME);
		m_infoVisualObj.pField->SetIdxTexture(nIdx);
	}
	else if (m_infoVisualObj.pField == nullptr)
	{
		return E_FAIL;
	}

	// 地面の生成
	m_infoVisualObj.pWall = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_infoVisualObj.pWall != nullptr)
	{
		m_infoVisualObj.pWall->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 200.0f));
		m_infoVisualObj.pWall->SetSize(WALL_WIDTH, WALL_HEIGHT);
		m_infoVisualObj.pWall->SetRot(D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f));
		int nIdx = CTexture::GetInstance()->Regist(WALL_FILE_NAME);
		m_infoVisualObj.pWall->SetIdxTexture(nIdx);
	}
	else if (m_infoVisualObj.pWall == nullptr)
	{
		return E_FAIL;
	}

	for (int nCount = 0; nCount < ACTOR_TYPE_MAX; nCount++)
	{
		// 役者の生成
		m_infoVisualObj.apModelActor[nCount] = CMotion::Create((char*)PLAYER_BODY_PATH[nCount]);

		if (m_infoVisualObj.apModelActor[nCount] != nullptr)
		{
			m_infoVisualObj.apModelActor[nCount]->SetPosition(PLAYER_POS[m_typeActing][nCount]);
			m_infoVisualObj.apModelActor[nCount]->SetRot(PLAYER_ROT[m_typeActing][nCount]);
			m_infoVisualObj.apModelActor[nCount]->SetMotion(1);
		}
		else if (m_infoVisualObj.apModelActor[nCount] == nullptr)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=====================================================
// 演技の設定処理
//=====================================================
void CRankActing::SetObjActing(void)
{
	for (int nCount = 0; nCount < ACTOR_TYPE_MAX; nCount++)
	{
		if (m_infoVisualObj.apModelActor[nCount] != nullptr)
		{
			m_infoVisualObj.apModelActor[nCount]->SetPosition(PLAYER_POS[m_typeActing][nCount]);
			m_infoVisualObj.apModelActor[nCount]->SetRot(PLAYER_ROT[m_typeActing][nCount]);
			m_infoVisualObj.apModelActor[nCount]->SetMotion(1);
		}
	}
}

//=====================================================
// 演技の切り替えカウント処理
//=====================================================
void CRankActing::CountActing(void)
{
	// 演技カウントが設定値以下
	if (m_nActingCnt <= CHANGE_COUNT)
	{
		// カウントを加算
		m_nActingCnt++;
	}
	else
	{
		// カウントの初期化
		m_nActingCnt = 0;

		// 演技番号を増やす
		m_typeActing = (ACTING_TYPE)(m_typeActing + 1);

		// 演技が最大数
		if (m_typeActing >= ACTING_TYPE_MAX)
		{
			// 演技を初期化
			m_typeActing = (ACTING_TYPE)0;
		}

		// 演技を設定
		SetObjActing();
	}
}