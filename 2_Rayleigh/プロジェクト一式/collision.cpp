//*****************************************************
//
// 当たり判定処理[collision.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "collision.h"
#include "main.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"
#include "debugproc.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_EDGE	(4)	// 辺の数

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CCollision *CCollision::m_apCollision[NUM_OBJECT] = {};	// 当たり判定管理用の配列
int CCollision::m_nNumAll = 0;

//=====================================================
// コンストラクタ
//=====================================================
CCollision::CCollision()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_tag = TAG_NONE;
	m_pObjectOwner = nullptr;
	m_pObjectOther = nullptr;

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (m_apCollision[nCnt] == nullptr)
		{// 保管する
			m_apCollision[nCnt] = this;

			// ID記憶
			m_nID = nCnt;

			m_nNumAll++;

			break;
		}
	}
}

//=====================================================
// デストラクタ
//=====================================================
CCollision::~CCollision()
{
	m_nNumAll--;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CCollision::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CCollision::Uninit(void)
{
	if (m_apCollision[m_nID] != nullptr)
	{
		m_apCollision[m_nID] = nullptr;
	}

	if (m_pObjectOwner != nullptr)
	{
		m_pObjectOwner = nullptr;
	}

	if (m_pObjectOther != nullptr)
	{
		m_pObjectOther = nullptr;
	}

	delete this;
}

//=====================================================
// 削除処理
//=====================================================
void CCollision::DeleteAll(void)
{
	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (m_apCollision[nCnt] != nullptr)
		{// 削除する
			m_apCollision[nCnt]->Uninit();
		}
	}
}

//=====================================================
// 更新処理
//=====================================================
void CCollision::Update(void)
{

}

//=====================================================
// キューブとのトリガー判定
//=====================================================
bool CCollision::TriggerCube(TAG tag)
{
	// 変数宣言
	bool bHitOld = false;
	bool bHit = false;
	D3DXVECTOR3 vtxMax;
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;

	// 当たり判定の取得
	CCollision **ppCollision = GetCollision();

	pos = GetOwner()->GetPosition();
	posOld = GetOwner()->GetPositionOld();

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] != nullptr)
		{
			if (ppCollision[nCnt]->GetType() == TYPE_CUBE)
			{
				if (ppCollision[nCnt]->GetTag() != tag && tag != TAG_NONE)
				{
					continue;
				}

				vtxMax = ppCollision[nCnt]->GetVtxMax() + ppCollision[nCnt]->GetPosition();
				vtxMin = ppCollision[nCnt]->GetVtxMin() + ppCollision[nCnt]->GetPosition();

				if (pos.y > vtxMin.y && pos.y < vtxMax.y &&
					pos.x > vtxMin.x && pos.x < vtxMax.x &&
					pos.z > vtxMin.z && pos.z < vtxMax.z)
				{// 現在ブロックの中にいる
					if (posOld.y < vtxMin.y || posOld.y > vtxMax.y ||
						posOld.x < vtxMin.x || posOld.x > vtxMax.x ||
						posOld.z < vtxMin.z || posOld.z > vtxMax.z)
					{// 前回ブロックの中にいない
						bHit = true;
					}
				}
			}
		}
	}

	return bHit;
}

