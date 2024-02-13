//*****************************************************
//
// 武器マネージャー[weaponManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "weaponManager.h"
#include "Weapon.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const char* PARAM_PATH = "data\\TEXT\\weapon.txt";	// パラメーターのパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CWeaponManager *CWeaponManager::m_pWeaponManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CWeaponManager::CWeaponManager()
{
	ZeroMemory(&m_aInfo[0], sizeof(m_aInfo));
	ZeroMemory(&m_infoShotgun, sizeof(SInfoShotgun));
	ZeroMemory(&m_infoRailgun, sizeof(SInfoRailgun));
	ZeroMemory(&m_anProb[0], sizeof(m_anProb));
}

//=====================================================
// デストラクタ
//=====================================================
CWeaponManager::~CWeaponManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CWeaponManager *CWeaponManager::Create(void)
{
	if (m_pWeaponManager == nullptr)
	{// インスタンス生成
		m_pWeaponManager = new CWeaponManager;

		// 初期化処理
		m_pWeaponManager->Init();
	}

	return m_pWeaponManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CWeaponManager::Init(void)
{
	// パラメーター読み込み
	LoadParam();

	return S_OK;
}

//=====================================================
// パラメーター読み込み処理
//=====================================================
void CWeaponManager::LoadParam(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntParam = 0;

	// ファイルから読み込む
	FILE* pFile = fopen(PARAM_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "MAGNUMSET") == 0)
			{// パラメーター読込開始
				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// 基底パラメーター読み込み
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_MAGNUM);

					if (strcmp(cTemp, "END_MAGNUMSET") == 0)
					{// パラメーター読込終了
						break;
					}
				}
			}

			if (strcmp(cTemp, "MACHINEGUNSET") == 0)
			{// パラメーター読込開始
				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// 基底パラメーター読み込み
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_MACHINEGUN);

					if (strcmp(cTemp, "END_MACHINEGUNSET") == 0)
					{// パラメーター読込終了
						break;
					}
				}
			}

			if (strcmp(cTemp, "SHOTGUNSET") == 0)
			{// パラメーター読込開始
				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// 基底パラメーター読み込み
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_SHOTGUN);

					// ショットガン特殊パラメーター読込
					LoadShotgunInfo(pFile, &cTemp[0]);

					if (strcmp(cTemp, "END_SHOTGUNSET") == 0)
					{// パラメーター読込終了
						break;
					}
				}
			}

			if (strcmp(cTemp, "RAILGUNSET") == 0)
			{// パラメーター読込開始
				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// 基底パラメーター読み込み
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_RAILGUN);

					// 固有パラメーター読込
					LoadRailgunInfo(pFile, &cTemp[0]);

					if (strcmp(cTemp, "END_RAILGUNSET") == 0)
					{// パラメーター読込終了
						break;
					}
				}
			}

			if (strcmp(cTemp, "MINIGUNSET") == 0)
			{// パラメーター読込開始
				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// 基底パラメーター読み込み
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_MINIGUN);

					if (strcmp(cTemp, "END_MINIGUNSET") == 0)
					{// パラメーター読込終了
						break;
					}
				}
			}

			if (strcmp(cTemp, "LAUNCHERSET") == 0)
			{// パラメーター読込開始
				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// 基底パラメーター読み込み
					LoadBaseInfo(pFile, &cTemp[0], CWeapon::TYPE_LAUNCHER);

					if (strcmp(cTemp, "END_LAUNCHERSET") == 0)
					{// パラメーター読込終了
						break;
					}
				}
			}

			if (strcmp(cTemp, "PROBSET") == 0)
			{// 確率読込開始
				int nCntProb = 0;

				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "PROB") == 0)
					{
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_anProb[nCntProb]);

						nCntProb++;
					}

					if (strcmp(cTemp, "END_PROBSET") == 0 ||
						nCntProb >= CWeapon::TYPE_MAX)
					{// パラメーター読込終了
						break;
					}
				}
			}

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
		assert(("武器情報読み込みに失敗", false));
	}
}

//=====================================================
// 基底パラメーター読込
//=====================================================
void CWeaponManager::LoadBaseInfo(FILE* pFile, char* pTemp, int nCntParam)
{
	if (strcmp(pTemp, "BULLET") == 0)
	{// 弾数
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%d", &m_aInfo[nCntParam].nMaxBullet);
	}

	if (strcmp(pTemp, "RAPID") == 0)
	{// 連射力
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%d", &m_aInfo[nCntParam].nRapid);
	}

	if (strcmp(pTemp, "DAMAGE") == 0)
	{// 威力
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_aInfo[nCntParam].fDamage);
	}

	if (strcmp(pTemp, "BULLET_LIFE") == 0)
	{// 弾の寿命
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_aInfo[nCntParam].fLifeBullet);
	}

	if (strcmp(pTemp, "VIB_POWER") == 0)
	{// コントローラーの振動の強さ
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_aInfo[nCntParam].fVibPower);
	}

	if (strcmp(pTemp, "VIB_TIME") == 0)
	{// コントローラーの振動時間
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%d", &m_aInfo[nCntParam].nVibTime);
	}
}

//=====================================================
// ショットガンパラメーター読込
//=====================================================
void CWeaponManager::LoadShotgunInfo(FILE *pFile, char* pTemp)
{
	if (strcmp(pTemp, "NUM_PELLET") == 0)
	{// 発射する弾数
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%d", &m_infoShotgun.nNumPellet);
	}

	if (strcmp(pTemp, "ANGLE_DIFFUSE") == 0)
	{// 拡散角度
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_infoShotgun.fAngleDiffuse);
	}
}

//=====================================================
// レールガンパラメーター読込
//=====================================================
void CWeaponManager::LoadRailgunInfo(FILE *pFile, char* pTemp)
{
	if (strcmp(pTemp, "WIDTH") == 0)
	{// 幅
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_infoRailgun.fWidth);
	}

	if (strcmp(pTemp, "LENGTH") == 0)
	{// 長さ
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_infoRailgun.fLength);
	}
}

//=====================================================
// 終了処理
//=====================================================
void CWeaponManager::Uninit(void)
{
	m_pWeaponManager = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CWeaponManager::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CWeaponManager::Draw(void)
{

}