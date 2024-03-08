//*****************************************************
//
// 破片処理[debris.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _DERIS_H_
#define _DERIS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// クラス定義
//*****************************************************
class CDebris : public CObjectX
{
public:
	CDebris(int nPriority = 3);
	~CDebris();

	static CDebris* Create(D3DXVECTOR3 pos, int nLife, int nModelType, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f }, float fGravity = 0.0f, bool bBounce = false, int nPriority = 3);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_SHRINK,	// 縮んでいる状態
		STATE_MAX
	};

	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_WALL,	// 壁
		TYPE_SOIL,	// 土
		TYPE_MAX
	};

	void Shrink(void);

	D3DXVECTOR3 m_rotVelocity;	// 回転速度
	D3DXVECTOR3 m_move;	// 移動量
	int m_nLife;	// 寿命
	int m_nModelType;	// モデル種類
	float m_fGravity;	// 重力
	bool m_bBounce;
	STATE m_state;	// 状態
	TYPE m_type;	// 
};

#endif