//*****************************************************
//
// ブロックの処理[block.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "block.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "collision.h"
#include "debrisSpawner.h"
#include <stdio.h>

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
	const float SPEED_MOVE(1.0f);	// 移動速度
	const char* MAP_FILE = "data\\MAP\\map01.bin";	// マップのファイルパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CBlock *CBlock::m_apBlock[NUM_OBJECT] = {};	// ブロックの配列
int CBlock::m_nNumAll = 0;	// 総数
int *CBlock::m_pIdxObject = nullptr;	// 番号のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CBlock::CBlock(int nPriority)
{
	m_type = TYPE_DESK;
	m_pCollisionCube = nullptr;
	m_fLife = 0.0f;
	m_nID = -1;

	for (int nCntBlock = 0;nCntBlock < NUM_OBJECT;nCntBlock++)
	{
		if (m_apBlock[nCntBlock] == nullptr)
		{// 保存用配列にコピー
			m_apBlock[nCntBlock] = this;

			m_nID = nCntBlock;

			break;
		}
	}

	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CBlock::~CBlock()
{
	m_nNumAll--;
}

//=====================================================
// 生成処理
//=====================================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CBlock *pBlock = nullptr;

	if (type <= CBlock::TYPE_NONE && type >= CBlock::TYPE_MAX)
	{// 範囲外の種類が渡されたらnullptrを返す
		return nullptr;
	}

	if (pBlock == nullptr)
	{// インスタンス生成
		pBlock = new CBlock;

		pBlock->SetPosition(pos);
		pBlock->SetRot(rot);

		// 初期化処理
		pBlock->m_type = type;
		pBlock->Init();

		// 種類ごとのモデル読込
		pBlock->SetIdxModel(m_pIdxObject[type]);
		pBlock->BindModel(m_pIdxObject[type]);

		if (rot.y != 0)
		{// 角度によって最大頂点、最小頂点を変える処理
			pBlock->SwapVtx();
		}

		if (pBlock->m_pCollisionCube == nullptr)
		{// 当たり判定生成
			pBlock->m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_BLOCK, pBlock);

			if (pBlock->m_pCollisionCube != nullptr)
			{
				pBlock->m_pCollisionCube->SetPosition(pBlock->GetPosition());

				pBlock->m_pCollisionCube->SetVtx(pBlock->GetVtxMax(), pBlock->GetVtxMin());
			}
		}
	}

	return pBlock;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBlock::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 継承クラスの初期化
	CObjectX::Init();

	// タイプの設定
	SetType(TYPE_BLOCK);

	m_fLife = 300.0f;

	// 影の有効化
	EnableShadow(true);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBlock::Uninit(void)
{
	if (m_pCollisionCube != nullptr)
	{// 当たり判定の消去
		m_pCollisionCube->Uninit();

		m_pCollisionCube = nullptr;
	}

	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CBlock::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CBlock::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//=====================================================
// ヒット処理
//=====================================================
void CBlock::Hit(float fDamage)
{
	if (m_type == TYPE_IRONWALL)
	{
		return;
	}

	m_fLife -= fDamage;

	if (m_fLife <= 0.0f)
	{// 破壊判定
		Delete(m_nID);

		float fRotY = (rand() % 628 - 314) / 100.0f;
		
		// 瓦礫の生成
		CObjectX* pObjX = CObjectX::Create(GetPosition(), D3DXVECTOR3(GetRot().x, fRotY, GetRot().z));
		if (pObjX != nullptr)
		{
			int nRand = rand() % 2;
			
			if (nRand == 0)
			{
				pObjX->BindModel(CModel::Load("data\\MODEL\\block\\woodRubble00.x"));
			}
			else
			{
				pObjX->BindModel(CModel::Load("data\\MODEL\\block\\woodRubble01.x"));
			}
		}

		// 破片の生成
		CDebrisSpawner::Create(D3DXVECTOR3(GetPosition().x, GetPosition().y + 50.0f, GetPosition().z), CDebrisSpawner::TYPE::TYPE_WALL, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

//=====================================================
// 頂点を入れ替える処理
//=====================================================
void CBlock::SwapVtx(void)
{
	D3DXVECTOR3 vtxMax = GetVtxMax();
	D3DXVECTOR3 vtxMin = GetVtxMin();
	D3DXVECTOR3 vtxTemp = vtxMin;
	
	vtxMin = { -vtxMax.z,vtxMin.y,-vtxMax.x };
	vtxMax = { -vtxTemp.z,vtxMax.y,-vtxTemp.x };

	SetVtxMax(vtxMax);
	SetVtxMin(vtxMin);
}

//=====================================================
// 部分削除
//=====================================================
void CBlock::Delete(int nIdx)
{
	CBlock *pBlockDelete = m_apBlock[nIdx];

	// 配列を詰める
	/*for (int nCntBlock = nIdx; nCntBlock < NUM_OBJECT - 1; nCntBlock++)
	{
		if (m_apBlock[nCntBlock + 1] != nullptr)
		{
			m_apBlock[nCntBlock] = m_apBlock[nCntBlock + 1];

			m_apBlock[nCntBlock]->m_nID = nCntBlock;

			m_apBlock[nCntBlock + 1] = nullptr;
		}
	}*/

	if (pBlockDelete != nullptr)
	{// 削除処理
		pBlockDelete->Uninit();

		pBlockDelete = nullptr;
	}

	m_apBlock[nIdx] = nullptr;
}

//=====================================================
// 全削除処理
//=====================================================
void CBlock::DeleteAll(void)
{
	for (int nCntBlock = 0;nCntBlock < NUM_OBJECT;nCntBlock++)
	{
		if (m_apBlock[nCntBlock] != nullptr)
		{
			m_apBlock[nCntBlock] = nullptr;
		}
	}
}

//=====================================================
// 番号削除処理
//=====================================================
void CBlock::DeleteIdx(void)
{
	if (m_pIdxObject != nullptr)
	{// 番号ポインタの破棄
		delete m_pIdxObject;
		m_pIdxObject = nullptr;
	}
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CBlock::Load(char *pPath)
{
	int nNumBlock;
	MemBlock memBlock;

	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(pPath, "rb");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		
		// 数の読込
		fread(&nNumBlock, sizeof(int), 1, pFile);

		for (int nCntBlock = 0;nCntBlock < nNumBlock;nCntBlock++)
		{
			// ブロックの読込
			fread(&memBlock, sizeof(MemBlock), 1, pFile);

			// ブロックの生成
			Create(memBlock.pos, memBlock.rot,memBlock.type);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		assert(("ブロック配置データ読み込みに失敗", false));
	}

	return S_OK;
}

//=====================================================
// 影のチェック処理
//=====================================================
float CBlock::CheckShadow(D3DXVECTOR3 pos)
{
	float fHeight = -1000;

	for (int i = 0; i < NUM_OBJECT; i++)
	{
		if (m_apBlock[i] != nullptr)
		{
			// ブロックの情報取得
			D3DXVECTOR3 posBlock = m_apBlock[i]->GetPosition();
			D3DXVECTOR3 vtxMax = m_apBlock[i]->GetVtxMax() + posBlock;
			D3DXVECTOR3 vtxMin = m_apBlock[i]->GetVtxMin() + posBlock;

			if (pos.x >= vtxMin.x && pos.x <= vtxMax.x &&
				vtxMax.z >= pos.z && vtxMin.z <= pos.z)
			{// 横以内にいるとき
				if (pos.y >= posBlock.y)
				{
					float fDiff = vtxMax.y - pos.y;
					float fDiffMax = fHeight - pos.y;

					if (fDiff * fDiff < fDiffMax * fDiffMax)
					{// 最小の差分より小さかったら
						fHeight = vtxMax.y;
					}
				}
			}
		}
	}

	return fHeight;
}

//=====================================================
// モデル番号読込処理
//=====================================================
void CBlock::LoadModel(void)
{
	char *pPath[CBlock::TYPE_MAX] =
	{
		"data\\MODEL\\BLOCK\\desk00.x",			// 机
		"data\\MODEL\\BLOCK\\concrete_00.x",	// コンクリート壁
		"data\\MODEL\\BLOCK\\WodenWall_00.x",	// 木製壁
		"data\\MODEL\\GIMMICK\\door000.x",	// ドア
		"data\\MODEL\\BLOCK\\potato_bed00.x",			// 土
		"data\\MODEL\\BLOCK\\shelf.x",			// 棚
		"data\\MODEL\\BLOCK\\concrete_01.x",	// 高いコンクリート壁
		"data\\MODEL\\BLOCK\\unbreakable_wall.x",	// 鉄の壁
	};

	if (m_pIdxObject == nullptr)
	{
		// 番号の生成
		m_pIdxObject = new int[CBlock::TYPE_MAX];

		// 値の初期化
		ZeroMemory(m_pIdxObject,sizeof(int) * CBlock::TYPE_MAX);

		for (int nCntBlock = 0; nCntBlock < CBlock::TYPE_MAX; nCntBlock++)
		{
			m_pIdxObject[nCntBlock] = CModel::Load(pPath[nCntBlock]);
		}
	}
}

//=====================================================
// 保存処理
//=====================================================
void CBlock::Save(void)
{
	//ポインタ宣言
	FILE *pFile;

	// 情報受け取り用構造体
	MemBlock memBlock;

	//ファイルを開く
	pFile = fopen(MAP_FILE, "wb");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		fwrite(&m_nNumAll, sizeof(int), 1, pFile);

		for (int nCntBlock = 0; nCntBlock < m_nNumAll; nCntBlock++)
		{
			if (m_apBlock[nCntBlock] != nullptr)
			{
				memBlock.pos = m_apBlock[nCntBlock]->GetPosition();
				memBlock.rot = m_apBlock[nCntBlock]->GetRot();
				memBlock.type = m_apBlock[nCntBlock]->m_type;

				//バイナリファイルに書き込む
				fwrite(&memBlock, sizeof(MemBlock), 1, pFile);
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		assert(("ブロックデータの保存失敗", false));
	}
}