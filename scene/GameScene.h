#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
  public: // メンバ関数
	enum PartId {
		Root,  //大本
		Spine, //脊髄
		Chest, //胸
		Head,  //頭
		Arml,  //左腕
		ArmR,  //右腕
		Hip,   //尻
		LegL,  //左足
		LegR,  //右足
	};
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	uint32_t textureHundle_ = 0;   //テクスチャハンドル
	uint32_t soundDateHundle_ = 0; //サウンドハンドル
	uint32_t voiceHundle_ = 0;

	Model* model_ = nullptr;
	WorldTransform worldTransform_[9][9][9]; //ワールドトランスフォーム
	ViewProjection viewProjection_;
	int32_t value_ = 0;

	float viewAngel = 0.0f; //カメラ上方向の角度
	                        /// <summary>
	                        /// ゲームシーン用
	                        /// </summary>
};
