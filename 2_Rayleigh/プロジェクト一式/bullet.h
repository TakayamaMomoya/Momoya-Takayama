//*****************************************************
//
// 弾処理[bullet.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object2D.h"
#include "collision.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_ORBIT	(4)	// 軌跡の数
#define MAX_HIT	(54)	// 最大で覚えられる命中したオブジェクト

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class COrbit;

//*****************************************************
// クラスの定義
//*****************************************************
class CBullet : public CObject
{
public:
	typedef enum
	{
		TYPE_NONE = 0,	// 何でもない
		TYPE_PLAYER,	// プレイヤーの弾
		TYPE_ENEMY,	// 敵の弾
		TYPE_MAX
	}TYPE;

	CBullet(int nPriority = 6);	// コンストラクタ
	~CBullet();	// デストラクタ

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, TYPE type, bool bPierce = false, float fRadius = 10.0f, float fDamage = 5.0f, D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f });
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetNumAll(void) { return m_nNumAll; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }

private:
	bool BulletHit(CCollision::TAG tag);
	void Death(void);

	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;	// 向き
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	int m_nLife;	// 寿命
	TYPE m_type;	// 種類
	static int m_nNumAll;	// 総数
	bool m_bPierce;	// 貫通するかどうか
	CCollisionSphere *m_pCollisionSphere;	// 球の当たり判定
	COrbit *m_apOrbit[NUM_ORBIT];	// 軌跡のポインタ
	D3DXCOLOR m_col;	// 色
	float m_fDamage;	// 与ダメージ
	float m_fSize;	// 弾のサイズ
};

#endif