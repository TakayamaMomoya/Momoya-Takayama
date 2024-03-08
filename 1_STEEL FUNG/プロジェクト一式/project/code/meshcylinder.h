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
// 定数定義
//*****************************************************
namespace meshCylinder
{
const float  MESH_RADIUS = 100.0f;	// 半径
const int MESH_U = 16;	// 横の分割数
const int MESH_V = 1;	// 縦の分割数
const int SPLIT_TEX_U = 3;	// 横のテクスチャ分割数
const int SPLIT_TEX_V = 1;	// 縦のテクスチャ分割数
const float MESH_HEIGHT = 50.0f;	// メッシュの高さ
}

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
		float fRadius;	// 半径
		float fHeight;	// 高さ
		int nMeshU;	// 横の分割数
		int nMeshV;	// 縦の分割数
		int nTexU;	// テクスチャの横の分割数
		int nTexV;	// テクスチャの縦の分割数
	}MeshCylinder;

	static CMeshCylinder *Create
	(
		int nMeshU = meshCylinder::MESH_U,
		int nMeshV = meshCylinder::MESH_V,
		int nTexU = meshCylinder::SPLIT_TEX_U,
		int nTexV = meshCylinder::SPLIT_TEX_V
	);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void JustDraw(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	void SetPosition(D3DXVECTOR3 pos) { m_meshCylinder.pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_meshCylinder.pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_meshCylinder.rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_meshCylinder.rot; }
	int GetNumVtx(void) { return m_meshCylinder.nNumVtx; }
	void SetRadius(float fRadius) { m_meshCylinder.fRadius = fRadius; }
	void SetHeight(float fHeight) { m_meshCylinder.fHeight = fHeight; }
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	void SetNumMeshU(int nNumMesh) { m_meshCylinder.nMeshU = nNumMesh; }
	void SetNumMeshV(int nNumMesh) { m_meshCylinder.nMeshV = nNumMesh; }
	MeshCylinder *GetMeshCylinder(void) { return &m_meshCylinder; }
	void SetCol(D3DXCOLOR col);
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetMtx(D3DXMATRIX mtx) { m_meshCylinder.mtxWorld = mtx; }
	void SetVtx(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファへのポインタ
	MeshCylinder m_meshCylinder;	//構造体の情報
	D3DXCOLOR m_col;	// 色
	int m_nIdxTexture;	// テクスチャ番号
};
#endif