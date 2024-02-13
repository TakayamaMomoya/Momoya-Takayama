//*****************************************************
//
// 当たり判定処理[collision.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _COLLISION_H_
#define _COLLISION_H_

//*****************************************************
// インクルード
//****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//****************************************************
class CObject2D;

//*****************************************************
// クラス定義
//*****************************************************
class CCollision
{
public:
	typedef enum
	{// 識別タグ
		TAG_NONE = 0,	// 何でもない
		TAG_PLAYER,	// プレイヤー
		TAG_ENEMY,	// 敵
		TAG_PLAYERBULLET,	// プレイヤーの弾
		TAG_ENEMYBULLET,	// 敵の弾
		TAG_BLOCK,	// ブロック
		TAG_MAX
	}TAG;

	typedef enum
	{
		TYPE_NONE = 0, // 何でもない
		TYPE_SPHERE,	// 球の当たり判定
		TYPE_CUBE,	// 立方体の当たり判定
		TYPE_MAX
	}TYPE;

	CCollision();	// コンストラクタ
	~CCollision();	// デストラクタ

	static CCollision *Create(TAG tag,TYPE type,CObject *obj);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static void DeleteAll(void);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	CCollision **GetCollision(void) { return &m_apCollision[0]; }
	TAG GetTag(void) { return m_tag; }
	TYPE GetType(void) { return m_type; }
	void SetType(TYPE type) { m_type = type; }
	void SetTag(TAG tag) { m_tag = tag; }
	virtual float GetRadius(void) { return 0.0f; }
	virtual bool SphereCollision(TAG tag) { return false; }
	void SetOwner(CObject *pObj) { m_pObjectOwner = pObj; }
	void SetOther(CObject *pObj) { m_pObjectOther = pObj; }
	CObject *GetOther(void) { return m_pObjectOther; }
	CObject *GetOwner(void) { return m_pObjectOwner; }
	void ReleaseOther(void) { m_pObjectOther = nullptr; }
	virtual D3DXVECTOR3 GetVtxMax(void) { return D3DXVECTOR3(0.0f,0.0f,0.0f); }
	virtual D3DXVECTOR3 GetVtxMin(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	static bool IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *fRate, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f });
	static bool IsCrossTrigger(D3DXVECTOR3 posTarget, D3DXVECTOR3 posTargetOld,D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest);
	bool TriggerCube(TAG tag);
	bool ChckObstclBtwn(CObject *pObject,D3DXVECTOR3 vecDiff);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	D3DXVECTOR3 m_pos;	// 場所
	D3DXVECTOR3 m_posOld;	// 前回の場所
	TAG m_tag;	// 識別用タグ
	TYPE m_type;
	static CCollision *m_apCollision[NUM_OBJECT];
	int m_nID;	// 番号
	static int m_nNumAll;
	CObject *m_pObjectOwner;
	CObject *m_pObjectOther;
};

class CCollisionSphere : public CCollision
{// 球の当たり判定
public:
	CCollisionSphere();	// コンストラクタ
	~CCollisionSphere();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool SphereCollision(TAG tag);
	float GetRadius(void) { return m_fRadius; }
	static CCollisionSphere *Create(TAG tag, TYPE type, CObject *obj);
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	bool IsTriggerExit(TAG tag);
	bool IsTriggerEnter(TAG tag);

private:
	float m_fRadius;
};

class CCollisionCube : public CCollision
{// 立方体の当たり判定
public:
	CCollisionCube();	// コンストラクタ
	~CCollisionCube();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool CubeCollision(TAG tag, D3DXVECTOR3 *pMove);
	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }
	static CCollisionCube *Create(TAG tag, CObject *obj);
	void SetVtx(D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	D3DXVECTOR3 CollisionVector(CObject *pObj);

private:
	D3DXVECTOR3 m_vtxMax;	// 最大頂点
	D3DXVECTOR3 m_vtxMin;	// 最小頂点
};


#endif