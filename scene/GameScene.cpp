#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <fstream>
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model_;
	delete player_;
	delete debugCamera_;
	delete skydome_;
	delete skydomeModel_;
	delete railCamera_;

	for (Enemy* enemy : enemys_) 
	{
		delete enemy;
	}

	for (EnemyBullet* bullet : enemyBullets_)
	{
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	player_ = new Player();
	Vector3 playerPosition(0, 0, 30);
	player_->Initialize(model_, textureHandle_, playerPosition);

	LoadEnemyPopData();

	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});

	player_->SetParent(&railCamera_->GetWorldTransform());

	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
	UpdateEnemyPopCommands();

	for (Enemy* enemy : enemys_)
	{
		enemy->Update();
	}

	enemys_.remove_if([](Enemy* enemy)
	{
		if (enemy->IsDead())
		{
			delete enemy;
			return true;
		}
		return false;
	});
	
	for (EnemyBullet* bullet : enemyBullets_) 
	{
		bullet->Update();
	}

	CheckAllCollisions();

	skydome_->Update();

	#ifdef _DEBUG

	if (input_->TriggerKey(DIK_RETURN) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	} else if (input_->TriggerKey(DIK_RETURN) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}

	if (isDebugCameraActive_ == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
#endif
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


	for (Enemy* enemy : enemys_) 
	{
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : enemyBullets_)
	{
		bullet->Draw(viewProjection_);
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



void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;
	float PlayerRadius = 1.0f;
	float EnemyRadius = 1.0f;
	float PlayerBulletRadius = 1.0f;
	float EnemyBulletRadius = 1.0f;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets_) {
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		Vector3 distance = {
		    (posB.x - posA.x) * (posB.x - posA.x), (posB.y - posA.y) * (posB.y - posA.y),
		    (posB.z - posA.z) * (posB.z - posA.z)};

		if (distance.x + distance.y + distance.z <=
		    (PlayerRadius + EnemyBulletRadius) * (PlayerRadius + EnemyBulletRadius)) {
			player_->OnCollision();

			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 敵キャラと自弾の当たり判定
	for (Enemy* enemy : enemys_) {
		posA = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {
			posB = bullet->GetWorldPosition();

			// 座標AとBの距離を求める
			Vector3 distance = {
			    (posB.x - posA.x) * (posB.x - posA.x), (posB.y - posA.y) * (posB.y - posA.y),
			    (posB.z - posA.z) * (posB.z - posA.z)};

			if (distance.x + distance.y + distance.z <=
			    (EnemyRadius + PlayerBulletRadius) * (EnemyRadius + PlayerBulletRadius)) {
				enemy->OnCollision();

				bullet->OnCollision();
			}
		}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
		for (PlayerBullet* playerBullet : playerBullets) {
			posA = playerBullet->GetWorldPosition();
			for (EnemyBullet* enemhyBullet : enemyBullets_) {
				posB = enemhyBullet->GetWorldPosition();

				// 座標AとBの距離を求める
				Vector3 distance = {
				    (posB.x - posA.x) * (posB.x - posA.x), (posB.y - posA.y) * (posB.y - posA.y),
				    (posB.z - posA.z) * (posB.z - posA.z)};

				if (distance.x + distance.y + distance.z <=
				    (EnemyBulletRadius + PlayerBulletRadius) *
				        (EnemyBulletRadius + PlayerBulletRadius)) {
					playerBullet->OnCollision();

					enemhyBullet->OnCollision();
				}
			}
		}
#pragma endregion
	}
}

void GameScene::AddEnemyBullet(EnemyBullet * enemyBullet) 
{ 
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	//待機処理
	if (isStay_)
	{
		watingTimer_--;
		if (watingTimer_ <= 0)
		{
			//待機完了
			isStay_ = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0)
		{
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			enemy_ = new Enemy();
			enemys_.push_back(enemy_);
			enemy_->Initialize(model_, textureHandle_,Vector3(x,y,z));
			enemy_->SetGameScene(this);
			// 敵キャラに自キャラのアドレスを渡す
			enemy_->SetPlayer(player_);

		}else if (word.find("WAIT") == 0)
		{
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isStay_ = true;
			watingTimer_ = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}