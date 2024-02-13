//*****************************************************
//
// キャラクターの処理[character.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMotion;

//*****************************************************
// クラスの定義
//*****************************************************
class CCharacter : public CObject
{
public:
	CCharacter(int nPriority = 4);	// コンストラクタ
	~CCharacter();	// デストラクタ

	static CCharacter *Create(char *pPath);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	void SetPositionOld(D3DXVECTOR3 pos) { m_info.posOld = pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_info.posOld; }
	void SetMove(D3DXVECTOR3 move) { m_info.move = move; }
	D3DXVECTOR3 GetMove(void) { return m_info.move; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_info.rot; }
	CMotion *GetBody(void) { return m_info.pBody; }
	void Load(char *pPath);
	void SetMotion(int nMotion);
	int GetMotion(void);

private:
	struct SInfo
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 前回の位置
		D3DXVECTOR3 move;	// 移動量
		D3DXVECTOR3 rot;	// 向き
		CMotion *pBody;	// 体
		char *pPath;	// 体のパス
	};

	SInfo m_info;	// 自身の情報
};

#endif