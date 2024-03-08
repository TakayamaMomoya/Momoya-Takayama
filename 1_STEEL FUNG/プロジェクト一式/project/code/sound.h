//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 高山桃也
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:
	typedef enum
	{
		LABEL_BGM_TITLE = 0,				// タイトルBGM
		LABEL_BGM_GAME,					// ゲームBGM
		LABEL_BGM_BOSS00,					// ボスBGM０
		LABEL_SE_START_GAME,			// ゲームスタート

		LABEL_SE_HIT00,					// ヒット０
		LABEL_SE_HIT01,					// ヒット１
		LABEL_SE_SHOT00,					// 射撃０
		LABEL_SE_SHOT01,					// 射撃１
		LABEL_SE_SHOT02,					// 射撃２
		LABEL_SE_SHOT03,					// 射撃３
		LABEL_SE_SHOT04,					// 射撃４
		LABEL_SE_EXPLOSION00,					// 爆発０
		LABEL_SE_EXPLOSION01,					// 爆発１
		LABEL_SE_DASH00,					// ダッシュ０
		LABEL_SE_DASH01,					// ダッシュ１
		LABEL_SE_BOOST00,					// ブースト０
		LABEL_SE_WARNING00,					// 警告０
		LABEL_SE_LAND00,					// 着地０
		LABEL_SE_IMPACT00,					// 衝撃０
		LABEL_SE_IMPACT01,					// 衝撃１
		LABEL_SE_SLASH00,					// 斬撃０

		LABEL_SE_PAUSE_MENU,			// ポーズ
		LABEL_SE_PAUSE_ARROW,			// ポーズ移動
		LABEL_SE_PAUSE_ENTER,			// ポーズ選択
		LABEL_MAX
	} LABEL;
	typedef enum
	{
		FADESTATE_NONE = 0,	//何もしていない状態
		FADESTATE_IN,	//フェードイン状態
		FADESTATE_OUT,	//フェードアウト状態
		FADESTATE_MAX
	}FADESTATE;

	CSound();	// コンストラクタ
	~CSound();	// デストラクタ

	static CSound *Create(HWND hWnd);
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);

	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);

	void SetVolume(LABEL label, float fVolume);
	void SetFade(LABEL LabelNow, LABEL LabelNext, float fSpeed);

	static CSound *GetInstance(void) { return m_pSound; }

private:
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];	// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];	// オーディオデータサイズ
	FADESTATE m_fadeSound;	// フェード状態
	LABEL m_SoundLabelNow;	// 現在のサウンドラベル
	LABEL m_SoundLabelNext;	// 次のサウンドラベル
	float m_fSpeedFadeSound;	// サウンドがフェードするスピード
	float m_fVolumeSound;	// サウンドのボリューム

	// サウンドの情報
	SOUNDINFO m_aSoundInfo[LABEL_MAX] =
	{
		{ "data/SOUND/BGM/title.wav", -1 },				// タイトルBGM
		{ "data/SOUND/BGM/game.wav", -1 },				// ゲームBGM
		{ "data/SOUND/BGM/boss00.wav", -1 },				// ボスBGM０
		{ "data/SOUND/SE/enter.wav",0 },			// ゲームスタート

		{ "data/SOUND/SE/hit00.wav",0 },					// ヒット０
		{ "data/SOUND/SE/hit01.wav",0 },					// ヒット１
		{ "data/SOUND/SE/shot00.wav",0 },					// 射撃０
		{ "data/SOUND/SE/shot01.wav",0 },					// 射撃１
		{ "data/SOUND/SE/shot02.wav",0 },					// 射撃２
		{ "data/SOUND/SE/shot03.wav",0 },					// 射撃３
		{ "data/SOUND/SE/shot04.wav",0 },					// 射撃４
		{ "data/SOUND/SE/explosion00.wav",0 },					// 爆発０
		{ "data/SOUND/SE/explosion01.wav",0 },					// 爆発１
		{ "data/SOUND/SE/dash00.wav",0 },					// ダッシュ０
		{ "data/SOUND/SE/dash01.wav",0 },					// ダッシュ１
		{ "data/SOUND/SE/boost00.wav",0 },					// ブースト０
		{ "data/SOUND/SE/warning00.wav",-1 },					// 警告０
		{ "data/SOUND/SE/land00.wav",0 },					// 着地０
		{ "data/SOUND/SE/impact00.wav",0 },					// 衝撃０
		{ "data/SOUND/SE/impact01.wav",0 },					// 衝撃１
		{ "data/SOUND/SE/slash00.wav",0 },					// 斬撃０

		{ "data/SOUND/SE/pause.wav",0 },				// ポーズ
		{ "data/SOUND/SE/pause_arrow.wav",0 },			// 選択
		{ "data/SOUND/SE/pause_enter.wav",0 },			// 決定
	};

	static CSound *m_pSound;	// 自身のポインタ
};

namespace Sound
{
void Play(CSound::LABEL label);
}

#endif