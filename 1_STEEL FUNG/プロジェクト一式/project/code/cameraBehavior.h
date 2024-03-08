//*****************************************************
//
// カメラビヘイビア[cameraBehavior.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CAMERASTRATEGY_H_
#define _CAMERASTRATEGY_H_

//****************************************************
// 前方宣言
//****************************************************
class CCamera;

//****************************************************
// クラスの定義
//****************************************************
// 基底クラス
class CCameraBehavior
{
public:
	virtual void Update(CCamera *pCamera) = 0;

private:
};

// 出現カメラ
class CApperPlayer : public CCameraBehavior
{
public:
	void Update(CCamera *pCamera) override;

private:
};

// プレイヤー追従
class CFollowPlayer : public CCameraBehavior
{
public:
	void Update(CCamera *pCamera) override;

private:
};

// 敵を見る
class CLookEnemy : public CCameraBehavior
{
public:
	void Update(CCamera *pCamera) override;

private:

};

// 敵と近いときの動き
class CMoveCylinder : public CCameraBehavior
{
public:
	void Update(CCamera *pCamera) override;

private:

};

// 操作
class CMoveControl : public CCameraBehavior
{
public:
	void Update(CCamera *pCamera) override;

private:

};


#endif