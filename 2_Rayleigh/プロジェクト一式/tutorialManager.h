//*****************************************************
//
// チュートリアルマネージャー[tutorialManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _TUTORIALMANAGER_H_
#define _TUTORIALMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CAnim2D;

//*****************************************************
// クラスの定義
//*****************************************************
class CTutorialManager : public CObject
{
public:
	CTutorialManager();	// コンストラクタ
	~CTutorialManager();	// デストラクタ

	static CTutorialManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTutorialManager *GetInstance(void) { return m_pTutorialManager; }

private:
	void Load(void);

	static CTutorialManager *m_pTutorialManager;	// 自身のポインタ
};

#endif
