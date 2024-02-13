//*****************************************************
//
// 分割モーションの処理[motionDiv.cpp]
// Author:酒井南勝
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "motionDiv.h"
#include "model.h"
#include <stdio.h>
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "particle.h"
#include "sound.h"
#include "universal.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_STRING	(256)	// 文字列の最大数

//=====================================================
// コンストラクタ
//=====================================================
CMotionDiv::CMotionDiv(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_aMotionInfo, sizeof(m_aMotionInfo));
	ZeroMemory(&m_aKeyOld, sizeof(m_aKeyOld));
	ZeroMemory(&m_apParts[0], sizeof(m_apParts));
	ZeroMemory(&m_aLoopMotion[0], sizeof(m_aLoopMotion));
	ZeroMemory(&m_aMotionType[0], sizeof(m_aMotionType));
	ZeroMemory(&m_aMotionTypeOld[0], sizeof(m_aMotionTypeOld));
	ZeroMemory(&m_aCounterMotion[0], sizeof(m_aCounterMotion));
	ZeroMemory(&m_aKey[0], sizeof(m_aKey));
	ZeroMemory(&m_aFrame[0], sizeof(m_aFrame));
	ZeroMemory(&m_aNumKey[0], sizeof(m_aNumKey));
	ZeroMemory(&m_aFrame[0], sizeof(m_aFrame));
	ZeroMemory(&m_aNumMotion[0], sizeof(m_aNumMotion));
	ZeroMemory(&m_aNumParts[0], sizeof(m_aNumParts));
	ZeroMemory(&m_aFinish[0], sizeof(m_aFinish));
	m_bShadow = false;
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_posShadow = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
	m_bInde = false;
}

