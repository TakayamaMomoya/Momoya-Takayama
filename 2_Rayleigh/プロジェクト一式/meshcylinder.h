//*****************************************************
//
// メッシュシリンダーの処理[meshcylinder.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラス定義
//*****************************************************
class CMeshCylinder : public CObject
{
public:
	CMeshCylinder(int nPriority = 5);	// コンストラクタ
	~CMeshCylinder();	// デストラクタ

	typedef struct
	{
		D3DXVECTOR3 pos;						//位置
		D3DXVECTOR3 rot;						//向き
		D3DXMATRIX mtxWorld;					//ワールドマトリックス
		int nNumIdx;							//インデックス数
		int nNumVtx;							//頂点数
	}MeshCylinder;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshCylinder *Create(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	void SetPosition(D3DXVECTOR3 pos) { m_meshCylinder.pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_meshCylinder.pos; }
	int GetNumVtx(void) { return m_meshCylinder.nNumVtx; }

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファへのポインタ
	MeshCylinder m_meshCylinder;	//構造体の情報
	D3DXCOLOR m_col;	// 色
};
#endif