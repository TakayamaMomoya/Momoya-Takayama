//*****************************************************
//
// プレイヤーマネージャー[playerManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "playerManager.h"
#include "player.h"
#include <stdio.h>

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPlayerManager *CPlayerManager::m_pPlayerManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CPlayerManager::CPlayerManager()
{
	m_nNumPlayer = 0;
	m_nNumAttack = 0;
	m_pInfoAttack = nullptr;
	ZeroMemory(&m_apPlayer[0], sizeof(m_apPlayer));
	ZeroMemory(&m_anIDJoypad[0], sizeof(m_anIDJoypad));
	ZeroMemory(&m_abDeathPlayer[0], sizeof(m_abDeathPlayer));
}

//=====================================================
// デストラクタ
//=====================================================
CPlayerManager::~CPlayerManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CPlayerManager *CPlayerManager::Create(void)
{
	if (m_pPlayerManager == nullptr)
	{// インスタンス生成
		m_pPlayerManager = new CPlayerManager;

		// 初期化処理
		m_pPlayerManager->Init();
	}

	return m_pPlayerManager;
}

//=====================================================
// プレイヤー生成処理
//=====================================================
void CPlayerManager::CreatePlayer(void)
{
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		if(m_anIDJoypad[i] != -1)
		{
			CPlayer* pPlayer = nullptr;

			pPlayer = CPlayer::Create(m_nNumPlayer);

			if (pPlayer != nullptr)
			{
				pPlayer->SetIDJoypad(m_anIDJoypad[i]);

				pPlayer->SetPosition(D3DXVECTOR3(-75.0f + 50.0f * m_nNumPlayer, 0.0f, -100.0f));

				m_apPlayer[m_nNumPlayer] = pPlayer;

				m_nNumPlayer++;
			}
		}
	}
}

//=====================================================
// プレイヤー登録処理
//=====================================================
CPlayer *CPlayerManager::BindPlayer(int nIdxJoypad)
{
	if (nIdxJoypad < 0 || nIdxJoypad >= NUM_PLAYER)
	{
		return nullptr;
	}

	CPlayer* pPlayer = nullptr;

	pPlayer = CPlayer::Create(m_nNumPlayer);

	if (pPlayer != nullptr)
	{
		pPlayer->SetIDJoypad(nIdxJoypad);

		m_apPlayer[m_nNumPlayer] = pPlayer;

		m_anIDJoypad[m_nNumPlayer] = nIdxJoypad;

		pPlayer->SetPosition(D3DXVECTOR3(50.0f * m_nNumPlayer, 0.0f, 0.0f));

		m_nNumPlayer++;
	}

	return pPlayer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayerManager::Init(void)
{
	// プレイヤーの番号の初期設定
	for (int i = 0; i < NUM_PLAYER; i++)
	{
		m_anIDJoypad[i] = -1;
	}

	// プレイヤーパラメーターの読み込み
	LoadParamPlayer();

	return S_OK;
}

//=====================================================
// プレイヤーパラメーター読み込み
//=====================================================
void CPlayerManager::LoadParamPlayer(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntAttack = 0;

	// ファイルから読み込む
	FILE *pFile = fopen("data\\TEXT\\playerParam.txt", "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "LIFE") == 0)
			{// 初期体力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_paramPlayer.fInitialLife);
			}

			if (strcmp(cTemp, "SPEED_MOVE") == 0)
			{// 移動速度
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_paramPlayer.fSpeedMove);
			}

			if (strcmp(cTemp, "SPEED_ROT") == 0)
			{// 回転速度
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_paramPlayer.fSpeedRot);
			}

			if (strcmp(cTemp, "TIME_DAMAGE") == 0)
			{// 無敵時間
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_paramPlayer.fTimeDamage);
			}

			if (strcmp(cTemp, "POW_BLOW") == 0)
			{// 吹き飛ばす力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_paramPlayer.fPowBlow);
			}

			if (strcmp(cTemp, "DAMAGE_PUNCH") == 0)
			{// パンチの威力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_paramPlayer.fDamagePunch);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 終了条件
				break;
			}

			if (strcmp(cTemp, "NUM_ATTACK") == 0)
			{// 攻撃判定読込開始
				// 攻撃判定数読込
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_nNumAttack);

				if (m_pInfoAttack == nullptr)
				{// 判定情報の生成
					m_pInfoAttack = new CPlayer::AttackInfo[m_nNumAttack];

					for (int i = 0; i < m_nNumAttack; i++)
					{// 情報のクリア
						ZeroMemory(&m_pInfoAttack[i], sizeof(CPlayer::AttackInfo));
					}
				}
				else
				{
					break;
				}

				if (m_pInfoAttack == nullptr)
				{// 生成できなかった場合
					break;
				}

				while (true)
				{
					// 文字読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "ATTACKSET") == 0)
					{// パラメーター読込開始
						while (true)
						{
							// 文字読み込み
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "MOTION") == 0)
							{// モーションの種類
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_pInfoAttack[nCntAttack].nIdxMotion);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									fscanf(pFile, "%f", &m_pInfoAttack[nCntAttack].pos[nCntPos]);
								}
							}

							if (strcmp(cTemp, "KEY") == 0)
							{// キーの番号
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_pInfoAttack[nCntAttack].nKey);
							}

							if (strcmp(cTemp, "FRAME") == 0)
							{// フレーム番号
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_pInfoAttack[nCntAttack].nFrame);
							}

							if (strcmp(cTemp, "RADIUS") == 0)
							{// 半径
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%f", &m_pInfoAttack[nCntAttack].fRadius);
							}

							if (strcmp(cTemp, "END_ATTACKSET") == 0)
							{// パラメーター読込終了
								nCntAttack++;

								break;
							}
						}
					}

					if (m_nNumAttack <= nCntAttack)
					{
						break;
					}
				}
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		assert(("プレイヤーパラメーター読み込みに失敗", false));
	}
}

//=====================================================
// 終了処理
//=====================================================
void CPlayerManager::Uninit(void)
{
	m_pPlayerManager = nullptr;

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		if (m_apPlayer[i] != nullptr)
		{
			m_apPlayer[i] = nullptr;
		}
	}

	delete this;
}

//=====================================================
// 個別終了処理
//=====================================================
void CPlayerManager::ReleasePlayer(int nIdx)
{
	m_apPlayer[nIdx] = nullptr;

	m_nNumPlayer--;
}