//=====================================================
// デストラクタ
//=====================================================
CMotionDiv::~CMotionDiv()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMotionDiv::Init(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		SetMotion(nCntDiv, 0);
		InitPose(0);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMotionDiv::Uninit(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
		{// パーティクル情報の破棄
			if (m_aMotionInfo[nCntDiv][nCntMotion].pParticle != nullptr)
			{
				delete m_aMotionInfo[nCntDiv][nCntMotion].pParticle;
				m_aMotionInfo[nCntDiv][nCntMotion].pParticle = nullptr;
			}
		}
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CMotionDiv::Update(void)
{
	// 現在の位置を保存
	m_posOld = m_pos;

	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		//変数宣言
		int nNextKey;

		// パーツのトランスフォーム
		D3DXVECTOR3 pos, rot;

		if (m_apParts[nCntDiv][0] == nullptr)
		{
			return;
		}

		int nMotionType = m_aMotionType[nCntDiv];
		
		if (m_aMotionInfo[nCntDiv][nMotionType].pParticle != nullptr)
		{
			for (int nCntParticle = 0; nCntParticle < m_aMotionInfo[nCntDiv][nMotionType].nNumParticle; nCntParticle++)
			{// 全てのパーティクルを確認
				if (m_aKey[nCntDiv] == m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].nKey &&
					m_aCounterMotion[nCntDiv] == m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].nFrame)
				{// パーティクル生成
					// 親パーツの位置取得
					D3DXMATRIX* pMtx = m_apParts[nCntDiv][m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].nIdxParent]->pParts->GetMatrix();
					pos = D3DXVECTOR3(pMtx->_41, pMtx->_42, pMtx->_43) + m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].offset;

					CParticle::Create(pos, (CParticle::TYPE)m_aMotionInfo[nCntDiv][nMotionType].pParticle[nCntParticle].nType);
				}
			}
		}

		for (int nCntParts = 0; nCntParts < m_aNumParts[nCntDiv]; nCntParts++)
		{
			if (m_aKey[nCntDiv] >= m_aMotionInfo[nCntDiv][nMotionType].nNumKey - 1)
			{
				break;
			}

			// パーツのトランスフォーム取得
			pos = m_apParts[nCntDiv][nCntParts]->pParts->GetPosOrg();
			rot = m_apParts[nCntDiv][nCntParts]->pParts->GetRot();

			if (m_aKey[nCntDiv] < m_aMotionInfo[nCntDiv][nMotionType].nNumKey - 1)
			{
				nNextKey = m_aKey[nCntDiv] + 1;
			}
			else
			{
				nNextKey = m_aMotionInfo[nCntDiv][nMotionType].nNumKey - 1;
			}

			//前回と次のキーの差分取得
			float DiffPosX = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosX -
				m_aKeyOld[nCntDiv][nCntParts].fPosX;

			float DiffPosY = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosY -
				m_aKeyOld[nCntDiv][nCntParts].fPosY;

			float DiffPosZ = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosZ -
				m_aKeyOld[nCntDiv][nCntParts].fPosZ;

			float DiffRotX = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotX -
				m_aKeyOld[nCntDiv][nCntParts].fRotX;

			float DiffRotY = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotY -
				m_aKeyOld[nCntDiv][nCntParts].fRotY;

			float DiffRotZ = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotZ -
				m_aKeyOld[nCntDiv][nCntParts].fRotZ;

			universal::LimitRot(&DiffRotX);
			universal::LimitRot(&DiffRotY);
			universal::LimitRot(&DiffRotZ);

			//目的の値=======================================================================================================
			float DestPosX = pos.x + m_aKeyOld[nCntDiv][nCntParts].fPosX +
				DiffPosX * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestPosY = pos.y + m_aKeyOld[nCntDiv][nCntParts].fPosY +
				DiffPosY * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestPosZ = pos.z + m_aKeyOld[nCntDiv][nCntParts].fPosZ +
				DiffPosZ * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestRotX = m_aKeyOld[nCntDiv][nCntParts].fRotX +
				DiffRotX * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestRotY = m_aKeyOld[nCntDiv][nCntParts].fRotY +
				DiffRotY * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			float DestRotZ = m_aKeyOld[nCntDiv][nCntParts].fRotZ +
				DiffRotZ * (float)(1.0f / (float)m_aFrame[nCntDiv]) * m_aCounterMotion[nCntDiv];

			//パーツの向き・位置設定
			m_apParts[nCntDiv][nCntParts]->pParts->SetPosition(D3DXVECTOR3(DestPosX, DestPosY, DestPosZ));

			universal::LimitRot(&DestRotX);
			universal::LimitRot(&DestRotY);
			universal::LimitRot(&DestRotZ);

			rot = D3DXVECTOR3(DestRotX, DestRotY, DestRotZ);

			m_apParts[nCntDiv][nCntParts]->pParts->SetRot(rot);
		}

		m_aCounterMotion[nCntDiv]++;

		if (m_aKey[nCntDiv] >= m_aMotionInfo[nCntDiv][nMotionType].nNumKey - 1)
		{// モーションが終了していたら
			m_aMotionTypeOld[nCntDiv] = nMotionType;

			// 終了した状態にする
			m_aFinish[nCntDiv] = true;

			if (m_aMotionInfo[nCntDiv][nMotionType].bLoop)
			{// ループするものは再設定
				SetMotion(nCntDiv,nMotionType);
			}
		}

		if (m_aCounterMotion[nCntDiv] > m_aFrame[nCntDiv])
		{//キーのフレーム数に達したら
			if (m_aKey[nCntDiv] < m_aMotionInfo[nCntDiv][nMotionType].nNumKey)
			{
				m_aKey[nCntDiv]++;

				m_aFrame[nCntDiv] = m_aMotionInfo[nCntDiv][nMotionType].aKeyInfo[m_aKey[nCntDiv]].nFrame;

				m_aCounterMotion[nCntDiv] = 0;

				SetKeyOld();
			}
		}
	}
}

