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
#include "afterimage.h"
#include "universal.h"

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
	m_bLoopMotion = false;
	m_motionType = 0;
	m_motionTypeOld = 0;
	m_nCounterMotion = 0;
	m_nKey = 0;
	m_nNumKey = 0;
	m_nNumMotion = 0;
	m_nNumParts = 0;
	m_bFinish = false;
	m_bShadow = false;
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_posShadow = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
}

//=====================================================
// デストラクタ
//=====================================================
CMotion::~CMotion()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMotion::Uninit(void)
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{// パーティクル情報の破棄
		if (m_aMotionInfo[nCntMotion].pParticle != nullptr)
		{
			delete m_aMotionInfo[nCntMotion].pParticle;
			m_aMotionInfo[nCntMotion].pParticle = nullptr;
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

	// 汎用処理取得
	CUniversal *pUniversal = CUniversal::GetInstance();

	// 現在の位置を保存
	m_posOld = m_pos;

	//変数宣言
	int nNextKey;

	// パーツのトランスフォーム
	D3DXVECTOR3 pos, rot;

	if (m_aMotionInfo[m_motionType].pParticle != nullptr)
	{
		for (int nCntParticle = 0; nCntParticle < m_aMotionInfo[m_motionType].nNumParticle; nCntParticle++)
		{// 全てのパーティクルを確認
			if (m_nKey == m_aMotionInfo[m_motionType].pParticle[nCntParticle].nKey &&
				m_nCounterMotion == m_aMotionInfo[m_motionType].pParticle[nCntParticle].nFrame)
			{// パーティクル生成
				// 親パーツの位置取得
				D3DXMATRIX *pMtx = m_apParts[m_aMotionInfo[m_motionType].pParticle[nCntParticle].nIdxParent]->m_pParts->GetMatrix();
				pos = D3DXVECTOR3(pMtx->_41, pMtx->_42, pMtx->_43) + m_aMotionInfo[m_motionType].pParticle[nCntParticle].offset;

				CParticle::Create(pos, (CParticle::TYPE)m_aMotionInfo[m_motionType].pParticle[nCntParticle].nType);
			}
		}
	}

	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_nKey >= m_aMotionInfo[m_motionType].nNumKey - 1)
		{
			break;
		}

		// パーツのトランスフォーム取得
		pos = m_apParts[nCntParts]->m_pParts->GetPosOrg();
		rot = m_apParts[nCntParts]->m_pParts->GetRot();

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

		pUniversal->LimitRot(&DiffRotX);
		pUniversal->LimitRot(&DiffRotY);
		pUniversal->LimitRot(&DiffRotZ);

		//目的の値=======================================================================================================
		float DestPosX = pos.x + m_aKeyOld[nCntParts].fPosX +
			DiffPosX * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestPosY = pos.y + m_aKeyOld[nCntParts].fPosY +
			DiffPosY * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestPosZ = pos.z + m_aKeyOld[nCntParts].fPosZ +
			DiffPosZ * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestRotX = m_aKeyOld[nCntParts].fRotX +
			DiffRotX * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestRotY = m_aKeyOld[nCntParts].fRotY +
			DiffRotY * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestRotZ = m_aKeyOld[nCntParts].fRotZ +
			DiffRotZ * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		//パーツの向き・位置設定
		m_apParts[nCntParts]->m_pParts->SetPosition(D3DXVECTOR3(DestPosX, DestPosY, DestPosZ));

		pUniversal->LimitRot(&DestRotX);
		pUniversal->LimitRot(&DestRotY);
		pUniversal->LimitRot(&DestRotZ);

		rot = D3DXVECTOR3(DestRotX, DestRotY, DestRotZ);

		m_apParts[nCntParts]->m_pParts->SetRot(rot);
	}

	m_nCounterMotion++;

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

	if (m_nCounterMotion > m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame)
	{//キーのフレーム数に達したら
		if (m_nKey < m_aMotionInfo[m_motionType].nNumKey)
		{
			m_nKey++;

			m_nCounterMotion = 0;

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

	// モーション情報の設定
	m_motionType = nMotionType;
	m_nKey = 0;
	m_nCounterMotion = 0;
}

//=====================================================
// 終了するキーの情報保存
//=====================================================
void CMotion::SetKeyOld(void)
{
	for (int nCntPart = 0;nCntPart < m_nNumParts;nCntPart++)
	{
		m_aKeyOld[nCntPart].fPosX = m_apParts[nCntPart]->m_pParts->GetPosition().x - m_apParts[nCntPart]->m_pParts->GetPosOrg().x;
		m_aKeyOld[nCntPart].fPosY = m_apParts[nCntPart]->m_pParts->GetPosition().y - m_apParts[nCntPart]->m_pParts->GetPosOrg().y;
		m_aKeyOld[nCntPart].fPosZ = m_apParts[nCntPart]->m_pParts->GetPosition().z - m_apParts[nCntPart]->m_pParts->GetPosOrg().z;

		m_aKeyOld[nCntPart].fRotX = m_apParts[nCntPart]->m_pParts->GetRot().x;
		m_aKeyOld[nCntPart].fRotY = m_apParts[nCntPart]->m_pParts->GetRot().y;
		m_aKeyOld[nCntPart].fRotZ = m_apParts[nCntPart]->m_pParts->GetRot().z;
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
		}
	}
}

