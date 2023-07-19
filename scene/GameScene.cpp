#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	enemy_ = new Enemy();
	enemy_->Initialize(model_, textureHandle_);

	debugCamera_ = new DebugCamera(50, 50);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	if (enemy_ != nullptr)
	{
		enemy_->Update();
	}

	CheckAllCollisions();

	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN))
	{
		isDebugCameraActive_ = true;
	}
#endif

	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
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
	player_->Draw(viewProjection_);


	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
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

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{ 
	Vector3 posA, posB;
	float PlayerRadius = 1.0f;
	float EnemyBulletRadius = 1.0f;

	//const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets)
	{
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		Vector3 distance = {
		    (posB.x - posA.x) * (posB.x - posA.x),
			(posB.y - posA.y) * (posB.y - posA.y),
		    (posB.z - posA.z) * (posB.z - posA.z)};


		if (distance.x + distance.y + distance.z <= (PlayerRadius + EnemyBulletRadius) * (PlayerRadius + EnemyBulletRadius))
		{
			player_->OnCollision();

			bullet->OnCollision();
		}
	}


	#pragma endregion
}