//=====================================================
// モーション設定
//=====================================================
void CMotionDiv::SetMotion(int nNum, int nMotionType)
{
	// 終了状態の解除
	m_aFinish[nNum] = false;

	// 終了するモーションを保存
	m_aMotionTypeOld[nNum] = m_aMotionType[nNum];
	SetKeyOld();

	if (m_aKey[nNum] != -1)
	{
		m_aFrame[nNum] = m_aMotionInfo[nNum][m_aMotionType[nNum]].aKeyInfo[m_aKey[nNum]].nFrame;
	}

	if (m_aMotionType[nNum] != nMotionType)
	{
		m_aFrame[nNum] = 10;
	}

	// モーション情報の設定
	m_aMotionType[nNum] = nMotionType;
	m_aKey[nNum] = -1;
	m_aCounterMotion[nNum] = 0;
}

//=====================================================
// 終了するキーの情報保存
//=====================================================
void CMotionDiv::SetKeyOld(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int nCntPart = 0; nCntPart < m_aNumParts[nCntDiv]; nCntPart++)
		{
			m_aKeyOld[nCntDiv][nCntPart].fPosX = m_apParts[nCntDiv][nCntPart]->pParts->GetPosition().x - m_apParts[nCntDiv][nCntPart]->pParts->GetPosOrg().x;
			m_aKeyOld[nCntDiv][nCntPart].fPosY = m_apParts[nCntDiv][nCntPart]->pParts->GetPosition().y - m_apParts[nCntDiv][nCntPart]->pParts->GetPosOrg().y;
			m_aKeyOld[nCntDiv][nCntPart].fPosZ = m_apParts[nCntDiv][nCntPart]->pParts->GetPosition().z - m_apParts[nCntDiv][nCntPart]->pParts->GetPosOrg().z;

			m_aKeyOld[nCntDiv][nCntPart].fRotX = m_apParts[nCntDiv][nCntPart]->pParts->GetRot().x;
			m_aKeyOld[nCntDiv][nCntPart].fRotY = m_apParts[nCntDiv][nCntPart]->pParts->GetRot().y;
			m_aKeyOld[nCntDiv][nCntPart].fRotZ = m_apParts[nCntDiv][nCntPart]->pParts->GetRot().z;
		}
	}
}

//=====================================================
// ポーズの初期設定
//=====================================================
void CMotionDiv::InitPose(int nMotion)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int i = 0; i < m_aMotionInfo[nCntDiv][nMotion].nNumKey; i++)
		{
			for (int nCntPart = 0; nCntPart < m_aNumParts[nCntDiv]; nCntPart++)
			{
				m_aKeyOld[nCntDiv][nCntPart] = m_aMotionInfo[nCntDiv][nMotion].aKeyInfo[i].aKey[nCntPart];
			}
		}
	}
}

