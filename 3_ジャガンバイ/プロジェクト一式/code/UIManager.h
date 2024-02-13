//*****************************************************
//
// UIマネージャー[UIManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************

#include "object.h"
#include "weapon.h"

//*****************************************************
// 前方宣言
//*****************************************************

class CUI;
class CLife;
class CUIMagazine;

//*****************************************************
// 定数定義
//*****************************************************
namespace LIFE
{
	const float MAX_SIZE = 30.0f;
	const float WIDTH_MAX = 0.8f * MAX_SIZE;
	const float HEIGHT_MAX = 0.7f * MAX_SIZE;
}

//*****************************************************
// クラスの定義
//*****************************************************
class CUIManager : public CObject
{
public:

	CUIManager();	// コンストラクタ
	~CUIManager();	// デストラクタ

	static CUIManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUIManager *GetInstance(void) { return m_pUIManager; }
	bool IsDisp(void) { return m_bDisp; }
	void EnableDisp(bool bDisp) { m_bDisp = bDisp; }
	void SetPlayer(int nIdx);

private:

	void CreateFace(int nIdx);
	void CreateWeapon(int nIdx);
	void CreateLife(int nIdx);
	void CreateUIMagazine(int nIdx);

	void UpdateUIWeapon(void);
	void UpdateUISubColor(void);
	void UpdateUIChangeColor(void);

	struct SInfo
	{
		CWeapon::TYPE weaponType;	// 武器の種類
		bool bIsWeaponNull;			// 武器の有無

		float fUIColorAlpha;		// UIのアルファ値
		int nCntColorChange;		// 変更するまでのカウント

		CUI *pUiFace;				// 顔のUI			
		CUI *pUiWeapon;				// 武器のUI

		CLife *pLife;				// ライフUI
		CUIMagazine* pUIMagazine;	// 装弾数UI
	};

	static CUIManager *m_pUIManager;	// 自身のポインタ
	bool m_bDisp;				// UIを表示するかどうか
	SInfo m_aInfo[NUM_PLAYER];	// 自身の情報
};

#endif
