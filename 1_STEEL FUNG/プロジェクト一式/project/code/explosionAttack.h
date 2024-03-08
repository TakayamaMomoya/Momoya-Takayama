//*****************************************************
//
// 爆発攻撃の処理[explosionAttack.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ExplAttack_H_
#define _ExplAttack_H_

#include "Object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CAnim3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CExplAttack : public CObject
{
public:
	CExplAttack(int nPriority = 3);	// コンストラクタ
	~CExplAttack();	// デストラクタ

	static CExplAttack *Create(D3DXVECTOR3 pos,float fRadius);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

private:
	float m_fRadius;	// 半径
	D3DXVECTOR3 m_pos;	// 位置
	CCollisionSphere *m_pCollision;	// 当たり判定
	CAnim3D *m_pAnim;	// アニメーションエフェクト
};

#endif