//=====================================================
// キューブが二点間にあるか判定
//=====================================================
bool CCollision::ChckObstclBtwn(CObject *pObj, D3DXVECTOR3 vecDiff)
{
	D3DXVECTOR3 pos,posTarget;
	D3DXVECTOR3 vtxMax, vtxMin;
	D3DXVECTOR3 vtxMaxOwn, vtxMinOwn;
	D3DXVECTOR3 aVector[NUM_EDGE];
	int nIdx;
	float fRate;

	if (pObj == nullptr)
	{
		return false;
	}

	// 受け取ったオブジェクトの位置を取得
	pos = pObj->GetPosition();
	posTarget = pos + vecDiff;

	// 当たり判定の取得
	CCollision **ppCollision = GetCollision();

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] != nullptr)
		{
			if (ppCollision[nCnt]->GetType() == TYPE_CUBE && ppCollision[nCnt] != this)
			{
				if (ppCollision[nCnt]->GetTag() != TAG_BLOCK)
				{// タグに合わなければ繰り返し
					continue;
				}

				// 最大・最小頂点の取得
				vtxMax = ppCollision[nCnt]->GetVtxMax() + ppCollision[nCnt]->GetPosition();
				vtxMin = ppCollision[nCnt]->GetVtxMin() + ppCollision[nCnt]->GetPosition();
				vtxMaxOwn = GetVtxMax();
				vtxMinOwn = GetVtxMin();

				// 四点の位置を設定
				aVector[0] = D3DXVECTOR3(vtxMin.x - vtxMaxOwn.x, 0.0f, vtxMin.z - vtxMaxOwn.z);
				aVector[1] = D3DXVECTOR3(vtxMin.x - vtxMaxOwn.x, 0.0f, vtxMax.z - vtxMinOwn.z);
				aVector[2] = D3DXVECTOR3(vtxMax.x - vtxMinOwn.x, 0.0f, vtxMax.z - vtxMinOwn.z);
				aVector[3] = D3DXVECTOR3(vtxMax.x - vtxMinOwn.x, 0.0f, vtxMin.z - vtxMaxOwn.z);
				
				// どのベクトルにトリガー判定が入ったのか確認
				for (int nCntEdge = 0; nCntEdge < NUM_EDGE; nCntEdge++)
				{
					nIdx = (nCntEdge + 1) % NUM_EDGE;

					if (IsCross(pos, aVector[nCntEdge], aVector[nIdx], &fRate, vecDiff) &&
						IsCross(posTarget, aVector[nCntEdge], aVector[nIdx], &fRate, vecDiff) == false)
					{
						fRate *= 1;

						if (fRate < 1.0f && fRate > 0.0f)
						{// 線分が辺に接触していたら

							if (pos.y < vtxMax.y)
							{
								return true;
							}
						}
					}

				}
			}
		}
	}

	return false;
}

//=====================================================
// 外積の当たり判定計算
//=====================================================
bool CCollision::IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *pRate,D3DXVECTOR3 move)
{
	bool bHit = false;

	D3DXVECTOR3 vec1 = vecDest - vecSorce;

	if (vec1.z * (posTarget.x - vecSorce.x) - vec1.x * (posTarget.z - vecSorce.z) < 0)
	{
		D3DXVECTOR3 vecToPos = posTarget - vecSorce;

		if (pRate != nullptr)
		{
			// 割合を算出
			float fAreaMax = (vecDest.z * move.x) - (vecDest.x * move.z);
			float fArea = (vecToPos.z * move.x) - (vecToPos.x * move.z);
			*pRate = fArea / fAreaMax;
		}

		bHit = true;
	}

	return bHit;
}

//=====================================================
// 外積のトリガー判定計算
//=====================================================
bool CCollision::IsCrossTrigger(D3DXVECTOR3 posTarget, D3DXVECTOR3 posTargetOld, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest)
{
	bool bHit = false;

	D3DXVECTOR3 vec = vecDest - vecSorce;

	if (vec.z * (posTarget.x - vecSorce.x) - vec.x * (posTarget.z - vecSorce.z) > 0 &&
		vec.z * (posTargetOld.x - vecSorce.x) - vec.x * (posTargetOld.z - vecSorce.z) < 0)
	{
		bHit = true;
	}

	return bHit;
}

//=====================================================
// 生成処理
//=====================================================
CCollision *CCollision::Create(TAG tag, TYPE type, CObject *obj)
{
	CCollision *pCollision = nullptr;

	if (pCollision == nullptr)
	{
		// インスタンス生成
		switch (type)
		{
		case TYPE_SPHERE:
			pCollision = new CCollisionSphere;
			break;
		default:
			break;
		}

		if (pCollision != nullptr)
		{
			// 初期化処理
			pCollision->Init();

			// タグ受け取り
			pCollision->m_tag = tag;

			// 持ち主のポインタ
			pCollision->m_pObjectOwner = obj;
		}
	}

	return pCollision;
}

//=====================================================
// コンストラクタ
//=====================================================
CCollisionSphere::CCollisionSphere()
{
	m_fRadius = 0.0f;

	SetType(TYPE_SPHERE);
}

