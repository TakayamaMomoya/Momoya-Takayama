//*****************************************************
//
// ギミックマネージャー[gimmickManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "GimmickManager.h"
#include "Gimmick.h"
#include "door.h"
#include "container.h"
#include "itemRepair.h"
#include "box.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const char* MAP_PATH = "data\\MAP\\gimmick1.txt";	// 配置情報のパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CGimmickManager *CGimmickManager::m_pGimmickManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CGimmickManager::CGimmickManager()
{
	m_pHead = nullptr;
	m_pTail = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CGimmickManager::~CGimmickManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CGimmickManager *CGimmickManager::Create(void)
{
	if (m_pGimmickManager == nullptr)
	{// インスタンス生成
		m_pGimmickManager = new CGimmickManager;

		// 初期化処理
		m_pGimmickManager->Init();
	}

	return m_pGimmickManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGimmickManager::Init(void)
{
	// 配置データの読込
	Load();

	return S_OK;
}

//=====================================================
// 読込処理
//=====================================================
void CGimmickManager::Load(void)
{
	// 変数宣言
	char cTemp[256];

	// ファイルから読み込む
	FILE* pFile = fopen(MAP_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// ギミックの生成
			CreateGimmick(pFile, &cTemp[0]);

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 読み込みの終了
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		assert(("ギミック配置情報読み込みに失敗", false));
	}
}

//=====================================================
// データごとに生成する処理
//=====================================================
void CGimmickManager::CreateGimmick(FILE *pFile, char *pTemp)
{
	if (strcmp(pTemp, "DOORSET") == 0)
	{// ドアの設置
		LoadDoor(pFile, pTemp);
	}

	if (strcmp(pTemp, "CONTAINERSET") == 0)
	{// コンテナの設置
		LoadContainer(pFile, pTemp);
	}

	if (strcmp(pTemp, "REPAIRSET") == 0)
	{// 部品の設置
		LoadRepair(pFile, pTemp);
	}

	if (strcmp(pTemp, "BOXSET") == 0)
	{// 木箱の設置
		LoadBox(pFile, pTemp);
	}
}

//=====================================================
// ドア読込処理
//=====================================================
void CGimmickManager::LoadDoor(FILE *pFile, char *pTemp)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	float rotDest = 0.0f;

	while (true)
	{
		(void)fscanf(pFile, "%s", pTemp);

		if (strcmp(pTemp, "POS") == 0)
		{// 位置
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				(void)fscanf(pFile, "%f", &pos[i]);
			}
		}

		if (strcmp(pTemp, "ROT") == 0)
		{// 向き
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				float fAngle;

				(void)fscanf(pFile, "%f", &fAngle);

				rot[i] = D3DXToRadian(fAngle);
			}
		}

		if (strcmp(pTemp, "ROTDEST") == 0)
		{// 目標の向き
			(void)fscanf(pFile, "%s", pTemp);

			float fAngle;

			(void)fscanf(pFile, "%f", &fAngle);

			rotDest = D3DXToRadian(fAngle);
		}

		if (strcmp(pTemp, "END_DOORSET") == 0)
		{// ドア設定終了
			CDoor *pDoor = CDoor::Create();

			if (pDoor != nullptr)
			{
				pDoor->SetPosition(pos);
				pDoor->SetRot(rot);
				pDoor->SetOrgRot(rot.y);
				pDoor->SetDestRot(rotDest);
			}

			break;
		}
	}
}

//=====================================================
// コンテナ読込処理
//=====================================================
void CGimmickManager::LoadContainer(FILE *pFile, char *pTemp)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	while (true)
	{
		(void)fscanf(pFile, "%s", pTemp);

		if (strcmp(pTemp, "POS") == 0)
		{// 位置
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				(void)fscanf(pFile, "%f", &pos[i]);
			}
		}

		if (strcmp(pTemp, "ROT") == 0)
		{// 向き
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				float fAngle;

				(void)fscanf(pFile, "%f", &fAngle);

				rot[i] = D3DXToRadian(fAngle);
			}
		}

		if (strcmp(pTemp, "END_CONTAINERSET") == 0)
		{// コンテナ設定終了
			CContainer *pContainer = CContainer::Create();

			if (pContainer != nullptr)
			{
				pContainer->SetPosition(pos);
				pContainer->SetRot(rot);
			}

			break;
		}
	}
}

//=====================================================
// 部品読込処理
//=====================================================
void CGimmickManager::LoadRepair(FILE *pFile, char *pTemp)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	while (true)
	{
		(void)fscanf(pFile, "%s", pTemp);

		if (strcmp(pTemp, "POS") == 0)
		{// 位置
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				(void)fscanf(pFile, "%f", &pos[i]);
			}
		}

		if (strcmp(pTemp, "ROT") == 0)
		{// 向き
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				float fAngle;

				(void)fscanf(pFile, "%f", &fAngle);

				rot[i] = D3DXToRadian(fAngle);
			}
		}

		if (strcmp(pTemp, "END_REPAIRSET") == 0)
		{// 部品設定終了
			CItemRepair *pRapair = CItemRepair::Create();

			if (pRapair != nullptr)
			{
				pRapair->SetPosition(pos);
				pRapair->SetRot(rot);
			}

			break;
		}
	}
}

//=====================================================
// 木箱読込処理
//=====================================================
void CGimmickManager::LoadBox(FILE *pFile, char *pTemp)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	int nType = CBox::TYPE::TYPE_RANDOM;

	while (true)
	{
		(void)fscanf(pFile, "%s", pTemp);

		if (strcmp(pTemp, "POS") == 0)
		{// 位置
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				(void)fscanf(pFile, "%f", &pos[i]);
			}
		}

		if (strcmp(pTemp, "ROT") == 0)
		{// 向き
			(void)fscanf(pFile, "%s", pTemp);

			for (int i = 0; i < 3; i++)
			{
				float fAngle;

				(void)fscanf(pFile, "%f", &fAngle);

				rot[i] = D3DXToRadian(fAngle);
			}
		}

		if (strcmp(pTemp, "TYPE") == 0)
		{// 種類
			(void)fscanf(pFile, "%s", pTemp);

			(void)fscanf(pFile, "%d", &nType);
		}

		if (strcmp(pTemp, "END_BOXSET") == 0)
		{// 設定終了
			CBox *pBox = CBox::Create((CBox::TYPE)nType);

			if (pBox != nullptr)
			{
				pBox->SetPosition(pos);
				pBox->SetRot(rot);
			}

			break;
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CGimmickManager::Uninit(void)
{
	m_pGimmickManager = nullptr;

	Release();
}

//=====================================================
// 全ギミック削除処理
//=====================================================
void CGimmickManager::DeleteAllGimmick(void)
{
	CGimmick *pGimmick = GetHead();

	while (pGimmick != nullptr)
	{
		CGimmick *pGimmickNext = pGimmick->GetNext();

		if (pGimmick != nullptr)
		{
			pGimmick->Uninit();
		}

		pGimmick = pGimmickNext;
	}
}

//=====================================================
// 更新処理
//=====================================================
void CGimmickManager::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CGimmickManager::Draw(void)
{

}