//=====================================================
// 自分のマトリックスの設定
//=====================================================
void CMotionDiv::SetMatrix(void)
{
	//変数宣言
	D3DXMATRIX mtxRotModel, mtxTransModel;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
// マトリックスをかけ合わせる処理
//=====================================================
void CMotionDiv::MultiplyMtx(void)
{
	if (m_bInde == false)
	{
		// 親のマトリックス
		SetMatrix();
	}

	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		D3DXMATRIX mtxRotModel, mtxTransModel;
		D3DXMATRIX* pMtxParent;
		D3DXMATRIX* pMtx;

		D3DXMATRIX mtxShadow;
		D3DLIGHT9 light;
		D3DXVECTOR4 posLight;
		D3DXVECTOR3 normal;
		D3DXPLANE plane;

		// ライトの位置設定
		pDevice->GetLight(2, &light);
		posLight = { -light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f };

		// 平面情報の生成
		normal = { 0.0f,1.0f,0.0f };
		D3DXPlaneFromPointNormal(&plane, &m_posShadow, &normal);

		for (int nCntParts = 0; nCntParts < m_aNumParts[nCntDiv]; nCntParts++)
		{
			// マトリックスの取得
			pMtx = m_apParts[nCntDiv][nCntParts]->pParts->GetMatrix();

			//ワールドマトリックス初期化
			D3DXMatrixIdentity(pMtx);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
				m_apParts[nCntDiv][nCntParts]->pParts->GetRot().y, m_apParts[nCntDiv][nCntParts]->pParts->GetRot().x, m_apParts[nCntDiv][nCntParts]->pParts->GetRot().z);
			D3DXMatrixMultiply(pMtx, pMtx, &mtxRotModel);

			//位置を反映
			D3DXMatrixTranslation(&mtxTransModel,
				m_apParts[nCntDiv][nCntParts]->pParts->GetPosition().x, m_apParts[nCntDiv][nCntParts]->pParts->GetPosition().y, m_apParts[nCntDiv][nCntParts]->pParts->GetPosition().z);
			D3DXMatrixMultiply(pMtx, pMtx, &mtxTransModel);

			if (m_apParts[nCntDiv][nCntParts]->nIdxParent != -1)
			{//親パーツがある場合
				// 親マトリックスの取得
				pMtxParent = m_apParts[nCntDiv][m_apParts[nCntDiv][nCntParts]->nIdxParent]->pParts->GetMatrix();
			}
			else
			{
				if (nCntDiv == 0)
				{
					pMtxParent = &m_mtxWorld;
				}
				else
				{
					pMtxParent = m_apParts[0][0]->pParts->GetMatrix();
				}
				
			}

			//親パーツとパーツのワールドマトリックスをかけ合わせる
			D3DXMatrixMultiply(pMtx, pMtx, pMtxParent);

			if (m_bShadow)
			{
				// シャドウマトリックス初期化
				D3DXMatrixIdentity(&mtxShadow);

				// シャドウマトリックスの作成
				D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
				D3DXMatrixMultiply(&mtxShadow, pMtx, &mtxShadow);

				// シャドウマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				m_apParts[nCntDiv][nCntParts]->pParts->DrawShadow();
			}

			//ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, pMtx);

			m_apParts[nCntDiv][nCntParts]->pParts->Draw();
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CMotionDiv::Draw(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		// マトリックスをかけ合わせる処理
		MultiplyMtx();

		CDebugProc::GetInstance()->Print("\nフレーム[%d]", m_aFrame[nCntDiv]);
		CDebugProc::GetInstance()->Print("\nキー[%d]", m_aKey[nCntDiv]);
	}
}

//=====================================================
// 読込処理
//=====================================================
void CMotionDiv::Load(char* pPathLower, char* pPathUpper)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		//変数宣言
		char cTemp[MAX_STRING];
		int nCntMotion = 0;
		int nCntModel = 0;

		FILE* pFile = NULL;

		if (nCntDiv == DIV_BODY_LOWER)
		{
			//ファイルから読み込む
			pFile = fopen(pPathLower, "r");
		}
		else if (nCntDiv == DIV_BODY_UPPER)
		{
			//ファイルから読み込む
			pFile = fopen(pPathUpper, "r");
		}

		if (pFile != nullptr)
		{//ファイルが開けた場合
			while (true)
			{
				//文字読み込み
				(void)fscanf(pFile, "%s", &cTemp[0]);

				//ファイル名読み込み=========================================
				if (strcmp(cTemp, "NUM_MODEL") == 0)
				{
					//"="読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					//モデル数読み込み
					(void)fscanf(pFile, "%d", &m_aNumParts[nCntDiv]);

					for (int nCntFile = 0; nCntFile < m_aNumParts[nCntDiv];)
					{//ファイル名読み込み

						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "MODEL_FILENAME") == 0)
						{//ファイル名読み込み
							(void)fscanf(pFile, "%s", &cTemp[0]);

							char aPath[MAX_STRING];

							// モデルパス読込
							(void)fscanf(pFile, "%s", &aPath[0]);

							m_apParts[nCntDiv][nCntFile] = new Parts;

							m_apParts[nCntDiv][nCntFile]->pParts = CParts::Create();

							int nIdx = CModel::Load(&aPath[0]);

							// モデル読込
							m_apParts[nCntDiv][nCntFile]->pParts->SetIdxModel(nIdx);
							m_apParts[nCntDiv][nCntFile]->pParts->BindModel(m_apParts[nCntDiv][nCntFile]->pParts->GetIdxModel());

							nCntFile++;
						}
					}
				}
				//===========================================================

				//キャラクター設定===========================================
				if (strcmp(cTemp, "CHARACTERSET") == 0)
				{//キャラクタースタート
					nCntModel = 0;

					while (strcmp(cTemp, "END_CHARACTERSET") != 0)
					{//終わりまでキャラクター設定
						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "PARTSSET") == 0)
						{//パーツスタート
							while (strcmp(cTemp, "END_PARTSSET") != 0)
							{//終わりまでパーツ設定
								(void)fscanf(pFile, "%s", &cTemp[0]);

								if (strcmp(cTemp, "INDEX") == 0)
								{//番号読み込み
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_apParts[nCntDiv][nCntModel]->nIdx);
								}

								if (strcmp(cTemp, "PARENT") == 0)
								{//親番号読み込み
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_apParts[nCntDiv][nCntModel]->nIdxParent);
								}

								if (strcmp(cTemp, "POS") == 0)
								{//位置読み込み
									D3DXVECTOR3 pos;

									(void)fscanf(pFile, "%s", &cTemp[0]);

									for (int nCntPos = 0; nCntPos < 3; nCntPos++)
									{
										(void)fscanf(pFile, "%f", &pos[nCntPos]);
									}

									m_apParts[nCntDiv][nCntModel]->pParts->SetPosition(pos);

									m_apParts[nCntDiv][nCntModel]->pParts->SetPosOrg(pos);
								}

								if (strcmp(cTemp, "ROT") == 0)
								{//向き読み込み
									D3DXVECTOR3 rot;

									(void)fscanf(pFile, "%s", &cTemp[0]);

									for (int nCntRot = 0; nCntRot < 3; nCntRot++)
									{
										(void)fscanf(pFile, "%f", &rot[nCntRot]);
									}

									m_apParts[nCntDiv][nCntModel]->pParts->SetRot(rot);
								}

							}//END_PART

							nCntModel++;
						}
					}//END_CHAR
				}
				//===========================================================

				//変数宣言
				int nCntKey = 0;
				int nCntPart = 0;
				int nCntParticle = 0;

				//モーション設定===========================================
				if (strcmp(cTemp, "MOTIONSET") == 0)
				{//モーションスタート
					while (strcmp(cTemp, "END_MOTIONSET") != 0)
					{//終わりまでキャラクター設定

						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "LOOP") == 0)
						{//ループ判断
							(void)fscanf(pFile, "%s", &cTemp[0]);

							(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntDiv][nCntMotion].bLoop);
						}

						if (strcmp(cTemp, "NUM_KEY") == 0)
						{//キー数判断
							(void)fscanf(pFile, "%s", &cTemp[0]);

							(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][nCntMotion].nNumKey);
						}

						if (strcmp(cTemp, "NUM_PARTICLE") == 0)
						{//パーティクル数判断
							(void)fscanf(pFile, "%s", &cTemp[0]);

							(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].nNumParticle);

							if (m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].nNumParticle != 0)
							{
								// パーティクル情報を生成
								m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle = new PARTICLE_INFO[m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].nNumParticle];

								// パーティクル情報初期化
								ZeroMemory(m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle, sizeof(PARTICLE_INFO) * m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].nNumParticle);
							}
						}

						if (strcmp(cTemp, "PARTICLESET") == 0 && m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle != 0)
						{// パーティクル情報設定
							while (strcmp(cTemp, "END_PARTICLESET") != 0)
							{//終わりまでパーティクル設定
								(void)fscanf(pFile, "%s", &cTemp[0]);

								if (strcmp(cTemp, "KEY") == 0)
								{// 再生キー取得
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].nKey);
								}

								if (strcmp(cTemp, "FRAME") == 0)
								{// 再生フレーム取得
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].nFrame);
								}

								if (strcmp(cTemp, "TYPE") == 0)
								{// 種類取得
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].nType);
								}

								if (strcmp(cTemp, "POS") == 0)
								{//位置読み込み
									D3DXVECTOR3 pos;

									(void)fscanf(pFile, "%s", &cTemp[0]);

									for (int nCntPos = 0; nCntPos < 3; nCntPos++)
									{
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].offset[nCntPos]);
									}
								}

								if (strcmp(cTemp, "PARENT") == 0)
								{// 親パーツ番号取得
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][m_aNumMotion[nCntDiv]].pParticle[nCntParticle].nIdxParent);
								}
							}

							nCntParticle++;
						}

						//if (strcmp(cTemp, "NUM_COLLISION") == 0)
						//{// 当たり判定数判断
						//	(void)fscanf(pFile, "%s", &cTemp[0]);

						//	(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumCollision);

						//	if (m_aMotionInfo[m_nNumMotion].nNumCollision != 0)
						//	{
						//		// 当たり判定情報を生成
						//		m_aMotionInfo[m_nNumMotion].pCollision = new COLLISION_INFO[m_aMotionInfo[m_nNumMotion].nNumCollision];

						//		// 当たり判定情報初期化
						//		ZeroMemory(m_aMotionInfo[m_nNumMotion].pCollision, sizeof(COLLISION_INFO) * m_aMotionInfo[m_nNumMotion].nNumCollision);
						//	}
						//}

						//if (strcmp(cTemp, "COLLISIONSET") == 0 && m_aMotionInfo[m_nNumMotion].pCollision != 0)
						//{// 当たり判定情報設定
						//	while (strcmp(cTemp, "END_COLLISIONSET") != 0)
						//	{//終わりまで当たり判定設定
						//		(void)fscanf(pFile, "%s", &cTemp[0]);

						//		if (strcmp(cTemp, "KEY") == 0)
						//		{// 再生キー取得
						//			(void)fscanf(pFile, "%s", &cTemp[0]);

						//			(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nKey);
						//		}

						//		if (strcmp(cTemp, "FRAME") == 0)
						//		{// 再生フレーム取得
						//			(void)fscanf(pFile, "%s", &cTemp[0]);

						//			(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nFrame);
						//		}

						//		if (strcmp(cTemp, "POS") == 0)
						//		{//位置読み込み
						//			D3DXVECTOR3 pos;

						//			(void)fscanf(pFile, "%s", &cTemp[0]);

						//			for (int nCntPos = 0; nCntPos < 3; nCntPos++)
						//			{
						//				(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].offset[nCntPos]);
						//			}
						//		}

						//		if (strcmp(cTemp, "PARENT") == 0)
						//		{// 親パーツ番号取得
						//			(void)fscanf(pFile, "%s", &cTemp[0]);

						//			(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nIdxParent);
						//		}
						//	}

						//	nCntParticle++;
						//}

						if (strcmp(cTemp, "KEYSET") == 0)
						{//キースタート
							while (strcmp(cTemp, "END_KEYSET") != 0)
							{//終わりまでキー設定

								(void)fscanf(pFile, "%s", &cTemp[0]);

								if (strcmp(cTemp, "FRAME") == 0)
								{//フレーム数取得
									(void)fscanf(pFile, "%s", &cTemp[0]);

									(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].nFrame);
								}

								if (strcmp(cTemp, "KEY") == 0)
								{//フレーム数取得
									while (strcmp(cTemp, "END_KEY") != 0)
									{//終わりまでキー設定
										(void)fscanf(pFile, "%s", &cTemp[0]);

										if (strcmp(cTemp, "POS") == 0)
										{//位置取得
											(void)fscanf(pFile, "%s", &cTemp[0]);

											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
										}

										if (strcmp(cTemp, "ROT") == 0)
										{//向き取得
											(void)fscanf(pFile, "%s", &cTemp[0]);

											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
											(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
										}
									}

									nCntPart++;

								}//KEYif
							}//ENDKEYwhile

							nCntKey++;
							nCntPart = 0;
						}
					}//MOTIONwhile

					nCntMotion++;
					m_aNumMotion[nCntDiv]++;
				}
				//===========================================================

				if (strcmp(cTemp, "END_SCRIPT") == 0)
				{
					break;
				}
			}//while閉じ

			fclose(pFile);
		}
		else
		{
			assert(("モーションデータ読み込みに失敗", false));
		}
	}
}