//=====================================================
// デストラクタ
//=====================================================
CCollisionSphere::~CCollisionSphere()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CCollisionSphere::Init(void)
{
	m_fRadius = 100.0f;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CCollisionSphere::Uninit(void)
{
	CCollision::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CCollisionSphere::Update(void)
{

}

//=====================================================
// 球の当たり判定
//=====================================================
bool CCollisionSphere::SphereCollision(TAG tag)
{
	bool bHit = false;

	CCollision **ppCollision = GetCollision();

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] != nullptr)
		{
			if (ppCollision[nCnt]->GetType() == TYPE_SPHERE)
			{
				if (tag == TAG_NONE)
				{

				}
				else if (ppCollision[nCnt]->GetTag() != tag)
				{
					continue;
				}

				// 差分取得
				D3DXVECTOR3 vecDiff = ppCollision[nCnt]->GetPosition() - GetPosition();

				float fLength = D3DXVec3Length(&vecDiff);

				if (fLength < ppCollision[nCnt]->GetRadius() + GetRadius())
				{
					// ぶつかった相手の持ち主を判別
					SetOther(ppCollision[nCnt]->GetOwner());

					return true;
				}
			}
		}
	}

	return bHit;
}

//=====================================================
// 球を通り抜けた判定
//=====================================================
bool CCollisionSphere::IsTriggerExit(TAG tag)
{
	bool bExit = false;

	CCollision **ppCollision = GetCollision();

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] != nullptr)
		{
			if (ppCollision[nCnt]->GetType() == TYPE_SPHERE)
			{
				if (tag == TAG_NONE)
				{

				}
				else if (ppCollision[nCnt]->GetTag() != tag)
				{
					continue;
				}

				// 差分取得
				D3DXVECTOR3 vecDiff = ppCollision[nCnt]->GetPosition() - GetPosition();

				// 差分の長さ
				float fLengthDiff = D3DXVec3Length(&vecDiff);

				// 前回の位置との差分
				vecDiff = ppCollision[nCnt]->GetOwner()->GetPositionOld() - GetPositionOld();

				float fLengthOld = D3DXVec3Length(&vecDiff);

				// ぶつかる時の距離
				float fLength = ppCollision[nCnt]->GetRadius() + GetRadius();

				if (fLengthDiff >= fLength && fLengthOld < fLength)
				{
					// ぶつかった相手の持ち主を判別
					SetOther(ppCollision[nCnt]->GetOwner());

					return true;
				}
			}
		}
	}

	return bExit;
}

//=====================================================
// 球がぶつかった瞬間の判定
//=====================================================
bool CCollisionSphere::IsTriggerEnter(TAG tag)
{
	bool bExit = false;

	CCollision **ppCollision = GetCollision();

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] != nullptr)
		{
			if (ppCollision[nCnt]->GetType() == TYPE_SPHERE)
			{
				if (tag == TAG_NONE)
				{

				}
				else if (ppCollision[nCnt]->GetTag() != tag)
				{
					continue;
				}

				// 差分取得
				D3DXVECTOR3 vecDiff = ppCollision[nCnt]->GetPosition() - GetPosition();

				// 差分の長さ
				float fLengthDiff = D3DXVec3Length(&vecDiff);

				// 前回の位置との差分
				vecDiff = ppCollision[nCnt]->GetOwner()->GetPositionOld() - GetPositionOld();

				float fLengthOld = D3DXVec3Length(&vecDiff);

				// ぶつかる時の距離
				float fLength = ppCollision[nCnt]->GetRadius() + GetRadius();

				if (fLengthDiff <= fLength && fLengthOld > fLength)
				{
					// ぶつかった相手の持ち主を判別
					SetOther(ppCollision[nCnt]->GetOwner());

					return true;
				}
			}
		}
	}

	return bExit;
}

//=====================================================
// 生成処理
//=====================================================
CCollisionSphere *CCollisionSphere::Create(TAG tag, TYPE type, CObject *obj)
{
	CCollisionSphere *pCollision = nullptr;

	if (pCollision == nullptr)
	{
		// インスタンス生成
		pCollision = new CCollisionSphere;

		if (pCollision != nullptr)
		{
			// 初期化処理
			pCollision->Init();

			// タグ受け取り
			pCollision->SetTag(tag);

			// 持ち主のポインタ
			pCollision->SetOwner(obj);
		}
	}

	return pCollision;
}

//=====================================================
// コンストラクタ
//=====================================================
CCollisionCube::CCollisionCube()
{
	m_vtxMax = { 0.0f,0.0f,0.0f };
	m_vtxMin = { 0.0f,0.0f,0.0f };

	SetType(TYPE_CUBE);
}

