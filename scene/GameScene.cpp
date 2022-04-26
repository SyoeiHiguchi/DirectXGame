#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene()
{ 
	
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHundle_ = TextureManager::Load("mario.jpg");
	soundDateHundle_ = audio_->LoadWave("se_sad03.wav");
	model_ = Model::Create();

	std::random_device seed_gen;                                 //乱数シード生成器
	std::mt19937_64 engin(seed_gen());                             //メルセンヌ・ツイスター
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI); //乱数範囲（回転軸用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0); //乱数範囲（座標用）

	//for (size_t i = 0; i < _countof(worldTransform_); i++) 
	//{
	//	worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f}; // x,y,z軸周りのスケーリング角を設定
	//	worldTransform_[i].rotation_ = {
	//	  rotDist(engin), rotDist(engin), rotDist(engin)}; // x,y,z軸周りの回転角を設定
	//	worldTransform_[i].translation_ = {
	//	  posDist(engin), posDist(engin), posDist(engin)}; ////x,y,z軸周りの平行移動を設定
	//	worldTransform_[i].Initialize()

	//}
	worldTransform_[PartId::Root].Initialize();      //キャラクターの大本
	worldTransform_[PartId::Spine].translation_ = {0, 4.5f, 0};//脊髄
	worldTransform_[PartId::Spine].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::Spine].Initialize();
	//上半身
	worldTransform_[PartId::Chest].translation_ = {0,3.0f,0}; //胸
	worldTransform_[PartId::Chest].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Chest].Initialize();
	worldTransform_[PartId::Head].translation_ = {0, 3.0f, 0}; //頭
	worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::Head].Initialize();
	worldTransform_[PartId::Arml].translation_ = {3.0f, 0, 0}; //左腕
	worldTransform_[PartId::Arml].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::Arml].Initialize();
	worldTransform_[PartId::ArmR].translation_ = {-3.0f, 0, 0}; //右腕
	worldTransform_[PartId::ArmR].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmR].Initialize();
	//下半身
	worldTransform_[PartId::Hip].translation_ = {0, 0, 0}; //尻
	worldTransform_[PartId::Hip].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Hip].Initialize();
	worldTransform_[PartId::LegL].translation_ = {3.0f, -3.0f, 0}; //左足
	worldTransform_[PartId::LegL].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegL].Initialize();
	worldTransform_[PartId::LegR].translation_ = {-3.0f, -3.0f, 0}; //右足
	worldTransform_[PartId::LegR].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegR].Initialize();
	
	viewProjection_.eye;//カメラ視点座標を設定
	viewProjection_.target;//カメラ注視点を設定
	viewProjection_.up;
	viewProjection_.fovAngleY = XMConvertToRadians(45.0f);//カメラ垂直方向視野角を設定
	viewProjection_.aspectRatio = 1.0f;//アスペクト比の設定
	viewProjection_.nearZ = 0.1f;//ニアクリップ距離を設定
	//viewProjection_.farZ = 53.0f;//ファークリップ距離を設定
	viewProjection_.Initialize();//ビュープロダクションの初期化
	
}

