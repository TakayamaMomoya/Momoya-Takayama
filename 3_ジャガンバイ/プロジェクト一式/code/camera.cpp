//*****************************************************
//
// カメラの処理[camera.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "main.h"
#include "camera.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "debugproc.h"
#include "game.h"
#include "playerManager.h"
#include "player.h"
#include "rocket.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MOVE_SPEED					(3.0f)						//移動スピード
#define ROLL_SPEED					(0.02f)						//回転スピード
#define MOVE_FACT					(0.1f)						//移動係数
#define LIMIT_HEIGHT	(50)	// 位置を制限する高さ
#define INITIAL_ANGLE	(45.0f)	// 初期の視野角
#define ANGLE_GAME	(D3DX_PI * 0.4f)	// ゲーム中のカメラの角度
#define RATE_CAMERA_MOVE	(1.5f)	// カメラがどれだけプレイヤーの先を見るかの倍率
#define POSR_BOSS	(D3DXVECTOR3(2742.27f,240.53f,-81.36f))	// ボス戦の注視点位置
#define POSV_BOSS	(D3DXVECTOR3(2741.14f,280.15f,-261.20f))	// ボス戦の視点位置

#define MOVE_FACT_TITLE_ESC	(0.025f)	// タイトルの逃げるときの補正
//====================================================
// 初期化処理
//====================================================
HRESULT CCamera::Init(void)
{
	ZeroMemory(&m_camera,sizeof(Camera));

	m_camera.posV = D3DXVECTOR3(0.0f, 1050.0f, -590.0f);
	m_camera.posVOld = D3DXVECTOR3(0.0f, 30.0f, 100.0f);
	m_camera.posR = D3DXVECTOR3(0.0f, 200.0f, -145.0f);
	m_camera.posVDest = D3DXVECTOR3(0.0f, 1050.0f, -590.0f);
	m_camera.posRDest = D3DXVECTOR3(0.0f, 200.0f, -145.0f);
	m_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.fLength = 100.0f;
	m_camera.fViewAngle = INITIAL_ANGLE;

	float fLength = sqrtf
	(//Y面での対角線
		(m_camera.posV.x - m_camera.posR.x) * (m_camera.posV.x - m_camera.posR.x) + 
		(m_camera.posV.z - m_camera.posR.z) * (m_camera.posV.z - m_camera.posR.z)
	);

	m_camera.fLength = sqrtf
	(//目標までの直線距離
		fLength * fLength + (m_camera.posV.y - m_camera.posR.y) * (m_camera.posV.y - m_camera.posR.y)
	);

	m_camera.rot.x = atan2f(fLength, m_camera.posV.y - m_camera.posR.y);

	SetPosV();

	m_camera.fTimeEvent = 0.0f;

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CCamera::Uninit(void)
{

}

//====================================================
// 更新処理
//====================================================
void CCamera::Update(void)
{

}

//====================================================
// 目標に向かう処理
//====================================================
void CCamera::MoveDist(float fFact)
{
	// 目標位置に補正
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * fFact;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * fFact;
	
	// カウンターの変更
	if (m_camera.fTimeEvent > 0.0f)
	{
		float fTick = CManager::GetTick();

		m_camera.fTimeEvent -= fTick;

		if (m_camera.fTimeEvent <= 0.0f)
		{
			// 初期値に戻す
			m_camera.fTimeEvent = 0.0f;

			m_camera.posVDest = D3DXVECTOR3(0.0f, 1050.0f, -590.0f);
			m_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// ゲームを動かす
			CGame *pGame = CGame::GetInstance();

			if (pGame != nullptr)
			{
				pGame->EnableStop(false);
			}
		}
	}
}

//====================================================
// 操作処理
//====================================================
void CCamera::Control(void)
{
	// 入力取得
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	float fMove = MOVE_SPEED;

	//マウス操作======================================================
	if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == true)
	{//右クリック中、視点旋回
		D3DXVECTOR3 rot;

		//マウスの移動量代入
		rot = { (float)pMouse->GetMoveIX() * ROLL_SPEED, (float)pMouse->GetMoveIY() * ROLL_SPEED, 0.0f };

		D3DXVec3Normalize(&rot, &rot);

		//視点の旋回
		m_camera.rot.y += rot.x * ROLL_SPEED;
		m_camera.rot.x -= rot.y * ROLL_SPEED;

		//注視点を相対位置に設定
		SetPosR();

		if (pKeyboard->GetPress(DIK_LSHIFT) == true)
		{//加速
			fMove *= 3;
		}

		//視点移動===============================================
		if (pKeyboard->GetPress(DIK_A) == true)
		{//左移動
			m_camera.posV.x += sinf(m_camera.rot.y - D3DX_PI * 0.5f) * fMove;
			m_camera.posV.z += cosf(m_camera.rot.y - D3DX_PI * 0.5f) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_D) == true)
		{//右移動
			m_camera.posV.x += sinf(m_camera.rot.y - D3DX_PI * -0.5f) * fMove;
			m_camera.posV.z += cosf(m_camera.rot.y - D3DX_PI * -0.5f) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_W) == true)
		{//前移動
			m_camera.posV.x -= sinf(m_camera.rot.x + D3DX_PI) * sinf(m_camera.rot.y) * fMove;
			m_camera.posV.y += cosf(m_camera.rot.x + D3DX_PI) * MOVE_SPEED;
			m_camera.posV.z -= sinf(m_camera.rot.x + D3DX_PI) * cosf(m_camera.rot.y) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_S) == true)
		{//後移動
			m_camera.posV.x -= sinf(m_camera.rot.x) * sinf(m_camera.rot.y) * fMove;
			m_camera.posV.y += cosf(m_camera.rot.x) * MOVE_SPEED;
			m_camera.posV.z -= sinf(m_camera.rot.x) * cosf(m_camera.rot.y) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_E) == true)
		{//上昇
			m_camera.posV.y += fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_Q) == true)
		{//下降
			m_camera.posV.y -= fMove;
			SetPosR();
		}
	}
}

