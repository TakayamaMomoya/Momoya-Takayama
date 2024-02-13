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
#include "player.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MOVE_SPEED					(3.0f)						//移動スピード
#define ROLL_SPEED					(0.02f)						//回転スピード
#define MOVE_FACT					(0.3f)						//移動減衰係数
#define LIMIT_HEIGHT	(50)	// 位置を制限する高さ
#define INITIAL_ANGLE	(45.0f)	// 初期の視野角
#define ANGLE_GAME	(D3DX_PI * 0.4f)	// ゲーム中のカメラの角度
#define RATE_CAMERA_MOVE	(1.5f)	// カメラがどれだけプレイヤーの先を見るかの倍率
#define POSR_BOSS	(D3DXVECTOR3(2742.27f,240.53f,-81.36f))	// ボス戦の注視点位置
#define POSV_BOSS	(D3DXVECTOR3(2741.14f,280.15f,-261.20f))	// ボス戦の視点位置

//====================================================
// 初期化処理
//====================================================
HRESULT CCamera::Init(void)
{
	ZeroMemory(&m_camera,sizeof(Camera));

	m_camera.posV = D3DXVECTOR3(0.0f, 50.0f, 500.0f);
	m_camera.posVOld = D3DXVECTOR3(0.0f, 30.0f, 500.0f);
	m_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.fLength = 0.0f;
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
	CScene::MODE mode;

	mode = CManager::GetMode();

	switch (mode)
	{
	case CScene::MODE_TITLE:
		UpdateTitle();
		break;
	case CScene::MODE_GAME:
		UpdateGame();
		break;
	case CScene::MODE_RANKING:
		break;
	default:
		break;
	}
}

//====================================================
// デバッグ処理
//====================================================
void CCamera::Debug(void)
{
	// 情報取得
	//CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	//if (pKeyboard == nullptr)
	//{
	//	return;
	//}

	//if (pKeyboard->GetPress(DIK_UP))
	//{
	//	m_camera.fViewAngle += 0.4f;
	//}
	//if (pKeyboard->GetPress(DIK_DOWN))
	//{
	//	m_camera.fViewAngle -= 0.4f;
	//}
}

//====================================================
// タイトル更新
//====================================================
void CCamera::UpdateTitle(void)
{
	TitleMove();
}

//====================================================
// ゲーム更新
//====================================================
void CCamera::UpdateGame(void)
{

}

