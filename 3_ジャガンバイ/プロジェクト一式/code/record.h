//*****************************************************
//
// 戦績処理のヘッター[record.h]
// Author:酒井南勝
//
//*****************************************************

#ifndef _RECORD_H_
#define _RECORD_H_

//*****************************************************
// インクルード
//*****************************************************

#include "object.h"
#include "collision.h"
#include "number.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CRecord
{
public:

	enum GENRE_TYPE
	{
		GENRE_TYPE_DESTROY = 0,	// 破壊（敵を倒した数）
		GENRE_TYPE_MADMAN,		// 狂人（味方を殴った数）
		GENRE_TYPE_ENGINEER,	// 技術者（クリア時間）
		GENRE_TYPE_MAX
	};

	CRecord();	// コンストラクタ
	~CRecord();	// デストラクタ

	static CRecord* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void ReSetData(void);

	void AddDestroy(int nIdx);
	void CheckDeathEnemy(CObject* pObj, int nIdx);
	void CheckDeathEnemyAll(CCollision** ppCollsionMissile, D3DXVECTOR3 posMissile, float fRadiusMissile, int nIdx);

	void AddMedman(int nIdx);

	void AllSort(void);

	static CRecord* GetInstance(void) { return m_pRecord; }

	int GetNumSuvived(void) { return m_nNumSuvived; }

	int GetDestroy(int nIdx) { return m_aInfo[nIdx].nDestroy; }
	int GetDestroyRank(int nIdx) { return m_aInfo[nIdx].nDestroyRank; }

	int GetMadman(int nIdx) { return m_aInfo[nIdx].nMadman; }
	int GetMadmanRank(int nIdx) { return m_aInfo[nIdx].nMadmanRank; }

	float GetEngineer(void) { return m_fGameTime; }
	int GetEngineerRank(void) { return m_nTimeRank; }

private:

	void Debug(void);
	void SetNumPlayer(void);

	struct SInfo
	{
		int nDestroy;		// 敵の破壊数
		int nDestroyRank;	// 敵の破壊数の順位
		int nMadman;		// 味方を殴った数
		int nMadmanRank;	// 味方を殴った数の順位
	};

	static CRecord* m_pRecord;	// 自身のポインタ
	SInfo m_aInfo[NUM_PLAYER];	// 戦績情報
	int m_nNumSuvived;			// 生存者数
	float m_fGameTime;			// ゲームのプレイ時間
	int m_nTimeRank;	// タイムの順位
};

#endif