//====================================================
// タイトルの設定
//====================================================
void CCamera::SetTitle(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	m_camera.posV = posV;
	m_camera.posVDest = posV;

	m_camera.posR = posR;
	m_camera.posRDest = posR;
}

//====================================================
// タイトルの目的地設定
//====================================================
void CCamera::SetTitleDest(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest)
{
	m_camera.posVDest = posVDest;
	m_camera.posRDest = posRDest;
}

//====================================================
// セレクトの設定
//====================================================
void CCamera::SetSelect(void)
{
	D3DXVECTOR3 posV = { 54.0f,320.0f,-760.0f };
	D3DXVECTOR3 posR = { 54.0f,-122.0f,60.0f };

	m_camera.posV = posV;
	m_camera.posVDest = posV;

	m_camera.posR = posR;
	m_camera.posRDest = posR;
}

//====================================================
// ランキングの設定
//====================================================
void CCamera::SetRanking(void)
{
	D3DXVECTOR3 posV = { 0.0f,175.0f,-500.0f };
	D3DXVECTOR3 posR = { 0.0f,175.0f,0.0f };

	m_camera.posV = posV;
	m_camera.posVDest = posV;

	m_camera.posR = posR;
	m_camera.posRDest = posR;
}

//====================================================
// リフトの初期位置
//====================================================
void CCamera::SetLift(void)
{
	D3DXVECTOR3 posV = { 25.0f,80.0f,-130.0f };
	D3DXVECTOR3 posR = { 25.0f,-100.0f,820.0f };

	m_camera.posV = posV;
	m_camera.posVDest = posV;

	m_camera.posR = posR;
	m_camera.posRDest = posR;
}

//====================================================
// 上昇リフト
//====================================================
void CCamera::SetUpLift(void)
{
	if (m_camera.posV.y < 200.0f)
	{
		m_camera.posV.y += 2.0f;
		m_camera.posVDest.y += 2.0f;

		m_camera.posR.y += 2.0f;
		m_camera.posRDest.y += 2.0f;

		return;
	}
	
	m_camera.posV.y += 1.5f;
	m_camera.posVDest.y += 1.5f;
	m_camera.posR.y += 4.5f;
	m_camera.posRDest.y += 4.5f;
}