void GameScene::Update() 
{
	//視点移動処理
	{
	//	XMFLOAT3 move = {0, 0, 0};    //視点の移動ベクトル
	//	const float kEyeSpeed = 0.2f; //視点移動の速さ
	//	if (input_->PushKey(DIK_W)) { //押した方向で移動ベクトルを変更
	//		move = {0, 0, kEyeSpeed};
	//	} else if (input_->PushKey(DIK_S)) {
	//		move = {0, 0, -kEyeSpeed};
	//	}
	//	viewProjection_.eye.x += move.x; //視点移動（ベクトルの移動）
	//	viewProjection_.eye.y += move.y;
	//	viewProjection_.eye.z += move.z;
	//	viewProjection_.UpdateMatrix(); //行列の再計算
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}
	//注視点移動処理
	{
	//	XMFLOAT3 move = {0, 0, 0};       //注視点の移動ベクトル
	//	const float kTargetSpeed = 0.2f; //注視点の移動速度
	//	if (input_->PushKey(DIK_LEFT)) { //押した方向で移動ベクトルを変更
	//		move = {-kTargetSpeed, 0, 0};
	//	} else if (input_->PushKey(DIK_RIGHT)) {
	//		move = {kTargetSpeed, 0, 0};
	//	}
	//	viewProjection_.target.x += move.x; //注視点移動（ベクトルの加算）
	//	viewProjection_.target.y += move.y;
	//	viewProjection_.target.z += move.z;
	//	viewProjection_.UpdateMatrix(); //行列の再計算
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
		  viewProjection_.target.z);
	}
	//上方向の回転
	{ 
	//	const float kUpRotSpeed = 0.05f;//上方向の回転の速さ[ラジアン/fream]
	//	if (input_->PushKey(DIK_SPACE)) { //押した方向で移動ベクトルを変更
	//		viewAngel += kUpRotSpeed;
	//		viewAngel = fmodf(viewAngel, XM_2PI);//2πを超えたら0に戻す
	//	}
	//	viewProjection_.up = {cosf(viewAngel), sinf(viewAngel), 0.0f};//上方向ベクトルを計算(半径１の円周上の座標)
	//	viewProjection_.UpdateMatrix(); //行列の再計算
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	}
	
	//FoV変更処理
	{ 
		//if (input_->PushKey(DIK_W)) {//上キーで視野角が広がる
		//	viewProjection_.fovAngleY += 0.01f;
		//	viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
		//} else if (input_->PushKey(DIK_S))//下キーで視野角が狭まる
		//{
		//	viewProjection_.fovAngleY -= 0.01f;
		//	viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		//}
		//viewProjection_.UpdateMatrix(); //行列の再計算
		debugText_->SetPos(50, 110);//デバック用表示
		debugText_->Printf(
		"fovAngeleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));
	}
	//クリップ距離変更設定
	{
		//if (input_->PushKey(DIK_UP)) { //上下キーでニアクリップ処理を増減
		//	viewProjection_.nearZ += 0.1f;
		//} else if (input_->PushKey(DIK_DOWN)) //下キーで視野角が狭まる
		//{
		//	viewProjection_.nearZ -= 0.1f;
		//}
		//viewProjection_.UpdateMatrix(); //行列の再計算
		debugText_->SetPos(50, 130);    //デバック用表示
		debugText_->Printf("nearZ:%f",viewProjection_.nearZ);
	}
	//キャラクター移動処理
	{
		XMFLOAT3 move = {0, 0, 0};//キャラクター移動ベクトル
		const float kCharacterSpeed = 0.2f;//キャラクターの移動の速さ
		if (input_->PushKey(DIK_LEFT)) {//押した方向でベクトルを変更
			move = {-kCharacterSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0, 0};
		}
		// 注視点移動(ベクトル加算)
		worldTransform_[PartId::Root].translation_.x += move.x;
		worldTransform_[PartId::Root].translation_.y += move.y;
		worldTransform_[PartId::Root].translation_.z += move.z;
		//デバッグ用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
		  "Root:(%f,%f,%f)", worldTransform_[PartId::Root].translation_.x,
		  worldTransform_[PartId::Root].translation_.y,
		  worldTransform_[PartId::Root].translation_.z);

		worldTransform_[PartId::Root].UpdateMatrix();
		worldTransform_[PartId::Spine].UpdateMatrix();
		worldTransform_[PartId::Chest].UpdateMatrix();
		worldTransform_[PartId::Head].UpdateMatrix();
		worldTransform_[PartId::Arml].UpdateMatrix();
		worldTransform_[PartId::ArmR].UpdateMatrix();
		worldTransform_[PartId::Hip].UpdateMatrix();
		worldTransform_[PartId::LegL].UpdateMatrix();
		worldTransform_[PartId::LegR].UpdateMatrix();
	}
	//上半身回転
	{	
		const float kChestRotSpeed = 0.05f;//上半身の回転速さ[ラジアン/フレーム]
		if (input_->PushKey(DIK_U)) {//押した方向で移動ベクトルを変更
			worldTransform_[PartId::Chest].rotation_.y -= kChestRotSpeed;
		} else if (input_->PushKey(DIK_I)) {
			worldTransform_[PartId::Chest].rotation_.y += kChestRotSpeed;
		}
	}
	//下半身回転処理
	{ 
		const float kHipRotSpeed = 0.05f;
		if (input_->PushKey(DIK_J)) { //押した方向で移動ベクトルを変更
			worldTransform_[PartId::Hip].rotation_.y -= kHipRotSpeed;
		} else if (input_->PushKey(DIK_K)) {
			worldTransform_[PartId::Hip].rotation_.y += kHipRotSpeed;
		}

	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/* for (size_t i = 0; i < _countof(worldTransform_); i++) 
	{
		model_->Draw(worldTransform_[i], viewProjection_, textureHundle_);
	}*/
	//model_->Draw(worldTransform_[PartId::Root], viewProjection_, textureHundle_);
	//model_->Draw(worldTransform_[PartId::Spine], viewProjection_, textureHundle_);
	model_->Draw(worldTransform_[PartId::Chest], viewProjection_, textureHundle_);
	model_->Draw(worldTransform_[PartId::Head], viewProjection_, textureHundle_);
	model_->Draw(worldTransform_[PartId::Arml], viewProjection_, textureHundle_);
	model_->Draw(worldTransform_[PartId::ArmR], viewProjection_, textureHundle_);
	model_->Draw(worldTransform_[PartId::Hip], viewProjection_, textureHundle_);
	model_->Draw(worldTransform_[PartId::LegL], viewProjection_, textureHundle_);
	model_->Draw(worldTransform_[PartId::LegR], viewProjection_, textureHundle_);
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
