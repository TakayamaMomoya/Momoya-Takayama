//*****************************************************
//
// モーションの処理[motion.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"
#include "model.h"
#include <stdio.h>
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "particle.h"
#include "sound.h"
#include "universal.h"
#include "slow.h"
#include "effect3D.h"
#include "afterimage.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_STRING	(256)	// 文字列の最大数

//=====================================================
// コンストラクタ
//=====================================================
CMotion::CMotion(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_aMotionInfo, sizeof(m_aMotionInfo));
	ZeroMemory(&m_aKeyOld, sizeof(m_aKeyOld));
	ZeroMemory(&m_apParts[0], sizeof(m_apParts));
	ZeroMemory(&m_abMotion[0], sizeof(m_abMotion));
	m_bLoopMotion = false;
	m_motionType = 0;
	m_motionTypeOld = 0;
	m_fCounterMotion = 0;
	m_nKey = 0;
	m_nFrame = 0;
	m_nNumKey = 0;
	m_nNumMotion = 0;
	m_nNumParts = 0;
	m_bFinish = false;
	m_bShadow = false;
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_posShadow = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_bInde = false;
}

//=====================================================
// デストラクタ
//=====================================================
CMotion::~CMotion()
{

}

//=====================================================
// 生成処理
//=====================================================
CMotion *CMotion::Create(char *pPath)
{
	CMotion *pMotion = nullptr;

	if (pMotion == nullptr)
	{
		// インスタンス生成
		pMotion = new CMotion;

		if (pMotion != nullptr)
		{
			// 読込処理
			pMotion->Load(pPath);

			// 初期化処理
			pMotion->Init();
		}
	}

	return pMotion;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMotion::Init(void)
{
	// 初期モーション設定
	SetMotion(0);
	InitPose(0);

	// 可動パーツリセット
	ResetEnableMotion();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMotion::Uninit(void)
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{// パーティクル情報の破棄
		if (m_aMotionInfo[nCntMotion].pEvent != nullptr)
		{
			delete m_aMotionInfo[nCntMotion].pEvent;
			m_aMotionInfo[nCntMotion].pEvent = nullptr;
		}
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CMotion::Update(void)
{
	if (m_apParts[0] == nullptr)
	{
		return;
	}

	// 現在の位置を保存
	m_posOld = m_pos;

	//変数宣言
	int nNextKey;

	// パーツのトランスフォーム
	D3DXVECTOR3 pos, rot;

	// モーションのトランスフォーム管理
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_abMotion[nCntParts] == false)
		{
			continue;
		}

		if (m_nKey >= m_aMotionInfo[m_motionType].nNumKey - 1)
		{
			break;
		}

		// パーツのトランスフォーム取得
		pos = m_apParts[nCntParts]->pParts->GetPosOrg();
		rot = m_apParts[nCntParts]->pParts->GetRotation();

		if (m_nKey < m_aMotionInfo[m_motionType].nNumKey - 1)
		{
			nNextKey = m_nKey + 1;
		}
		else
		{
			nNextKey = m_aMotionInfo[m_motionType].nNumKey - 1;
		}

		//前回と次のキーの差分取得
		float DiffPosX = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosX -
			m_aKeyOld[nCntParts].fPosX;

		float DiffPosY = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosY -
			m_aKeyOld[nCntParts].fPosY;

		float DiffPosZ = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosZ -
			m_aKeyOld[nCntParts].fPosZ;

		float DiffRotX = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotX -
			m_aKeyOld[nCntParts].fRotX;

		float DiffRotY = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotY -
			m_aKeyOld[nCntParts].fRotY;

		float DiffRotZ = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotZ -
			m_aKeyOld[nCntParts].fRotZ;

		universal::LimitRot(&DiffRotX);
		universal::LimitRot(&DiffRotY);
		universal::LimitRot(&DiffRotZ);

		//目的の値=======================================================================================================
		float DestPosX = pos.x + m_aKeyOld[nCntParts].fPosX +
			DiffPosX * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestPosY = pos.y + m_aKeyOld[nCntParts].fPosY +
			DiffPosY * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestPosZ = pos.z + m_aKeyOld[nCntParts].fPosZ +
			DiffPosZ * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestRotX = m_aKeyOld[nCntParts].fRotX +
			DiffRotX * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestRotY = m_aKeyOld[nCntParts].fRotY +
			DiffRotY * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestRotZ = m_aKeyOld[nCntParts].fRotZ +
			DiffRotZ * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		//パーツの向き・位置設定
		m_apParts[nCntParts]->pParts->SetPosition(D3DXVECTOR3(DestPosX, DestPosY, DestPosZ));

		universal::LimitRot(&DestRotX);
		universal::LimitRot(&DestRotY);
		universal::LimitRot(&DestRotZ);

		rot = D3DXVECTOR3(DestRotX, DestRotY, DestRotZ);

		m_apParts[nCntParts]->pParts->SetRotation(rot);
	}

	float fFrameOld = m_fCounterMotion;;

	CSlow *pSlow = CSlow::GetInstance();

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		m_fCounterMotion += fScale;
	}
	else
	{
		m_fCounterMotion += 1.0f;
	}

	// イベントの管理
	if (m_aMotionInfo[m_motionType].pEvent != nullptr &&
		m_aMotionInfo[m_motionType].nNumEvent > 0)
	{
		for (int nCntEvent = 0; nCntEvent < m_aMotionInfo[m_motionType].nNumEvent; nCntEvent++)
		{
			if (m_nKey == m_aMotionInfo[m_motionType].pEvent[nCntEvent].nKey &&
				m_fCounterMotion >= m_aMotionInfo[m_motionType].pEvent[nCntEvent].nFrame && 
				fFrameOld < m_aMotionInfo[m_motionType].pEvent[nCntEvent].nFrame)
			{// イベントの開始
				m_aMotionInfo[m_motionType].pEvent[nCntEvent].fTimer = 0.0f;
			}

			if (m_aMotionInfo[m_motionType].pEvent[nCntEvent].fTimer <= m_aMotionInfo[m_motionType].pEvent[nCntEvent].fNumFrame)
			{// イベントの呼び出し
				Event(&m_aMotionInfo[m_motionType].pEvent[nCntEvent]);

				D3DXMATRIX mtxParent = *GetParts(m_aMotionInfo[m_motionType].pEvent[nCntEvent].nIdxParent)->pParts->GetMatrix();
				D3DXMATRIX mtx;

				universal::SetOffSet(&mtx, mtxParent, m_aMotionInfo[m_motionType].pEvent[nCntEvent].offset);

				D3DXVECTOR3 pos =
				{
					mtx._41,
					mtx._42,
					mtx._43
				};

				float fRadius = m_aMotionInfo[m_motionType].pEvent[nCntEvent].fRadius;

				CEffect3D::Create(pos, fRadius, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

				m_aMotionInfo[m_motionType].pEvent[nCntEvent].fTimer += 1.0f;
			}
		}
	}

	if (m_nKey >= m_aMotionInfo[m_motionType].nNumKey - 1)
	{// モーションが終了していたら
		m_motionTypeOld = m_motionType;

		// 終了した状態にする
		m_bFinish = true;

		if (m_aMotionInfo[m_motionType].bLoop)
		{// ループするものは再設定
			SetMotion(m_motionType);
		}
	}

	if (m_fCounterMotion > m_nFrame)
	{//キーのフレーム数に達したら
		if (m_nKey < m_aMotionInfo[m_motionType].nNumKey)
		{
			m_nKey++;

			m_nFrame = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;

			m_fCounterMotion = 0;

			SetKeyOld();
		}
	}
}

//=====================================================
// モーション設定
//=====================================================
void CMotion::SetMotion(int nMotionType)
{
	// 終了状態の解除
	m_bFinish = false;

	// 終了するモーションを保存
	m_motionTypeOld = m_motionType;
	SetKeyOld();

	if (m_nKey != -1)
	{
		m_nFrame = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;
	}

	if (m_motionType != nMotionType)
	{
		m_nFrame = 10;
	}

	// モーション情報の設定
	m_motionType = nMotionType;
	m_nKey = -1;
	m_fCounterMotion = 0;
}

//=====================================================
// 終了するキーの情報保存
//=====================================================
void CMotion::SetKeyOld(void)
{
	for (int nCntPart = 0;nCntPart < m_nNumParts;nCntPart++)
	{
		m_aKeyOld[nCntPart].fPosX = m_apParts[nCntPart]->pParts->GetPosition().x - m_apParts[nCntPart]->pParts->GetPosOrg().x;
		m_aKeyOld[nCntPart].fPosY = m_apParts[nCntPart]->pParts->GetPosition().y - m_apParts[nCntPart]->pParts->GetPosOrg().y;
		m_aKeyOld[nCntPart].fPosZ = m_apParts[nCntPart]->pParts->GetPosition().z - m_apParts[nCntPart]->pParts->GetPosOrg().z;

		m_aKeyOld[nCntPart].fRotX = m_apParts[nCntPart]->pParts->GetRotation().x;
		m_aKeyOld[nCntPart].fRotY = m_apParts[nCntPart]->pParts->GetRotation().y;
		m_aKeyOld[nCntPart].fRotZ = m_apParts[nCntPart]->pParts->GetRotation().z;
	}
}

//=====================================================
// ポーズの初期設定
//=====================================================
void CMotion::InitPose(int nMotion)
{
	for (int i = 0; i < m_aMotionInfo[nMotion].nNumKey;i++)
	{
		for (int nCntPart = 0; nCntPart < m_nNumParts; nCntPart++)
		{
			m_aKeyOld[nCntPart] = m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart];

			D3DXVECTOR3 rot =
			{
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fRotX,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fRotY,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fRotZ
			};

			D3DXVECTOR3 pos =
			{
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fPosX,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fPosY,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fPosZ
			};

			m_apParts[nCntPart]->pParts->SetRotation(rot);
			//m_apParts[nCntPart]->pParts->SetPosition(pos);
		}
	}
}