//=====================================================
// デストラクタ
//=====================================================
CCollisionCube::~CCollisionCube()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CCollisionCube::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CCollisionCube::Uninit(void)
{
	CCollision::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CCollisionCube::Update(void)
{

}

//=====================================================
// 立方体の当たり判定
//=====================================================
bool CCollisionCube::CubeCollision(TAG tag, D3DXVECTOR3 *pMove)
{
	bool bLand = false;
	D3DXVECTOR3 pos;
	CCollision **ppCollision = GetCollision();

	// 計算用変数
	D3DXVECTOR3 posOwn;
	D3DXVECTOR3 posOwnOld;
	D3DXVECTOR3 vtxMax;
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMaxOwn;
	D3DXVECTOR3 vtxMinOwn;

	if (GetOwner() != nullptr)
	{// 自分の設定
		posOwn = GetOwner()->GetPosition();
		posOwnOld = GetOwner()->GetPositionOld();
		vtxMaxOwn = GetVtxMax();
		vtxMinOwn = GetVtxMin();
	}

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] != nullptr)
		{
			if (ppCollision[nCnt]->GetType() == TYPE_CUBE)
			{
				if (ppCollision[nCnt]->GetTag() != tag && tag != TAG_NONE)
				{// タグに合わなければ繰り返し
					continue;
				}

				// 相手の位置を取得
				pos = ppCollision[nCnt]->GetPosition();

				// 相手の最大頂点を取得
				vtxMax = ppCollision[nCnt]->GetVtxMax() + ppCollision[nCnt]->GetPosition();
				vtxMin = ppCollision[nCnt]->GetVtxMin() + ppCollision[nCnt]->GetPosition();

				if (posOwn.y >= vtxMin.y - vtxMaxOwn.y &&
					posOwn.y <= vtxMax.y)
				{//上下で当っている場合
					if (posOwn.z >= vtxMin.z - vtxMaxOwn.z &&
						posOwn.z <= vtxMax.z + vtxMaxOwn.z &&
						posOwn.x >= vtxMin.x - vtxMaxOwn.x &&
						posOwn.x <= vtxMax.x - vtxMinOwn.x)
					{// XZ平面の中にいる場合
						if (posOwnOld.y <= vtxMin.y - vtxMaxOwn.y)
						{// 下から当たった場合
							posOwn.y = vtxMin.y - vtxMaxOwn.y;
						}
						else if (posOwnOld.y >= vtxMax.y)
						{// 上から当たった場合
							posOwn.y = vtxMax.y;

							// 移動量をなくす
							pMove->y = 0.0f;

							bLand = true;
						}
					}
				}

				GetOwner()->SetPosition(posOwn);
			}
		}
	}

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] != nullptr)
		{
			if (ppCollision[nCnt]->GetType() == TYPE_CUBE)
			{
				if (ppCollision[nCnt]->GetTag() != tag && tag != TAG_NONE)
				{// タグに合わなければ繰り返し
					continue;
				}

				// 相手の位置を取得
				pos = ppCollision[nCnt]->GetPosition();

				// 相手の最大頂点を取得
				vtxMax = ppCollision[nCnt]->GetVtxMax() + ppCollision[nCnt]->GetPosition();
				vtxMin = ppCollision[nCnt]->GetVtxMin() + ppCollision[nCnt]->GetPosition();

				if (posOwn.y >= vtxMin.y - vtxMaxOwn.y &&
					posOwn.y <= vtxMax.y)
				{//上下で当っている場合
					if (posOwn.z >= vtxMin.z - vtxMaxOwn.z &&
						posOwn.z <= vtxMax.z + vtxMaxOwn.z)
					{//横からの当たり判定
						if (posOwnOld.x >= vtxMax.x - vtxMinOwn.x &&
							posOwn.x <= vtxMax.x - vtxMinOwn.x)
						{//右から当たった場合
							//右に戻す
							posOwn.x = vtxMax.x - vtxMinOwn.x;

							//移動量をなくす
							pMove->x = 0;
						}

						if (posOwnOld.x <= vtxMin.x - vtxMaxOwn.x &&
							posOwn.x >= vtxMin.x - vtxMaxOwn.x)
						{//左から当たった場合
							//左に戻す
							posOwn.x = vtxMin.x - vtxMaxOwn.x;

							//移動量をなくす
							pMove->x = 0;
						}
					}
				}

				GetOwner()->SetPosition(posOwn);
			}
		}
	}

	return bLand;
}

//=====================================================
// 頂点設定
//=====================================================
void CCollisionCube::SetVtx(D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	m_vtxMax = vtxMax; 
	m_vtxMin = vtxMin;
}