//=====================================================
// 自分のマトリックスの設定
//=====================================================
void CMotion::SetMatrix(void)
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
void CMotion::MultiplyMtx(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

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
		pMtx = m_apParts[nCntParts]->m_pParts->GetMatrix();

		//ワールドマトリックス初期化
		D3DXMatrixIdentity(pMtx);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			m_apParts[nCntParts]->m_pParts->GetRot().y, m_apParts[nCntParts]->m_pParts->GetRot().x, m_apParts[nCntParts]->m_pParts->GetRot().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxRotModel);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel,
			m_apParts[nCntParts]->m_pParts->GetPosition().x, m_apParts[nCntParts]->m_pParts->GetPosition().y, m_apParts[nCntParts]->m_pParts->GetPosition().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxTransModel);
		
		if (m_apParts[nCntParts]->nIdxParent != -1)
		{//親パーツがある場合
			// 親マトリックスの取得
			pMtxParent = m_apParts[m_apParts[nCntParts]->nIdxParent]->m_pParts->GetMatrix();
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

			m_apParts[nCntParts]->m_pParts->DrawShadow();
		}

		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, pMtx);

		m_apParts[nCntParts]->m_pParts->Draw();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CMotion::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// マトリックスをかけ合わせる処理
	MultiplyMtx();
}