//=====================================================
// 自分のマトリックスの設定
//=====================================================
void CMotion::CalcMatrix(void)
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
}

//=====================================================
// マトリックスをかけ合わせる処理
//=====================================================
void CMotion::MultiplyMtx(void)
{
	if (m_bInde == false)
	{
		// 親のマトリックス
		CalcMatrix();
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DXMATRIX mtxRotModel, mtxTransModel;
	D3DXMATRIX *pMtxParent;
	D3DXMATRIX *pMtx;

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

	for (int nCntParts = 0;nCntParts < m_nNumParts;nCntParts++)
	{
		// マトリックスの取得
		pMtx = m_apParts[nCntParts]->pParts->GetMatrix();

		//ワールドマトリックス初期化
		D3DXMatrixIdentity(pMtx);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			m_apParts[nCntParts]->pParts->GetRotation().y, m_apParts[nCntParts]->pParts->GetRotation().x, m_apParts[nCntParts]->pParts->GetRotation().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxRotModel);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel,
			m_apParts[nCntParts]->pParts->GetPosition().x, m_apParts[nCntParts]->pParts->GetPosition().y, m_apParts[nCntParts]->pParts->GetPosition().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxTransModel);
		
		if (m_apParts[nCntParts]->nIdxParent != -1)
		{//親パーツがある場合
			// 親マトリックスの取得
			pMtxParent = m_apParts[m_apParts[nCntParts]->nIdxParent]->pParts->GetMatrix();
		}
		else
		{
			pMtxParent = &m_mtxWorld;
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

			m_apParts[nCntParts]->pParts->DrawShadow();
		}

		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, pMtx);

		m_apParts[nCntParts]->pParts->Draw();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CMotion::Draw(void)
{
	// マトリックスをかけ合わせる処理
	MultiplyMtx();
}

