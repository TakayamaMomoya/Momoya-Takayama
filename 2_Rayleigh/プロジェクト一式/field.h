//*****************************************************
//
// 地面の処理[field.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _FIELD_H_
#define _FIELD_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CField : public CObject
{
public:
	CField(); // コンストラクタ
	~CField();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CField *Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, float width = 3000.0f, float height = 3000.0f);

private:
	enum POLYGON
	{
		POLYGON_FRONT = 0,	// 前方
		POLYGON_RIGHT,	// 右
		POLYGON_LEFT,	// 右
		POLYGON_UNDER,	// 下
		POLYGON_MAX
	};

	CObject3D *m_pField;	// 床のポリゴン
	CObject3D *m_apPolygon[POLYGON_MAX];	// 他のポリゴン
};

#endif