//*****************************************************
//
// ３Dアニメーションエフェクト処理[animEffect3D.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ANIMEFFECT3D_H_
#define _ANIMEFFECT3D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "Object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CAnim3D;

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_STRING	(256)	// 最大文字数

//*****************************************************
// クラスの定義
//*****************************************************
class CAnimEffect3D : public CObject
{
public:
	enum TYPE
	{
		TYPE_MUZZLEFLUSH = 0,	// マズルフラッシュ
		TYPE_BLOOD,	// 血しぶき
		TYPE_BLOOD1,	// 血しぶき
		TYPE_BEAM,	// ビーム
		TYPE_EXPLOSION,	// 爆発
		TYPE_HIT00,	// 命中
		TYPE_BOOST,	// ブースト
		TYPE_PLASMA,	// プラズマ
		TYPE_BEAMFLASH,	// ビーム閃光
		TYPE_BIGEXPLOSION,	// でかい爆発
		TYPE_MAX
	};

	CAnimEffect3D(int nPriority = 4);	// コンストラクタ
	~CAnimEffect3D();	// デストラクタ

	static CAnimEffect3D *Create(void);
	CAnim3D *CreateEffect(D3DXVECTOR3 pos, TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CAnimEffect3D *GetInstance(void) { return m_pAnimEffect3D; }

private:
	struct SInfoAnimEffect
	{// アニメーションエフェクト情報
		char acPath[MAX_STRING];	// パス
		int nNumAnim;	// アニメーション数
		int nSpeedAnim;	// アニメーション速度
		float fSize;	// サイズ
		D3DXCOLOR col;	// 色
		bool bAdd;	// 加算合成するかどうか
		bool bLoop;	// ループするかどうか
		bool bBillboard;	// ビルボードにするかどうか
	};
	
	void Load(void);
	
	static CAnimEffect3D *m_pAnimEffect3D;	// 自身のポインタ
	SInfoAnimEffect *m_apAnimEffect[TYPE_MAX];	// 情報のポインタ
};

namespace Anim3D
{
CAnim3D *CreateAnim(D3DXVECTOR3 pos, CAnimEffect3D::TYPE type);
}

#endif