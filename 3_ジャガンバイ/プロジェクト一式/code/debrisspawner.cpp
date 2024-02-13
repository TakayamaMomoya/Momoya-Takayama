//*****************************************************
//
// 飛び散る破片処理[debrisSpawner.cpp]
// Author:小笠原彪
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "debris.h"
#include "debrisSpawner.h"
#include "collision.h"
#include <stdio.h>

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CDebrisSpawner::PARTICLE_INFO* CDebrisSpawner::m_apDebris[CDebrisSpawner::TYPE_MAX + 1] = {};

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CDebrisSpawner::CDebrisSpawner(int nPriority) : CObject(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_pPosOwner = nullptr;
	m_rot = { 0.0f,0.0f,0.0f };
	m_type = TYPE_NONE;
	m_nPriorityDebris = 0;
	m_nLife = 0;
	m_ModelIdx = 0;

	SetType(TYPE_PARTICLE);
}

//=====================================================
//	デストラクタ
//=====================================================
CDebrisSpawner::~CDebrisSpawner()
{

}

//=====================================================
//	初期化処理
//=====================================================
HRESULT CDebrisSpawner::Init(void)
{
	Load();

	return S_OK;
}

//=====================================================
//	終了処理
//=====================================================
void CDebrisSpawner::Uninit(void)
{
	Release();
}

//=====================================================
//	破棄処理
//=====================================================
void CDebrisSpawner::Unload(void)
{
	for (int nCntDebris = 0; nCntDebris < TYPE_MAX; nCntDebris++)
	{
		if (m_apDebris[nCntDebris] != nullptr)
		{
			delete m_apDebris[nCntDebris];
			m_apDebris[nCntDebris] = nullptr;
		}
	}
}

//=====================================================
//	壁の破片
//=====================================================
void CDebrisSpawner::DebrisWall(void)
{
}

//=====================================================
//	更新処理
//=====================================================
void CDebrisSpawner::Update(void)
{
	// 変数宣言
	float fRot, fRot2;
	float fMove = 0.0f;
	float fRadius = 0.0f;
	int nLife = 0;
	int nModelIdx = 0;
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	CObjectX* pObjectX = nullptr;

	for (int nCntEffect = 0; nCntEffect < m_apDebris[m_type]->nNumDebris; nCntEffect++)
	{
		if (m_apDebris[m_type]->nRot)
		{// 向きの反映
			if (m_apDebris[m_type]->fRangeRot > 0.1f)
			{
				fRot = m_rot.x + (float)(rand() % (int)(m_apDebris[m_type]->fRangeRot * 100.0f) - m_apDebris[m_type]->fRangeRot * 50.0f) / 100.0f;
				fRot2 = m_rot.y + (float)(rand() % (int)(m_apDebris[m_type]->fRangeRot * 100.0f) - m_apDebris[m_type]->fRangeRot * 50.0f) / 100.0f * 2;
			}
			else
			{
				fRot = m_rot.x + (rand() % (int)(0.1f * 100.0f) - 0.1f * 50.0f) / 100.0f;
				fRot2 = m_rot.y + (rand() % (int)(0.1f * 100.0f) - 0.1f * 50.0f) / 100.0f * 2;
			}
		}
		else
		{// 全方位
			fRot = rand() % 628 - 314 / 100.0f;
			fRot2 = rand() % 628 - 314 / 100.0f;
		}

		if (m_apDebris[m_type]->fSpeed > 0.1f)
		{
			fMove = (float)(rand() % (int)(m_apDebris[m_type]->fSpeed * 10)) * 0.1f + m_apDebris[m_type]->fSpeed * 0.5f;
		}
		if (m_apDebris[m_type]->nLifeDebris != 0)
		{
			nLife = rand() % m_apDebris[m_type]->nLifeDebris + m_apDebris[m_type]->nLifeDebris / 2;
		}

		move.x = sinf(fRot) * sinf(fRot2) * fMove;
		move.y = cosf(fRot) * fMove;
		move.z = sinf(fRot) * cosf(fRot2) * fMove;

		rot.x = (float)(rand() % 629 - 314) / 100.0f;
		rot.y = (float)(rand() % 629 - 314) / 100.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// エフェクト生成
		pObjectX = CDebris::Create(m_pos, nLife, m_apDebris[m_type]->nModelIdx , move, m_apDebris[m_type]->fGravity, m_apDebris[m_type]->bBounce);
		pObjectX->SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));
	}

	m_nLife--;

	if (m_nLife < 0)
	{
		// 自身の破棄
		Uninit();
	}
}

//=====================================================
//	位置設定処理
//=====================================================
void CDebrisSpawner::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
//	生成処理
//=====================================================
CDebrisSpawner* CDebrisSpawner::Create(D3DXVECTOR3 pos, TYPE type, D3DXVECTOR3 rot, int nPriority)
{
	CDebrisSpawner* pParticle = nullptr;

	if (pParticle == nullptr)
	{// インスタンス生成
		pParticle = new CDebrisSpawner;

		pParticle->Init();

		pParticle->m_type = type;

		pParticle->m_pos = pos;

		pParticle->m_rot = rot;

		pParticle->m_nLife = m_apDebris[type]->nLife;

		pParticle->m_nPriorityDebris = nPriority;
	}

	return pParticle;
}

//=====================================================
//	読込処理
//=====================================================
void CDebrisSpawner::Load(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntDebris = 1;
	PARTICLE_INFO* pInfo = nullptr;

	// ファイルから読み込む
	FILE* pFile = fopen("data\\TEXT\\debris.txt", "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "DEBRISSET") == 0)
			{// 読込開始
				if (m_apDebris[nCntDebris] == nullptr)
				{
					// インスタンス生成
					m_apDebris[nCntDebris] = new PARTICLE_INFO;
				}

				pInfo = m_apDebris[nCntDebris];

				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "END_DEBRISSET") == 0)
					{// パーティクル情報終了条件
						break;
					}

					if (strcmp(cTemp, "MODEL_TYPE") == 0)
					{// パーティクル寿命取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nModelIdx);
					}

					if (strcmp(cTemp, "LIFE_DEBRISSPAWNER") == 0)
					{// パーティクル寿命取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nLife);
					}

					if (strcmp(cTemp, "NUM_DEBRIS") == 0)
					{// エフェクト数取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nNumDebris);
					}

					if (strcmp(cTemp, "LIFE_DEBRIS") == 0)
					{// エフェクト寿命取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nLifeDebris);
					}

					if (strcmp(cTemp, "SPEED_DEBRIS") == 0)
					{// エフェクト速度取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &pInfo->fSpeed);
					}

					if (strcmp(cTemp, "GRAVITY") == 0)
					{// 重力取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &pInfo->fGravity);
					}

					if (strcmp(cTemp, "RANGEROT") == 0)
					{// 向きのランダム幅取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &pInfo->fRangeRot);
					}

					if (strcmp(cTemp, "IS_ROT") == 0)
					{// 加算合成かどうか取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nRot);
					}

					if (strcmp(cTemp, "IS_TURN") == 0)
					{// 加算合成かどうか取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "1") == 0)
						{
							pInfo->bTurn = true;
						}
						else
						{
							pInfo->bTurn = false;
						}
					}
					if (strcmp(cTemp, "IS_BAUNCE") == 0)
					{// 加算合成かどうか取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "1") == 0)
						{
							pInfo->bBounce = true;
						}
						else
						{
							pInfo->bBounce = false;
						}
					}
				}

				// パーティクル数加算
				nCntDebris++;
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 終了条件
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		assert(("パーティクル情報読み込みに失敗", false));
	}
}