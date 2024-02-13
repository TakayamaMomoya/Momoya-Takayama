//*****************************************************
//
// ビームの処理[beam.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BEAM_H_
#define _BEAM_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CAnim3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CBeam : public CObject
{
public:
	CBeam(int nPriority = 3);	// コンストラクタ
	~CBeam();	// デストラクタ

	static CBeam *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtx(D3DXMATRIX mtx) { m_info.mtxWorld = mtx; }
	void SetAnimSize(float fWidth, float fLength) { m_info.fWidthAnimDest = fWidth; m_info.fLengthAnimDest = fLength; }
	void SetExpand(float fSpeed) { m_info.fSpeedExpand = fSpeed; }	// 膨らむ速度設定
	void SetShrink(float fSpeed) { m_info.fSpeedShrink = fSpeed; }	// 縮む速度設定
	void SetExtend(float fSpeed) { m_info.fSpeedExtend = fSpeed; }	// 伸びる速度設定

private:
	struct SInfo
	{
		CAnim3D *pAnim;	// アニメーションエフェクト
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
		float fWidthAnim;	// アニメーションの幅
		float fLengthAnim;	// アニメーションの長さ
		float fWidthAnimDest;	// アニメーションの目標の幅
		float fLengthAnimDest;	// アニメーションの目標の長さ
		float fSpeedExpand;	// 膨らむ速度
		float fSpeedShrink;	// 縮む速度
		float fSpeedExtend;	// 伸びる速度
		bool bFinish;	// 膨らみきったかの判定
	};

	void ManageSize(void);
	void SetAnimVtx(void);

	SInfo m_info;
};

#endif
