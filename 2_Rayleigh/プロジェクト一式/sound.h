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
		LABEL_BGM000 = 0,				// タイトルBGM
		LABEL_BGM_BATTLE,				// 戦闘BGM 
		LABEL_BGM_BOSS,				// ボスBGM 
		LABEL_SE_ATTACK,				// 通常攻撃 
		LABEL_SE_AIRATTACK,				// 二段ジャンプ攻撃 
		LABEL_SE_LASER,				// レーザー
		LABEL_SE_BULLET,				// 実弾射撃
		LABEL_SE_HIT_NORMAL,				// 通常ヒット
		LABEL_SE_HIT_BOSS,				// ボスヒット
		LABEL_SE_JUMP,				// ジャンプ
		LABEL_SE_DASH,				// ダッシュ
		LABEL_SE_LAND,				// 着地
		LABEL_SE_PARRY,				// パリィ
		LABEL_SE_EXPLOSION,				// 爆発
		LABEL_SE_MISSILE,				// ミサイル
		LABEL_SE_ENTER,				// エンター
		LABEL_SE_DAMAGE,				// ダメージ
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
		{ "data/SOUND/BGM/Control_Station.wav", -1 },			// タイトル	
		{ "data/SOUND/BGM/GENIX.wav", -1 },			// 戦闘BGM
		{ "data/SOUND/BGM/Virus_Entry.wav", -1 },			// ボスBGM
		{ "data/SOUND/SE/attack00.wav", 0 },		// 通常攻撃 
		{ "data/SOUND/SE/attack01.wav", 0 },			// 二段ジャンプ攻撃
		{ "data/SOUND/SE/shot00.wav", 0 },			// レーザー
		{ "data/SOUND/SE/shot01.wav", 0 },			// 実弾射撃
		{ "data/SOUND/SE/hit00.wav", 0 },			// 通常ヒット
		{ "data/SOUND/SE/hit01.wav", 0 },			// ボスヒット
		{ "data/SOUND/SE/jump.wav", 0 },			// ジャンプ
		{ "data/SOUND/SE/dash.wav", 0 },			// ダッシュ
		{ "data/SOUND/SE/land.wav", 0 },			// 着地
		{ "data/SOUND/SE/parry.wav", 0 },			// パリィ
		{ "data/SOUND/SE/explosion.wav", 0 },			// 爆発
		{ "data/SOUND/SE/missile.wav", 0 },			// ミサイル
		{ "data/SOUND/SE/enter.wav", 0 },			// エンター
		{ "data/SOUND/SE/damage.wav", 0 },			// ダメージ
	};

	static CSound *m_pSound;	// 自身のポインタ
};

#endif