//=====================================================
// 読込処理
//=====================================================
void CMotion::Load(char *pPath)
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{// パーティクル情報の破棄
		if (m_aMotionInfo[nCntMotion].pEvent != nullptr)
		{
			delete m_aMotionInfo[nCntMotion].pEvent;
			m_aMotionInfo[nCntMotion].pEvent = nullptr;
		}
	}

	//ZeroMemory(&m_aMotionInfo, sizeof(m_aMotionInfo));
	ZeroMemory(&m_aKeyOld, sizeof(m_aKeyOld));
	ZeroMemory(&m_apParts[0], sizeof(m_apParts));
	//ZeroMemory(&m_abMotion[0], sizeof(m_abMotion));
	m_nKey = 0;
	m_nNumMotion = 0;

	//変数宣言
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	int nCntModel = 0;

	//ファイルから読み込む
	FILE *pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		while(true)
		{
			//文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			//ファイル名読み込み=========================================
			if (strcmp(cTemp, "NUM_MODEL") == 0)
			{
				//"="読み込み
				(void)fscanf(pFile, "%s", &cTemp[0]);

				//モデル数読み込み
				(void)fscanf(pFile, "%d", &m_nNumParts);

				for (int nCntFile = 0; nCntFile < m_nNumParts;)
				{//ファイル名読み込み

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "MODEL_FILENAME") == 0)
					{//ファイル名読み込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						char aPath[MAX_STRING];

						// モデルパス読込
						(void)fscanf(pFile, "%s", &aPath[0]);

						m_apParts[nCntFile] = new Parts;

						m_apParts[nCntFile]->pParts = CParts::Create();

						int nIdx = CModel::Load(&aPath[0]);

						// モデル読込
						m_apParts[nCntFile]->pParts->SetIdxModel(nIdx);
						m_apParts[nCntFile]->pParts->BindModel(m_apParts[nCntFile]->pParts->GetIdxModel());

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

								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdx);
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{//親番号読み込み
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdxParent);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								D3DXVECTOR3 pos;

								(void)fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									(void)fscanf(pFile, "%f", &pos[nCntPos]);
								}

								m_apParts[nCntModel]->pParts->SetPosition(pos);

								m_apParts[nCntModel]->pParts->SetPosOrg(pos);
							}

							if (strcmp(cTemp, "ROT") == 0)
							{//向き読み込み
								D3DXVECTOR3 rot;

								(void)fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntRot = 0; nCntRot < 3; nCntRot++)
								{
									(void)fscanf(pFile, "%f", &rot[nCntRot]);
								}

								m_apParts[nCntModel]->pParts->SetRotation(rot);
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
			int nCntEvent = 0;

			//モーション設定===========================================
			if (strcmp(cTemp, "MOTIONSET") == 0)
			{//モーションスタート
				while (strcmp(cTemp, "END_MOTIONSET") != 0)
				{//終わりまでキャラクター設定

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "LOOP") == 0)
					{//ループ判断
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntMotion].bLoop);
					}

					if (strcmp(cTemp, "NUM_KEY") == 0)
					{//キー数判断
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
					}

					if (strcmp(cTemp, "NUM_EVENT") == 0)
					{// イベント数判断
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumEvent);

						if (m_aMotionInfo[m_nNumMotion].nNumEvent != 0)
						{// イベント情報を生成
							m_aMotionInfo[m_nNumMotion].pEvent = new EVENT_INFO[m_aMotionInfo[m_nNumMotion].nNumEvent];

							ZeroMemory(m_aMotionInfo[m_nNumMotion].pEvent, sizeof(EVENT_INFO) * m_aMotionInfo[m_nNumMotion].nNumEvent);

							for (int i = 0; i < m_aMotionInfo[m_nNumMotion].nNumEvent;i++)
							{
								m_aMotionInfo[m_nNumMotion].pEvent->fTimer = FLT_MAX;
							}
						}
					}

					if (strcmp(cTemp, "EVENTSET") == 0 && m_aMotionInfo[m_nNumMotion].pEvent != 0)
					{// イベント情報設定
						while (strcmp(cTemp, "END_EVENTSET") != 0)
						{// 終わりまでイベント設定
							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "KEY") == 0)
							{// 再生キー取得
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nKey);
							}

							if (strcmp(cTemp, "FRAME") == 0)
							{// 再生フレーム取得
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nFrame);
							}

							if (strcmp(cTemp, "NUM_FRAME") == 0)
							{// 再生フレーム数取得
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fNumFrame);

								m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fTimer = FLT_MAX;
							}

							if (strcmp(cTemp, "RADIUS") == 0)
							{// 半径
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fRadius);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								D3DXVECTOR3 pos;

								(void)fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									(void)fscanf(pFile, "%f", &pos[nCntPos]);
								}

								m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].offset = pos;
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{// 親パーツ番号取得
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nIdxParent);
							}
						}

						nCntEvent++;
					}

					if (strcmp(cTemp, "KEYSET") == 0)
					{//キースタート
						while (strcmp(cTemp, "END_KEYSET") != 0)
						{//終わりまでキー設定

							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "FRAME") == 0)
							{//フレーム数取得
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
							}

							if (strcmp(cTemp, "KEY") == 0)
							{//フレーム数取得
								while (strcmp(cTemp, "END_KEY") != 0)
								{//終わりまでキー設定
									(void)fscanf(pFile, "%s", &cTemp[0]);

									if (strcmp(cTemp, "POS") == 0)
									{//位置取得
										(void)fscanf(pFile, "%s", &cTemp[0]);

										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
									}

									if (strcmp(cTemp, "ROT") == 0)
									{//向き取得
										(void)fscanf(pFile, "%s", &cTemp[0]);

										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
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
				m_nNumMotion++;
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

//=====================================================
// パーツの最大半径取得
//=====================================================
float CMotion::GetRadiusMax(void)
{
	float fRadiusMax = 0.0f;
	float fTemp;

	for (int nCntParts = 0;nCntParts < m_nNumParts;nCntParts++)
	{
		if (m_apParts[nCntParts] != nullptr)
		{// パーツの半径取得
			fTemp = m_apParts[nCntParts]->pParts->GetRadius();

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
D3DXVECTOR3 CMotion::GetMtxPos(int nIdx)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (nIdx < 0 && 
		nIdx >= m_nNumParts)
	{// 範囲外制限
		return pos;
	}

	if (m_apParts[nIdx] != nullptr)
	{
		if (m_apParts[nIdx]->pParts != nullptr)
		{
			D3DXMATRIX mtx = *m_apParts[nIdx]->pParts->GetMatrix();

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
void CMotion::SetAfterImage(D3DXCOLOR col, int m_nLife)
{
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apParts[nCntParts] != nullptr)
		{// 残像設定
			D3DXMATRIX *pMtx = m_apParts[nCntParts]->pParts->GetMatrix();
			CModel::Model *model = m_apParts[nCntParts]->pParts->GetModel();

			CAfterImage::Create(*m_apParts[nCntParts]->pParts->GetMatrix(), m_apParts[nCntParts]->pParts->GetIdxModel(),col,m_nLife);
		}
	}
}

//=====================================================
// 全パーツの色設定
//=====================================================
void CMotion::SetAllCol(D3DXCOLOR col)
{
	m_col = col;

	for (int i = 0; i < m_nNumParts; i++)
	{
		if (m_apParts[i] != nullptr)
		{
			if (m_apParts[i]->pParts != nullptr)
			{
				m_apParts[i]->pParts->SetEmissiveCol(col);
			}
		}
	}
}

//=====================================================
// 全パーツの色リセット
//=====================================================
void CMotion::ResetAllCol(void)
{
	for (int i = 0; i < m_nNumParts; i++)
	{
		if (m_apParts[i] != nullptr)
		{
			if (m_apParts[i]->pParts != nullptr)
			{
				m_apParts[i]->pParts->ResetColor();
			}
		}
	}
}

//=====================================================
// パーツごとにモーションするか設定
//=====================================================
void CMotion::EnableMotion(int nIdx,bool bMotion)
{
	if (nIdx >= 0 &&
		nIdx < MAX_PARTS)
	{
		m_abMotion[nIdx] = bMotion;
	}
	else
	{
		assert(("不正な番号の可動パーツを設定しようとしています", false));
	}
}

//=====================================================
// 可動パーツのリセット
//=====================================================
void CMotion::ResetEnableMotion(void)
{
	for (int i = 0; i < MAX_PARTS; i++)
	{
		m_abMotion[i] = true;
	}
}