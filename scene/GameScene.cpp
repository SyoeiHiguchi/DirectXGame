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

	for (size_t i = 0; i < _countof(worldTransform_); i++) 
	{
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f}; // x,y,z軸周りのスケーリング角を設定
		worldTransform_[i].rotation_ = {
		  rotDist(engin), rotDist(engin), rotDist(engin)}; // x,y,z軸周りの回転角を設定
		worldTransform_[i].translation_ = {
		  posDist(engin), posDist(engin), posDist(engin)}; ////x,y,z軸周りの平行移動を設定
		worldTransform_[i].Initialize(); //ワールドトランスフォームの初期化
	}

	
	viewProjection_.eye = {0, 0, -50};//カメラ視点座標を設定
	viewProjection_.target = {10, 0, 0};//カメラ注視点を設定
	viewProjection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f};
	viewProjection_.Initialize();//ビュープロダクションの初期化
	
}

void GameScene::Update() 
{
	//視点移動処理
	{
		XMFLOAT3 move = {0, 0, 0};    //視点の移動ベクトル
		const float kEyeSpeed = 0.2f; //視点移動の速さ
		if (input_->PushKey(DIK_W)) { //押した方向で移動ベクトルを変更
			move = {0, 0, kEyeSpeed};
		} else if (input_->PushKey(DIK_S)) {
			move = {0, 0, -kEyeSpeed};
		}
		viewProjection_.eye.x += move.x; //視点移動（ベクトルの移動）
		viewProjection_.eye.y += move.y;
		viewProjection_.eye.z += move.z;
		viewProjection_.UpdateMatrix(); //行列の再計算
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}
	//注視点移動処理
	{
		XMFLOAT3 move = {0, 0, 0};       //注視点の移動ベクトル
		const float kTargetSpeed = 0.2f; //注視点の移動速度
		if (input_->PushKey(DIK_LEFT)) { //押した方向で移動ベクトルを変更
			move = {-kTargetSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kTargetSpeed, 0, 0};
		}
		viewProjection_.target.x += move.x; //注視点移動（ベクトルの加算）
		viewProjection_.target.y += move.y;
		viewProjection_.target.z += move.z;
		viewProjection_.UpdateMatrix(); //行列の再計算
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
		  viewProjection_.target.z);
	}
	//上方向の回転
	{ 
		const float kUpRotSpeed = 0.05f;//上方向の回転の速さ[ラジアン/fream]
		if (input_->PushKey(DIK_SPACE)) { //押した方向で移動ベクトルを変更
			viewAngel += kUpRotSpeed;
			viewAngel = fmodf(viewAngel, XM_2PI);//2πを超えたら0に戻す
		}
		viewProjection_.up = {cosf(viewAngel), sinf(viewAngel), 0.0f};//上方向ベクトルを計算(半径１の円周上の座標)
		viewProjection_.UpdateMatrix(); //行列の再計算
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
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
	 for (size_t i = 0; i < _countof(worldTransform_); i++) 
	{
		model_->Draw(worldTransform_[i], viewProjection_, textureHundle_);
	}
	
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
