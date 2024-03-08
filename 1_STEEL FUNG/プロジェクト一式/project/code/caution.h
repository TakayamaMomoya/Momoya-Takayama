//*****************************************************
//
// 警告表示の処理[caution.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CAUTION_H_
#define _CAUTION_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CCaution	: public CObject
{
public:
	CCaution(int nPriority = 0);
	~CCaution();

	static CCaution *Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f });	// 生成処理
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }

private:
	enum STATE
	{
		STATE_NONE = 0,
		STATE_NORMAL,	// 通常状態
		STATE_FADEOUT,	// 徐々に消える状態
		STATE_MAX
	};
	struct SInfo
	{
		D3DXVECTOR3 pos;	// 警告の3D座標
		CUI *pIcon;	// アイコン
		CUI *pArrow;	// 矢印
		float fLife;	// 寿命
	};
	void UpdateIcon(void);

	SInfo m_info;
};

#endif