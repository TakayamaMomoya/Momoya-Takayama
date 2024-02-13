//*****************************************************
//
// ミサイル処理[missile.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MISSILE_H_
#define _MISSILE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "collision.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_HIT	(54)	// 最大で覚えられる命中したオブジェクト

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class COrbit;
class CCollisionSphere;
class CObjectX;
class CObject3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CMissile : public CObject
{
public:
	CMissile(int nPriority = 6);	// コンストラクタ
	~CMissile();	// デストラクタ

	static CMissile *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetNumAll(void) { return m_nNumAll; }
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_info.posOld; }
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetMove(D3DXVECTOR3 move) { m_info.move = move; }
	void SetRot(D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetDamage(float fDamage) { m_info.fDamage = fDamage; }

	void SetIdxPlayer(int nIdx) { m_info.nIdxPlayer = nIdx; }

private:
	struct SInfoVisual
	{
		CObjectX *pMissile;	// ミサイル本体の見た目
		CObject3D *pBackLight;	// 後方の光
	};
	struct SInfo
	{
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
		D3DXVECTOR3 move;	// 移動量
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 前回の位置
		float fLife;	// 寿命
		float fSpeed;	// 速度
		CCollisionSphere *pCollisionSphere;	// 球の当たり判定
		float fDamage;	// 与ダメージ
		float fRadiusExplosion;	// 爆発半径
		int nIdxPlayer;			// プレイヤー番号
	};
	void CreateVisual(void);
	void ManageMove(void);
	void FollowVisual(void);
	void Death(void);
	void DeleteVisual(void);

	SInfo m_info;	// 情報
	SInfoVisual m_infoVisual;	// 見た目の情報
	static int m_nNumAll;	// 総数
};

#endif