//=====================================================
// パーツの最大半径取得
//=====================================================
float CMotionDiv::GetRadiusMax(int nNum)
{
	float fRadiusMax = 0.0f;
	float fTemp;

	for (int nCntParts = 0; nCntParts < m_aNumParts[nNum]; nCntParts++)
	{
		if (m_apParts[nCntParts] != nullptr)
		{// パーツの半径取得
			fTemp = m_apParts[nNum][nCntParts]->pParts->GetRadius();

			if (fTemp > fRadiusMax)
			{// 最大半径設定
				fRadiusMax = fTemp;
			}
		}
	}

	return fRadiusMax;
}

//=====================================================
// パーツの絶対位置取得
//=====================================================
D3DXVECTOR3 CMotionDiv::GetMtxPos(int nNum,int nIdx)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (nIdx < 0 &&
		nIdx >= m_aNumParts[nNum])
	{// 範囲外制限
		return pos;
	}

	if (m_apParts[nNum][nIdx] != nullptr)
	{
		if (m_apParts[nNum][nIdx]->pParts != nullptr)
		{
			D3DXMATRIX mtx = *m_apParts[nNum][nIdx]->pParts->GetMatrix();

			pos =
			{
				mtx._41,
				mtx._42,
				mtx._43,
			};
		}
	}

	return pos;
}