//=====================================================
// 読込処理
//=====================================================
void CMotion::Load(char *pPath)
{
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
			fscanf(pFile, "%s", &cTemp[0]);

			//ファイル名読み込み=========================================
			if (strcmp(cTemp, "NUM_MODEL") == 0)
			{
				//"="読み込み
				fscanf(pFile, "%s", &cTemp[0]);

				//モデル数読み込み
				fscanf(pFile, "%d", &m_nNumParts);

				for (int nCntFile = 0; nCntFile < m_nNumParts;)
				{//ファイル名読み込み

					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "MODEL_FILENAME") == 0)
					{//ファイル名読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						char aPath[MAX_STRING];

						// モデルパス読込
						fscanf(pFile, "%s", &aPath[0]);

						m_apParts[nCntFile] = new Parts;

						m_apParts[nCntFile]->m_pParts = CParts::Create();

						int nIdx = CModel::Load(&aPath[0]);

						// モデル読込
						m_apParts[nCntFile]->m_pParts->SetIdxModel(nIdx);
						m_apParts[nCntFile]->m_pParts->BindModel(m_apParts[nCntFile]->m_pParts->GetIdxModel());

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
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "PARTSSET") == 0)
					{//パーツスタート
						while (strcmp(cTemp, "END_PARTSSET") != 0)
						{//終わりまでパーツ設定
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "INDEX") == 0)
							{//番号読み込み
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdx);
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{//親番号読み込み
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdxParent);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								D3DXVECTOR3 pos;

								fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									fscanf(pFile, "%f", &pos[nCntPos]);
								}

								m_apParts[nCntModel]->m_pParts->SetPosition(pos);

								m_apParts[nCntModel]->m_pParts->SetPosOrg(pos);
							}

							if (strcmp(cTemp, "ROT") == 0)
							{//向き読み込み
								D3DXVECTOR3 rot;

								fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntRot = 0; nCntRot < 3; nCntRot++)
								{
									fscanf(pFile, "%f", &rot[nCntRot]);
								}

								m_apParts[nCntModel]->m_pParts->SetRot(rot);
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

					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "LOOP") == 0)
					{//ループ判断
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntMotion].bLoop);
					}

					if (strcmp(cTemp, "NUM_KEY") == 0)
					{//キー数判断
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
					}

					if (strcmp(cTemp, "NUM_PARTICLE") == 0)
					{//パーティクル数判断
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumParticle);

						if (m_aMotionInfo[m_nNumMotion].nNumParticle != 0)
						{
							// パーティクル情報を生成
							m_aMotionInfo[m_nNumMotion].pParticle = new PARTICLE_INFO[m_aMotionInfo[m_nNumMotion].nNumParticle];

							// パーティクル情報初期化
							ZeroMemory(m_aMotionInfo[m_nNumMotion].pParticle, sizeof(PARTICLE_INFO) * m_aMotionInfo[m_nNumMotion].nNumParticle);
						}
					}

					if (strcmp(cTemp, "PARTICLESET") == 0 && m_aMotionInfo[m_nNumMotion].pParticle != 0)
					{// パーティクル情報設定
						while (strcmp(cTemp, "END_PARTICLESET") != 0)
						{//終わりまでパーティクル設定
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "KEY") == 0)
							{// 再生キー取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nKey);
							}

							if (strcmp(cTemp, "FRAME") == 0)
							{// 再生フレーム取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nFrame);
							}

							if (strcmp(cTemp, "TYPE") == 0)
							{// 種類取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nType);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								D3DXVECTOR3 pos;

								fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].offset[nCntPos]);
								}
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{// 親パーツ番号取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nIdxParent);
							}
						}

						nCntParticle++;
					}

					//if (strcmp(cTemp, "NUM_COLLISION") == 0)
					//{// 当たり判定数判断
					//	fscanf(pFile, "%s", &cTemp[0]);

					//	fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumCollision);

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
					//		fscanf(pFile, "%s", &cTemp[0]);

					//		if (strcmp(cTemp, "KEY") == 0)
					//		{// 再生キー取得
					//			fscanf(pFile, "%s", &cTemp[0]);

					//			fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nKey);
					//		}

					//		if (strcmp(cTemp, "FRAME") == 0)
					//		{// 再生フレーム取得
					//			fscanf(pFile, "%s", &cTemp[0]);

					//			fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nFrame);
					//		}

					//		if (strcmp(cTemp, "POS") == 0)
					//		{//位置読み込み
					//			D3DXVECTOR3 pos;

					//			fscanf(pFile, "%s", &cTemp[0]);

					//			for (int nCntPos = 0; nCntPos < 3; nCntPos++)
					//			{
					//				fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].offset[nCntPos]);
					//			}
					//		}

					//		if (strcmp(cTemp, "PARENT") == 0)
					//		{// 親パーツ番号取得
					//			fscanf(pFile, "%s", &cTemp[0]);

					//			fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nIdxParent);
					//		}
					//	}

					//	nCntParticle++;
					//}

					if (strcmp(cTemp, "KEYSET") == 0)
					{//キースタート
						while (strcmp(cTemp, "END_KEYSET") != 0)
						{//終わりまでキー設定

							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "FRAME") == 0)
							{//フレーム数取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
							}

							if (strcmp(cTemp, "KEY") == 0)
							{//フレーム数取得
								while (strcmp(cTemp, "END_KEY") != 0)
								{//終わりまでキー設定
									fscanf(pFile, "%s", &cTemp[0]);

									if (strcmp(cTemp, "POS") == 0)
									{//位置取得
										fscanf(pFile, "%s", &cTemp[0]);

										fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
										fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
										fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
									}

									if (strcmp(cTemp, "ROT") == 0)
									{//向き取得
										fscanf(pFile, "%s", &cTemp[0]);

										fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
										fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
										fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
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
			fTemp = m_apParts[nCntParts]->m_pParts->GetRadius();

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
		if (m_apParts[nIdx]->m_pParts != nullptr)
		{
			D3DXMATRIX mtx = *m_apParts[nIdx]->m_pParts->GetMatrix();

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
			D3DXMATRIX *pMtx = m_apParts[nCntParts]->m_pParts->GetMatrix();
			CModel::Model *model = m_apParts[nCntParts]->m_pParts->GetModel();

			CAfterImage::Create(*m_apParts[nCntParts]->m_pParts->GetMatrix(), m_apParts[nCntParts]->m_pParts->GetIdxModel(),col,m_nLife);
		}
	}
}

//=====================================================
// 全パーツの色設定
//=====================================================
void CMotion::SetAllCol(D3DXCOLOR col)
{
	for (int i = 0; i < m_nNumParts; i++)
	{
		if (m_apParts[i] != nullptr)
		{
			if (m_apParts[i]->m_pParts != nullptr)
			{
				m_apParts[i]->m_pParts->SetEmissiveCol(col);
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
			if (m_apParts[i]->m_pParts != nullptr)
			{
				m_apParts[i]->m_pParts->ResetColor();
			}
		}
	}
}

//=====================================================
// 生成処理
//=====================================================
CMotion *CMotion::Create(char *pPath)
{
	CMotion *pCMotion = nullptr;

	if (pCMotion == nullptr)
	{
		// インスタンス生成
		pCMotion = new CMotion;

		// 読込処理
		pCMotion->Load(pPath);

		// 初期化処理
		pCMotion->Init();
	}

	return pCMotion;
}