//====================================================
// 追従処理
//====================================================
void CCamera::FollowPlayer(void)
{
	// プレイヤーの取得
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();

	if (pPlayerManager == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posPlayer = { 0.0f,0.0f,0.0f };

	for (int i = 0;i < NUM_PLAYER;i++)
	{// 一番若い番号のプレイヤーの位置を参照
		CPlayer* pPlayer = pPlayerManager->GetPlayer(i);

		if (pPlayer != nullptr)
		{
			posPlayer = pPlayer->GetPosition();

			break;
		}
	}

	// 目的座標設定
	m_camera.posRDest = posPlayer;
	m_camera.posVDest = 
	{
		m_camera.posRDest.x,
		m_camera.posRDest.y + 70.0f,
		m_camera.posRDest.z - 100.0f,
	};

	// 目的座標に補正
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT;
}

//====================================================
// タイトルのカメラ挙動
//====================================================
void CCamera::UpdateTitle(D3DXVECTOR3 move)
{
	// カメラの移動処理
	m_camera.posVDest = m_camera.posVDest + move;

	// 目的座標に補正
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT_TITLE_ESC;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT_TITLE_ESC;
}

//====================================================
// タイトルの逃げるときのカメラ挙動
//====================================================
void CCamera::UpdateTitleEsc(D3DXVECTOR3 move)
{
	// カメラの移動処理
	m_camera.posVDest = m_camera.posVDest + move;
	m_camera.posRDest = m_camera.posRDest + move;

	// 目的座標に補正
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT_TITLE_ESC;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT_TITLE_ESC;
}

//====================================================
// リザルトのカメラ挙動
//====================================================
void CCamera::UpdateResult(void)
{
	// ロケットの座標取得
	CRocket *pRocket = CRocket::GetInstance();

	D3DXVECTOR3 posTarget = { 0.0f,0.0f,0.0f };

	if (pRocket != nullptr)
	{
		posTarget = pRocket->GetPosition();
	}
	else
	{
		return;
	}

	// 目的座標設定
	m_camera.posRDest = posTarget;
	m_camera.posVDest =
	{
		0.0f,
		500.0f,
		-300.0f,
	};

	// 目的座標に補正
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT;
}

//====================================================
// ゲームオーバーのカメラ挙動
//====================================================
void CCamera::UpdateGameover(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest)
{
	// 目的座標設定
	m_camera.posRDest = posRDest;
	m_camera.posVDest = posVDest;

	// 目的座標に補正
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT;
}

//====================================================
// 振動処理
//====================================================
void CCamera::Quake(void)
{
	//画面振動
	if (m_camera.nTimerQuake > 0)
	{
		//時間を減らす
		m_camera.nTimerQuake--;

		//視点注視点を動かしてカメラを揺らす
		m_camera.posV.x += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * sinf(m_camera.rot.y);
		m_camera.posV.y += (float)(rand() % 101 - 50) * m_camera.fQuakeSizeV;
		m_camera.posV.z += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * cosf(m_camera.rot.y);

		m_camera.posR.x += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * sinf(m_camera.rot.y);
		m_camera.posR.y += (float)(rand() % 101 - 50) * m_camera.fQuakeSizeV;
		m_camera.posR.z += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * cosf(m_camera.rot.y);

		//徐々に揺れのサイズを小さくする
		m_camera.fQuakeSizeH += (0.0f - m_camera.fQuakeSizeH) * 0.03f;
		m_camera.fQuakeSizeV += (0.0f - m_camera.fQuakeSizeV) * 0.03f;
	}
}

//====================================================
// 振動設定
//====================================================
void CCamera::SetQuake(float fQuakeSizeH, float fQuakeSizeV, int nTime)
{
	if (fQuakeSizeH > 0.0f && fQuakeSizeV > 0.0f)
	{
		//揺れる時間代入
		m_camera.nTimerQuake = nTime;

		//揺れの大きさ代入
		m_camera.fQuakeSizeH = fQuakeSizeH;
		m_camera.fQuakeSizeV = fQuakeSizeV;
	}
}

//====================================================
// 視点設定
//====================================================
void CCamera::SetPosV(void)
{
	m_camera.posV =
	{
		m_camera.posR.x + sinf(m_camera.rot.x) * sinf(m_camera.rot.y + D3DX_PI) * m_camera.fLength,
		m_camera.posR.y + cosf(m_camera.rot.x) * m_camera.fLength,
		m_camera.posR.z + sinf(m_camera.rot.x) * cosf(m_camera.rot.y + D3DX_PI) * m_camera.fLength
	};
}

//====================================================
// 注視点設定
//====================================================
void CCamera::SetPosR(void)
{
	m_camera.posR =
	{
		m_camera.posV.x - sinf(m_camera.rot.x) * sinf(m_camera.rot.y + D3DX_PI) * m_camera.fLength,
		m_camera.posV.y - cosf(m_camera.rot.x) * m_camera.fLength,
		m_camera.posV.z - sinf(m_camera.rot.x) * cosf(m_camera.rot.y + D3DX_PI) * m_camera.fLength
	};
}

//====================================================
// 設定処理
//====================================================
void CCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//プロジェクションマトリックス==================================================
	//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&m_camera.mtxProjection);

	//プロジェクションマトリックス作成
	D3DXMatrixPerspectiveFovLH(&m_camera.mtxProjection,
		D3DXToRadian(m_camera.fViewAngle),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		MIN_DRAW,
		MAX_DRAW);

	/*D3DXMatrixOrthoLH(&m_camera.mtxProjection,	// 平行投影のやり方
		720,	// 高さ
		1280,	// 幅
		MIN_DRAW,
		MAX_DRAW);*/

	//マトリックス設定
	pDevice->SetTransform(D3DTS_PROJECTION,&m_camera.mtxProjection);

	//ビューマトリックス============================================================
	//ビューマトリックス初期化
	D3DXMatrixIdentity(&m_camera.mtxView);

	//ビューマトリックス作成
	D3DXMatrixLookAtLH(&m_camera.mtxView,
		&m_camera.posV,
		&m_camera.posR,
		&m_camera.vecU);

	//ビューマトリックス設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);

#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\n視点の位置：[%f,%f,%f]", m_camera.posV.x, m_camera.posV.y, m_camera.posV.z);
	CDebugProc::GetInstance()->Print("\n視点の目標位置：[%f,%f,%f]", m_camera.posVDest.x, m_camera.posVDest.y, m_camera.posVDest.z);
	CDebugProc::GetInstance()->Print("\n注視点の位置：[%f,%f,%f]", m_camera.posR.x, m_camera.posR.y, m_camera.posR.z);
	CDebugProc::GetInstance()->Print("\n注視点の目標位置：[%f,%f,%f]", m_camera.posRDest.x, m_camera.posRDest.y, m_camera.posRDest.z);
	CDebugProc::GetInstance()->Print("\nカメラ距離：[%f]", m_camera.fLength);
#endif
}

//====================================================
// 情報取得
//====================================================
CCamera::Camera *CCamera::GetCamera(void)
{
	return &m_camera;
}