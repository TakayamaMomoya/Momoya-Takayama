//*****************************************************
//
// カメラの処理[camera.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CAMERA_H_
#define _CAMERA_H_

//****************************************************
// マクロ定義
//****************************************************
#define MIN_DRAW		(10.0f)					//描画を開始する距離
#define MAX_DRAW		(700000.0f)				//描画を終了する距離

//****************************************************
// クラスの定義
//****************************************************
class CCamera
{
public:
	//構造体宣言
	typedef struct
	{
		D3DXVECTOR3 posV;	// 視点
		D3DXVECTOR3 posVOld;	// 視点の前回の位置
		D3DXVECTOR3 posR;	// 注視点
		D3DXVECTOR3 posVDest;	// 目的の視点
		D3DXVECTOR3 posRDest;	// 目的の注視点
		D3DXVECTOR3 vecU;	// 上方向ベクトル
		D3DXVECTOR3 rot;	// 向き
		float fViewAngle;	// 視野角
		float fLength;	// 視点から注視点までの距離
		D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
		D3DXMATRIX mtxView;	// ビューマトリックス
		int nTimerQuake;	// 振動のタイマー
		float fQuakeSizeV;	// 振動のサイズ
		float fQuakeSizeH;	// 振動のサイズ
		float fTimeEvent;	// イベントのカウンター
	}Camera;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetDist(float fDist) { m_camera.fLength = fDist; }
	float GetDist(void) { return m_camera.fLength; }
	void Control(void);
	Camera *GetCamera(void);
	void FollowPlayer(void);
	void SetQuake(float fQuakeSizeH, float fQuakeSizeV, int nTime);
	void Quake(void);
	void UpdateTitle(D3DXVECTOR3 move);
	void UpdateTitleEsc(D3DXVECTOR3 move);
	void UpdateResult(void);
	void UpdateGameover(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest);
	void SetTitle(D3DXVECTOR3 posV, D3DXVECTOR3 posR);
	void SetTitleDest(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest);
	void SetSelect(void);
	void SetRanking(void);
	void SetLift(void);
	void SetUpLift(void);
	void MoveDist(float fFact);
	void SetPosRDest(D3DXVECTOR3 posRDest) { m_camera.posRDest = posRDest; }
	void SetPosVDest(D3DXVECTOR3 posVDest) { m_camera.posVDest = posVDest; }
	void SetEventTimer(float fTime) { m_camera.fTimeEvent = fTime; }

private:
	void SetPosV(void);
	void SetPosR(void);

	Camera m_camera;	// 構造体の情報
};

#endif