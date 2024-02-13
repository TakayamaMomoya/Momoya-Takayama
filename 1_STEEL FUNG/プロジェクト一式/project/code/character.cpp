//*****************************************************
//
// キャラクターの処理[character.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "character.h"
#include "motion.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define BODY_PATH	"data\\MOTION\\rayleigh.txt"	// 体のパス

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CCharacter::CCharacter(int nPriority)
{
	ZeroMemory(&m_info, sizeof(CCharacter::SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CCharacter::~CCharacter()
{

}

//=====================================================
// 生成処理
//=====================================================
CCharacter *CCharacter::Create(char *pPath)
{
	CCharacter *pCharacter = nullptr;

	pCharacter = new CCharacter;

	if (pCharacter != nullptr)
	{
		pCharacter->m_info.pPath = pPath;
		pCharacter->Init();
	}

	return pCharacter;
}

//=====================================================
// 読込処理
//=====================================================
void CCharacter::Load(char *pPath)
{
	if (m_info.pBody == nullptr && pPath != nullptr)
	{
		m_info.pBody = CMotion::Create(pPath);

		if (m_info.pBody != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 rot = GetRotation();

			m_info.pBody->SetPosition(pos);
			m_info.pBody->SetRotation(rot);

			m_info.pBody->MultiplyMtx();
		}
	}
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CCharacter::Init(void)
{
	// 体の読込
	Load(m_info.pPath);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CCharacter::Uninit(void)
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
void CCharacter::Update(void)
{
	if (m_info.pBody != nullptr)
	{// 体の追従
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 rot = GetRotation();

		m_info.pBody->SetPosition(pos);
		m_info.pBody->SetRotation(rot);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CCharacter::Draw(void)
{

}

//=====================================================
// モーション設定
//=====================================================
void CCharacter::SetMotion(int nMotion)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetMotion(nMotion);
	}
}

//=====================================================
// モーション取得
//=====================================================
int CCharacter::GetMotion(void)
{
	int nMotion = 0;

	if (m_info.pBody != nullptr)
	{
		nMotion = m_info.pBody->GetMotion();
	}

	return nMotion;
}