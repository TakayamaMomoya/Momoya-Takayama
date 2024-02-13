//*****************************************************
//
// 幽霊の処理[ghost.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "ghost.h"
#include "debugproc.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* BODY_PATH[NUM_PLAYER] =
{// プレイヤー見た目のパス
	"data\\MOTION\\motionPotatoman01.txt",
	"data\\MOTION\\motionPotatoman02.txt",
	"data\\MOTION\\motionPotatoman03.txt",
	"data\\MOTION\\motionPotatoman04.txt",
};
const float SPEED_UP = 1.0f;	// 上昇速度
const float TIME_DEATH = 3.0f;	// 消えるまでの時間
}

//=====================================================
// コンストラクタ
//=====================================================
CGhost::CGhost(int nPriority) : CMotion(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CGhost::~CGhost()
{

}

//=====================================================
// 生成処理
//=====================================================
CGhost *CGhost::Create(int nIdxPlayer)
{
	CGhost *pGhost = nullptr;

	if (pGhost == nullptr)
	{
		pGhost = new CGhost;

		if (pGhost != nullptr)
		{
			// 体の読込
			pGhost->Load((char*)BODY_PATH[nIdxPlayer]);

			// 初期化
			pGhost->Init();
		}
	}

	return pGhost;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGhost::Init(void)
{
	// 継承クラスの初期化
	CMotion::Init();

	SetMotion(2);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGhost::Uninit(void)
{
	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CGhost::Update(void)
{
	// 継承クラスの更新
	CMotion::Update();

	// 上昇
	D3DXVECTOR3 pos = GetPosition();

	pos.y += SPEED_UP;

	SetPosition(pos);

	// 色の変化
	D3DXCOLOR col = GetColor();

	float fTick = CManager::GetTick();

	float fRate = fTick / TIME_DEATH;

	col.a -= fRate;

	if (col.a <= 0.0f)
	{
		col.a = 0.0f;

		Uninit();
	}

	SetAllCol(col);
}

//=====================================================
// 描画処理
//=====================================================
void CGhost::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();

#ifdef _DEBUG
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}
#endif
}