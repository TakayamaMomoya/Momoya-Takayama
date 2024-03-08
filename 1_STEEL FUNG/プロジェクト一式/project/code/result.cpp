//*****************************************************
//
// リザルトの処理[result.cpp]
// Author:高山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "result.h"
#include "resultBehavior.h"
#include "fade.h"
#include "inputManager.h"
#include "object.h"
#include "object2D.h"
#include "meshcylinder.h"
#include "texture.h"
#include "camera.h"
#include "manager.h"
#include "saveDataManager.h"
#include <fstream>
#include <sstream>
#include <string>

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_ROTATION = 0.003f;	// 回るスピード
const float HEIGHT_CYLINDER = 800.0f;	// シリンダーの高さ
const std::string PATH_RANKING = "data\\TEXT\\ranking.txt";	// ランキングのパス
const D3DXVECTOR2 SIZE_CAPTION = { SCREEN_WIDTH * 0.5f,100.0f };	// 見出しのサイズ
const D3DXVECTOR3 POS_CAPTION = { SCRN_MID.x,SIZE_CAPTION.y,0.0f };	// 見出しの位置
}

//=====================================================
// コンストラクタ
//=====================================================
CResult::CResult()
{
	m_pCylinder = nullptr;
	m_pBehavior = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CResult::~CResult()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CResult::Init(void)
{
	if (m_pCylinder == nullptr)
	{
		// シリンダーの生成
		m_pCylinder = CMeshCylinder::Create();

		if (m_pCylinder != nullptr)
		{
			m_pCylinder->SetRadius(1500.0f);
			m_pCylinder->SetHeight(HEIGHT_CYLINDER);
			m_pCylinder->SetVtx();

			int nIdx = Texture::GetIdx("data\\TEXTURE\\BG\\result.jpg");
			m_pCylinder->SetIdxTexture(nIdx);
		}
	}

	// 見出しの生成
	CObject2D *pCaption = nullptr;

	pCaption = CObject2D::Create(4);

	if (pCaption != nullptr)
	{
		pCaption->SetPosition(POS_CAPTION);
		pCaption->SetSize(SIZE_CAPTION.x, SIZE_CAPTION.y);
		pCaption->SetVtx();

		int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\result.png");
		pCaption->SetIdxTexture(nIdx);
	}

	// カメラ位置の設定
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posV = { 0.0f,HEIGHT_CYLINDER * 0.5f,0.0f };
	pInfoCamera->posR = { 0.0f,HEIGHT_CYLINDER * 0.5f,1.0f };

	// データの読込
	CSaveDataManager *pSave = CSaveDataManager::GetInstance();

	if (pSave != nullptr)
	{
		pSave->Load();
	}

	LoadRanking();

	ChangeBehavior(new CResultPlayerScore);

	return S_OK;
}

//=====================================================
// ランキングの読込
//=====================================================
void CResult::LoadRanking(void)
{
	std::ifstream file(PATH_RANKING);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// 読み込むものがなくなるまで読込
			if (temp == "PLAYER")
			{// プレイヤー情報読込
				SInfoRanking info;

				while (std::getline(file, temp))
				{
					std::istringstream iss(temp);
					std::string key;
					iss >> key;

					if (key == "RANK")
					{// ランク
						iss >> info.nRank;
					}

					if (key == "NAME")
					{// 名前
						iss >> info.name;
					}

					if (key == "REWARD")
					{// 報酬額
						iss >> info.nReward;
					}

					if (temp == "END_PLAYER")
					{
						m_listRanking.push_back(info);

						break;
					}
				}
			}
		}
	}
	else
	{
		assert(("ランキングファイルが開けませんでした", false));
	}
}

//=====================================================
// 終了処理
//=====================================================
void CResult::Uninit(void)
{
	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Uninit();
		m_pCylinder = nullptr;
	}

	if (m_pBehavior != nullptr)
	{
		delete m_pBehavior;
		m_pBehavior = nullptr;
	}

	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CResult::Update(void)
{
	if (m_pCylinder != nullptr)
	{
		D3DXVECTOR3 rot = m_pCylinder->GetRotation();

		rot.y += SPEED_ROTATION;

		universal::LimitRot(&rot.y);

		m_pCylinder->SetRotation(rot);
	}

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Update(this);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CResult::Draw(void)
{

}

//=====================================================
// ビヘイビアの変更
//=====================================================
void CResult::ChangeBehavior(CResultBehavior *pBehavior)
{
	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Uninit(this);

		delete m_pBehavior;
	}

	m_pBehavior = pBehavior;

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Init(this);
	}
}