//====================================================
// カメラの動き
//====================================================
void CCamera::TitleMove(void)
{
	//目標の注視点設定
	m_camera.posRDest =
	{
		0.0f,
		180.0f,
		0.0f
	};

	m_camera.fLength = 240;

	//目標の視点設定
	m_camera.posVDest =
	{
		m_camera.posRDest.x + sinf(m_camera.rot.x) * sinf(m_camera.rot.y - D3DX_PI * 0.02f) * m_camera.fLength,
		m_camera.posRDest.y + cosf(m_camera.rot.x) * m_camera.fLength,
		m_camera.posRDest.z + sinf(m_camera.rot.x) * cosf(m_camera.rot.y - D3DX_PI * 0.02f) * m_camera.fLength
	};

	//注視点補正
	m_camera.posR.x += (m_camera.posRDest.x - m_camera.posR.x) * MOVE_FACT;
	m_camera.posR.y += (m_camera.posRDest.y - m_camera.posR.y) * MOVE_FACT;
	m_camera.posR.z += (m_camera.posRDest.z - m_camera.posR.z) * MOVE_FACT;

	//現在の視点補正
	m_camera.posV.x += (m_camera.posVDest.x - m_camera.posV.x) * MOVE_FACT;
	m_camera.posV.y += (m_camera.posVDest.y - m_camera.posV.y) * MOVE_FACT;
	m_camera.posV.z += (m_camera.posVDest.z - m_camera.posV.z) * MOVE_FACT;

	m_camera.rot.y += 0.01f;
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
			m_camera.posV.x -= sinf(m_camera.rot.y - D3DX_PI * 0.5f) * fMove;
			m_camera.posV.z -= cosf(m_camera.rot.y - D3DX_PI * 0.5f) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_D) == true)
		{//右移動
			m_camera.posV.x -= sinf(m_camera.rot.y - D3DX_PI * -0.5f) * fMove;
			m_camera.posV.z -= cosf(m_camera.rot.y - D3DX_PI * -0.5f) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_W) == true)
		{//前移動
			m_camera.posV.x += sinf(m_camera.rot.x + D3DX_PI) * sinf(m_camera.rot.y) * fMove;
			m_camera.posV.y += cosf(m_camera.rot.x + D3DX_PI) * MOVE_SPEED;
			m_camera.posV.z += sinf(m_camera.rot.x + D3DX_PI) * cosf(m_camera.rot.y) * fMove;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_S) == true)
		{//後移動
			m_camera.posV.x += sinf(m_camera.rot.x) * sinf(m_camera.rot.y) * fMove;
			m_camera.posV.y += cosf(m_camera.rot.x) * MOVE_SPEED;
			m_camera.posV.z += sinf(m_camera.rot.x) * cosf(m_camera.rot.y) * fMove;
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
void CCamera::SetTitle(void)
{
	D3DXVECTOR3 posV = { -1.29f,43.68f,475.58f };
	D3DXVECTOR3 posR = { 106.19f,-62.93f,-3.55f };

	m_camera.posV = posV;
	m_camera.posVDest = posV;

	m_camera.posR = posR;
	m_camera.posRDest = posR;
}

//====================================================
// 追従処理
//====================================================
void CCamera::FollowPlayer(void)
{
	// プレイヤー取得
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	Quake();

	D3DXVECTOR3 pos = pPlayer->GetPosition();
	D3DXVECTOR3 move = pPlayer->GetMove();

	m_camera.posRDest = pos + move * RATE_CAMERA_MOVE;

	m_camera.posVDest =
	{
		m_camera.posRDest.x + sinf(ANGLE_GAME) * sinf(D3DX_PI) * m_camera.fLength,
		m_camera.posRDest.y + cosf(ANGLE_GAME) * m_camera.fLength,
		m_camera.posRDest.z + sinf(ANGLE_GAME) * cosf(D3DX_PI) * m_camera.fLength
	};

	// 位置の補正
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT;
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT;
}

//====================================================
// ボス戦時の動き
//====================================================
void CCamera::BossBattle(void)
{
	// 振動処理
	Quake();

	m_camera.posRDest = POSR_BOSS;
	m_camera.posVDest = POSV_BOSS;

	// 位置の補正
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * MOVE_FACT;
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * MOVE_FACT;
}

//====================================================
// リザルトのカメラ挙動
//====================================================
void CCamera::UpdateResult(void)
{

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
		m_camera.posR.x + sinf(m_camera.rot.x) * sinf(m_camera.rot.y) * m_camera.fLength,
		m_camera.posR.y + cosf(m_camera.rot.x) * m_camera.fLength,
		m_camera.posR.z + sinf(m_camera.rot.x) * cosf(m_camera.rot.y) * m_camera.fLength
	};
}

//====================================================
// 注視点設定
//====================================================
void CCamera::SetPosR(void)
{
	m_camera.posR =
	{
		m_camera.posV.x - sinf(m_camera.rot.x) * sinf(m_camera.rot.y) * m_camera.fLength,
		m_camera.posV.y - cosf(m_camera.rot.x) * m_camera.fLength,
		m_camera.posV.z - sinf(m_camera.rot.x) * cosf(m_camera.rot.y) * m_camera.fLength
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
	CDebugProc::GetInstance()->Print("\n注視点の位置：[%f,%f,%f]", m_camera.posR.x, m_camera.posR.y, m_camera.posR.z);
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