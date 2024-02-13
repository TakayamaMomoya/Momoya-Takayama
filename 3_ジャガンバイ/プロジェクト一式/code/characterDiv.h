//*****************************************************
//
// 分割キャラクターの処理[characterDiv.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CHARACTER_DIV_H_
#define _CHARACTER_DIV_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMotionDiv;

//*****************************************************
// 定数定義
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CCharacterDiv : public CObject
{
public:

	enum Parts
	{
		PARTS_LOWER = 0,	// 下半身
		PARTS_UPPER,		// 上半身
		PARTS_MAX
	};

	CCharacterDiv(int nPriority = 4);	// コンストラクタ
	~CCharacterDiv();					// デストラクタ

	static CCharacterDiv* Create(char* pPathLower,char* pPathUpper);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	void SetPositionOld(D3DXVECTOR3 pos) { m_info.posOld = pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_info.posOld; }
	void SetMove(D3DXVECTOR3 move) { m_info.move = move; }
	D3DXVECTOR3 GetMove(void) { return m_info.move; }
	void SetRot(D3DXVECTOR3 rot) { m_info.rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_info.rot; }
	CMotionDiv* GetBody(void) { return m_info.pBody; }
	void Load(char* pPathLower, char* pPathUpper);
	void SetMotion(int nNum,int nMotion);
	int GetMotion(int nNum);
	D3DXMATRIX *GetMatrix(void) { return &m_info.mtxWorld; }

private:
	struct SInfo
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 前回の位置
		D3DXVECTOR3 move;	// 移動量
		D3DXVECTOR3 rot;	// 向き
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
		CMotionDiv* pBody;			// 体
		char* apPath[PARTS_MAX];	// 体のパス
	};

	SInfo m_info;	// 自身の情報
};

#endif