//=====================================================
// 残像の設定
//=====================================================
void CMotionDiv::SetAfterImage(int nNum,D3DXCOLOR col, int m_nLife)
{
	for (int nCntParts = 0; nCntParts < m_aNumParts[nNum]; nCntParts++)
	{
		if (m_apParts[nNum][nCntParts] != nullptr)
		{// 残像設定
			D3DXMATRIX* pMtx = m_apParts[nNum][nCntParts]->pParts->GetMatrix();
			CModel::Model* model = m_apParts[nNum][nCntParts]->pParts->GetModel();

			//CAfterImage::Create(*m_apParts[nCntParts]->m_pParts->GetMatrix(), m_apParts[nCntParts]->m_pParts->GetIdxModel(),col,m_nLife);
		}
	}
}

//=====================================================
// 全パーツの色設定
//=====================================================
void CMotionDiv::SetAllCol(D3DXCOLOR col)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int i = 0; i < m_aNumParts[nCntDiv]; i++)
		{
			if (m_apParts[nCntDiv][i] != nullptr)
			{
				if (m_apParts[nCntDiv][i]->pParts != nullptr)
				{
					m_apParts[nCntDiv][i]->pParts->SetEmissiveCol(col);
				}
			}
		}
	}
}

//=====================================================
// 全パーツの色リセット
//=====================================================
void CMotionDiv::ResetAllCol(void)
{
	for (int nCntDiv = 0; nCntDiv < DIV_BODY_MAX; nCntDiv++)
	{
		for (int i = 0; i < m_aNumParts[nCntDiv]; i++)
		{
			if (m_apParts[nCntDiv][i] != nullptr)
			{
				if (m_apParts[nCntDiv][i]->pParts != nullptr)
				{
					m_apParts[nCntDiv][i]->pParts->ResetColor();
				}
			}
		}
	}

}

//=====================================================
// 生成処理
//=====================================================
CMotionDiv* CMotionDiv::Create(char* pPathLower, char* pPathUpper)
{
	CMotionDiv* pCMotionDiv = nullptr;

	if (pCMotionDiv == nullptr)
	{
		// インスタンス生成
		pCMotionDiv = new CMotionDiv;

		// 読込処理
		pCMotionDiv->Load(pPathLower, pPathUpper);

		// 初期化処理
		pCMotionDiv->Init();
	}

	return pCMotionDiv;
}