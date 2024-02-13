//*****************************************************
//
// 選択処理[select.h]
// Author:小笠原彪
//
//*****************************************************
#ifndef _SELECT_H_
#define _SELECT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CBillboard;
class CObject2D;
class CPlayerManager;
class CPlayer;
class CContainer;
class CLift;
class CNumber3D;
class CObjectX;

//*****************************************************
// クラスの定義
//*****************************************************
class CSelect : public CScene
{
public:
	CSelect();	// コンストラクタ
	~CSelect();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update();
	void Draw();

private:
	enum SELECT_STATE
	{
		STATE_BEFORE = 0,
		STATE_GO,
		STATE_MAX
	};
	// フェードの種類(UI点滅用)
	enum FADE_STATE
	{
		FADE_NONE = 0,	// 無し
		FADE_IN,	// フェードイン
		FADE_OUT,	// フェードアウト
		FADE_MAX
	};
	// プレイヤー状態の種類
	enum PLAYER_STATE
	{
		PLAYER_NONE = 0,	// 無し
		PLAYER_ENTRY,	// 参加状態
		PLAYER_FREE,	// 自由状態
		PLAYER_INGAME,	// ゲームへ入る
		PLAYER_MAX
	};
	// メニューの構造体
	struct SJoinUi_info
	{
		CBillboard* pUi2D;	//メニュー
		D3DXCOLOR col;	// 色
		FADE_STATE state;	// 状態
	};
	// プレイヤー情報の構造体
	struct PlayerInfo
	{
		CPlayer* pPlayer;	// プレイヤー
		CObjectX* pLeaf;	// 葉っぱ
		PLAYER_STATE state;	// 状態
	};
	// コンテナ情報の構造体
	struct CContainerInfo
	{
		CContainer* pContainer;
		float fReSpawnTimer;
	};

	void JoinUiInit(void);	// メニューの初期設定
	void ObjDelete(int nPlayer);	// 削除
	void MenuColorChange(int nPlayer);	// メニュー色の変更
	void StartInit(void);	// スタートの初期設定
	void PotatoLeafInit(void);	// ポテトの葉の初期設定
	void ContainerInit(void);	// コンテナの初期設定
	void ReSetContainer(void);	// コンテナの再設置
	void EntryInput(int nPlayer);	// 参加の入力
	void MoveLimit(int nPlayer);	// 行動制限
	void PlayerShowUp(int nPlayer);	// プレイヤーの登場

	void Lift(void);	// リフトの設定

	void LiftInNumberUi(int nPlayer);	// リフトに入った人数表示

	SJoinUi_info m_aJoinUiData[NUM_PLAYER];	// 参加UI
	CObject2D* m_pStartUI;	// Start文字
	PlayerInfo m_apPlayerData[NUM_PLAYER];	// プレイヤー情報
	CContainerInfo m_aContainerData[NUM_PLAYER];	// コンテナ
	CLift* m_pLift;	// リフト
	CNumber3D* m_apNumber[2];	// 何人リフトに乗ってるかのテクスチャ
	CBillboard* m_pSlash;	// スラッシュ
	SELECT_STATE m_selectState;	// 人数選択の状態
	bool m_abEntry[NUM_PLAYER];	// 参加したかどうか
	bool m_bLiftCamera;	// リフトカメラの切り替え
	bool m_bLiftUp;	// リフトが上昇するかどうか
	bool m_bSound;	// サウンドを再生するかどうか
};

#endif