//=====================================================
// 当たったベクトルの取得
//=====================================================
D3DXVECTOR3 CCollisionCube::CollisionVector(CObject *pObj)
{
	D3DXVECTOR3 vecDest = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 pos, posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 vecToPos;
	D3DXVECTOR3 vtxMax, vtxMin;
	D3DXVECTOR3 vtxMaxOwn, vtxMinOwn;
	D3DXVECTOR3 aVector[NUM_EDGE];
	int nIdx;
	float fLengthSorce;
	float fLengthDest;

	if (pObj == nullptr)
	{
		return vecDest;
	}

	// 受け取ったオブジェクトの位置を取得
	pos = pObj->GetPosition();
	posOld = pObj->GetPositionOld();

	move = pos - posOld;

	// 当たり判定の取得
	CCollision **ppCollision = GetCollision();

	for (int nCnt = 0; nCnt < NUM_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] != nullptr)
		{
			if (ppCollision[nCnt]->GetType() == TYPE_CUBE && ppCollision[nCnt] != this)
			{
				if (ppCollision[nCnt]->GetTag() != TAG_BLOCK)
				{// タグに合わなければ繰り返し
					continue;
				}

				vtxMax = ppCollision[nCnt]->GetVtxMax() + ppCollision[nCnt]->GetPosition();
				vtxMin = ppCollision[nCnt]->GetVtxMin() + ppCollision[nCnt]->GetPosition();
				vtxMaxOwn = GetVtxMax();
				vtxMinOwn = GetVtxMin();

				if (pos.z >= vtxMin.z - vtxMaxOwn.z &&
					pos.z <= vtxMax.z - vtxMinOwn.z &&
					pos.x >= vtxMin.x - vtxMaxOwn.x &&
					pos.x <= vtxMax.x - vtxMinOwn.x &&
					pos.y >= vtxMin.y - vtxMaxOwn.y &&
					pos.y <= vtxMax.y - vtxMinOwn.y)
				{// 現在ブロックの中にいる
					aVector[0] = D3DXVECTOR3(vtxMin.x - vtxMaxOwn.x, 0.0f, vtxMin.z - vtxMaxOwn.z);
					aVector[1] = D3DXVECTOR3(vtxMin.x - vtxMaxOwn.x, 0.0f, vtxMax.z - vtxMinOwn.z);
					aVector[2] = D3DXVECTOR3(vtxMax.x - vtxMinOwn.x, 0.0f, vtxMax.z - vtxMinOwn.z);
					aVector[3] = D3DXVECTOR3(vtxMax.x - vtxMinOwn.x, 0.0f, vtxMin.z - vtxMaxOwn.z);

					// どのベクトルにトリガー判定が入ったのか確認
					for (int nCntEdge = 0; nCntEdge < NUM_EDGE; nCntEdge++)
					{
						nIdx = (nCntEdge + 1) % NUM_EDGE;

						if (IsCrossTrigger(pos, posOld, aVector[nCntEdge], aVector[nIdx]))
						{
							vecDest = aVector[nIdx] - aVector[nCntEdge];

							vecToPos = pos - aVector[nIdx];

							// 割合を算出
							float fAreaMax = (vecDest.z * move.x) - (vecDest.x * move.z);
							float fArea = (vecToPos.z * move.x) - (vecToPos.x * move.z);
							float fRate = fArea / fAreaMax;

							fLengthDest = D3DXVec3Length(&aVector[nIdx]);
							fLengthSorce = D3DXVec3Length(&aVector[nCntEdge]);

							// 最短距離によって目標ベクトル反転
							if (fLengthSorce < fLengthDest)
							{
								vecDest *= -1.0f - fRate;
							}
							else
							{
								vecDest *= -fRate;
							}
						}
					}
				}
			}
		}
	}

	return vecDest;
}

//=====================================================
// 生成処理
//=====================================================
CCollisionCube *CCollisionCube::Create(TAG tag, CObject *obj)
{
	CCollisionCube *pCollision = nullptr;

	if (pCollision == nullptr)
	{
		// インスタンス生成
		pCollision = new CCollisionCube;

		if (pCollision != nullptr)
		{
			// 初期化処理
			pCollision->Init();

			// タグ受け取り
			pCollision->SetTag(tag);

			// 持ち主のポインタ
			pCollision->SetOwner(obj);
		}
	}

	return pCollision;
}