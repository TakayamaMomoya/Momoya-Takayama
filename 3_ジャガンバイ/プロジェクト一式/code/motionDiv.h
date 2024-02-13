//*****************************************************
//
// 分割モーションの処理[motion_div.h]
// Author:酒井南勝
//
//*****************************************************

#ifndef _MOTION_DIV_H_
#define _MOTION_DIV_H_

//*****************************************************
// インクルード
//*****************************************************
#include "parts.h"
#include "object.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_PARTS	(20)	// パーツの最大数
#define MAX_MOTION	(40)	// モーションの最大数

//*****************************************************
// クラスの定義
//*****************************************************
class CMotionDiv : public CObject
{
public:

	enum DIV_BODY
	{// 分割した体
		DIV_BODY_LOWER = 0,	// 下半身
		DIV_BODY_UPPER,		// 上半身
		DIV_BODY_MAX
	};

	//キーの構造体
	typedef struct
	{
		float fPosX;				//位置X
		float fPosY;				//位置Y
		float fPosZ;				//位置Z
		float fRotX;				//向きX
		float fRotY;				//向きY
		float fRotZ;				//向きZ
	}KEY;

	//キー情報構造体
	typedef struct
	{
		int nFrame;					//再生フレーム
		KEY aKey[MAX_PARTS];			//各モデルのキー要素
	}KEY_INFO;

	typedef struct
	{// パーティクル情報構造体
		int nKey;	//生成するキー
		int nFrame;	//生成するフレーム
		int nType;	// 種類
		D3DXVECTOR3 offset;	// オフセット位置
		int nIdxParent;	// 親となるパーツの番号
	}PARTICLE_INFO;

	typedef struct
	{// 当たり判定情報構造体
		int nKey;	//生成するキー
		int nFrame;	//生成するフレーム
		D3DXVECTOR3 offset;	// オフセット位置
		int nIdxParent;	// 親となるパーツの番号
	}COLLISION_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;						//ループするかどうか
		int nNumKey;					//キーの総数
		KEY_INFO aKeyInfo[MAX_PARTS];	//キー情報
		int nNumParticle;	// パーティクルの数
		int nNumCollision;	// 当たり判定の数
		PARTICLE_INFO* pParticle;	// パーティクルのポインタ
		COLLISION_INFO* pCollision;	// 当たり判定のポインタ
	}MOTION_INFO;

	// パーツの構造体
	typedef struct
	{
		int nIdx;
		int nIdxParent;
		CParts* pParts;
	}Parts;

	// メンバ関数
	CMotionDiv(int nPriority = 5);	// コンストラクタ
	~CMotionDiv();	// デストラクタ
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Load(char* pPathLower, char* pPathUpper);
	void MultiplyMtx(void);
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 設定処理
	void SetPosShadow(D3DXVECTOR3 pos) { m_posShadow = pos; }	// 設定処理
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	D3DXVECTOR3* GetPosAddress(void) { return &m_pos; }	// 取得処理
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	D3DXVECTOR3 GetMtxPos(int nNum, int nIdx);
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }	// 取得処理
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMotion(int nNum, int nMotionType);
	int GetMotion(int nNum) { return m_aMotionType[nNum]; }
	void SetKeyOld(void);
	static CMotionDiv* Create(char* pPathLower, char* pPathUpper);
	Parts* GetParts(int nNum,int nIdx) { return m_apParts[nNum][nIdx]; }
	MOTION_INFO GetMotionInfo(int nNum, int nIdx) { return m_aMotionInfo[nNum][nIdx]; }
	D3DXMATRIX* GetMatrix(void) { return &m_mtxWorld; }
	bool IsFinish(int nNum) { return m_aFinish[nNum]; }
	void SetMatrix(void);
	float GetRadiusMax(int nNum);
	void SetAfterImage(int nNum, D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f }, int m_nLife = 10);
	int GetKey(int nNum) { return m_aKey[nNum]; }
	int GetFrame(int nNum) { return m_aCounterMotion[nNum]; }
	void SetAllCol(D3DXCOLOR col);
	void ResetAllCol(void);
	void InitPose(int nMotion);
	void EnableShadow(bool bShadow) { m_bShadow = bShadow; }
	void EnableIndependent(bool bInde) { m_bInde = bInde; }

private:
	Parts* m_apParts[DIV_BODY_MAX][MAX_PARTS];				// パーツの構造体
	MOTION_INFO m_aMotionInfo[DIV_BODY_MAX][MAX_MOTION];	// モーション情報の構造体
	KEY m_aKeyOld[DIV_BODY_MAX][MAX_PARTS];					// 前回のキー情報の構造体
	int m_aNumMotion[DIV_BODY_MAX];			// モーションの総数
	int m_aMotionType[DIV_BODY_MAX];		// モーションの種類
	int m_aMotionTypeOld[DIV_BODY_MAX];		// 前回のモーションの種類
	int m_aFrame[DIV_BODY_MAX];				// フレーム数
	bool m_aLoopMotion[DIV_BODY_MAX];		// ループするかどうか
	int m_aNumKey[DIV_BODY_MAX];			// キーの総数
	int m_aKey[DIV_BODY_MAX];				// 現在のキー
	int m_aCounterMotion[DIV_BODY_MAX];		// モーションカウンター
	int m_aNumParts[DIV_BODY_MAX];			// パーツの数
	bool m_aFinish[DIV_BODY_MAX];			// モーションが終わったかどうか

	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_posShadow;	// 影の位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;	//向き
	D3DXMATRIX m_mtxWorld;	// マトリックス
	bool m_bShadow;	// 影を描画するかどうか
	bool m_bInde;	// 分離しているかどうか
};

#endif