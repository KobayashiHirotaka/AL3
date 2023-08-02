#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
	delete skydome_;
	delete skydomeModel_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	Vector3 playerPosition(0, 0, 15.0f);
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, playerPosition);

	enemy_ = new Enemy();
	enemy_->Initialize(model_, textureHandle_);

	debugCamera_ = new DebugCamera(50, 50);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize(railCamera_->GetWorldTransform().translation_,railCamera_->GetWorldTransform().rotation_);

	player_->SetParent(&railCamera_->GetWorldTransform());
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	if (enemy_ != nullptr)
	{
		enemy_->Update();
	}

	CheckAllCollisions();

	skydome_->Update();

	railCamera_->Updata();

	/*debugCamera_->Update();*/

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN))
	{
		isRailCameraActive_ = true;
	}

#endif

	/*if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}*/

	if (isRailCameraActive_)
	{
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw()
{

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


	if (enemy_ != nullptr)
	{
		enemy_->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);

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
	float EnemyRadius = 1.0f;
	float PlayerBulletRadius = 1.0f;
	float EnemyBulletRadius = 1.0f;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
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

	#pragma region 敵キャラと自弾の当たり判定
	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets)
	{
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		Vector3 distance = {
		    (posB.x - posA.x) * (posB.x - posA.x), 
			(posB.y - posA.y) * (posB.y - posA.y),
		    (posB.z - posA.z) * (posB.z - posA.z)};

		if (distance.x + distance.y + distance.z <= (EnemyRadius + PlayerBulletRadius) * (EnemyRadius + PlayerBulletRadius))
		{
			enemy_->OnCollision();

			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets)
	{
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemhyBullet : enemyBullets)
		{
			posB = enemhyBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			Vector3 distance = {
			    (posB.x - posA.x) * (posB.x - posA.x), 
				(posB.y - posA.y) * (posB.y - posA.y),
			    (posB.z - posA.z) * (posB.z - posA.z)};

			if (distance.x + distance.y + distance.z <=
			    (EnemyBulletRadius + PlayerBulletRadius) * (EnemyBulletRadius + PlayerBulletRadius)) {
				playerBullet->OnCollision();

				enemhyBullet->OnCollision();
			}
		}
	}
	#pragma endregion
}