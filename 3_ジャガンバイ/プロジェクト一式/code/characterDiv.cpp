//*****************************************************
//
// 分割キャラクターの処理[character.h]
// Author:酒井南勝
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "characterDiv.h"
#include "motionDiv.h"
#include "renderer.h"
#include "universal.h"

//*****************************************************
// マクロ定義
//*****************************************************

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CCharacterDiv::CCharacterDiv(int nPriority)
{
	ZeroMemory(&m_info, sizeof(CCharacterDiv::SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CCharacterDiv::~CCharacterDiv()
{

}

//=====================================================
// 生成処理
//=====================================================
CCharacterDiv* CCharacterDiv::Create(char* pPathLower, char* pPathUpper)
{
	CCharacterDiv* pCharacter = nullptr;

	pCharacter = new CCharacterDiv;

	if (pCharacter != nullptr)
	{
		pCharacter->m_info.apPath[PARTS_LOWER] = pPathLower;
		pCharacter->m_info.apPath[PARTS_UPPER] = pPathUpper;
		pCharacter->Init();
	}

	return pCharacter;
}

//=====================================================
// 読込処理
//=====================================================
void CCharacterDiv::Load(char* pPathLower, char* pPathUpper)
{
	if (m_info.pBody == nullptr && pPathLower != nullptr && pPathUpper != nullptr)
	{
		m_info.pBody = CMotionDiv::Create(pPathLower, pPathUpper);

		if (m_info.pBody != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 rot = GetRot();

			m_info.pBody->SetPosition(pos);
			m_info.pBody->SetRot(rot);
		}
	}
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CCharacterDiv::Init(void)
{
	return S_OK;
}

//=====================================================
// 位置の設定
//=====================================================
void CCharacterDiv::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;
	
	if (m_info.pBody != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		m_info.pBody->SetPosition(pos);
	}
}

//=====================================================
// 終了処理
//=====================================================
void CCharacterDiv::Uninit(void)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody->Uninit();
		m_info.pBody = nullptr;
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CCharacterDiv::Update(void)
{
	if (m_info.pBody != nullptr)
	{// 体の追従
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 rot = GetRot();

		m_info.pBody->SetPosition(pos);
		m_info.pBody->SetRot(rot);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CCharacterDiv::Draw(void)
{
	//変数宣言
	D3DXMATRIX mtxRotModel, mtxTransModel;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_info.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
		m_info.rot.y, m_info.rot.x, m_info.rot.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxRotModel);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel,
		m_info.pos.x, m_info.pos.y, m_info.pos.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxTransModel);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_info.mtxWorld);
}

//=====================================================
// モーション設定
//=====================================================
void CCharacterDiv::SetMotion(int nNum, int nMotion)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetMotion(nNum,nMotion);
	}
}

//=====================================================
// モーション取得
//=====================================================
int CCharacterDiv::GetMotion(int nNum)
{
	int nMotion = 0;

	if (m_info.pBody != nullptr)
	{
		nMotion = m_info.pBody->GetMotion(nNum);
	}

	return nMotion;
}