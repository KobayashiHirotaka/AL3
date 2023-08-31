#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene()
{

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("playerHP.png");
	
	playerHpSprite_[0].reset(Sprite::Create(textureHandle_, {1080, 620}));
	playerHpSprite_[1].reset(Sprite::Create(textureHandle_, {1140, 620}));
	playerHpSprite_[2].reset(Sprite::Create(textureHandle_, {1200, 620}));

	textureHandle_ = TextureManager::Load("title.png");
	sceneTitleSprite_.reset(Sprite::Create(textureHandle_, {0, 0}));

	textureHandle_ = TextureManager::Load("Clear.png");
	sceneClearSprite_.reset(Sprite::Create(textureHandle_, {0, 0}));

	textureHandle_ = TextureManager::Load("GameOver.png");
	sceneGameOverSprite_.reset(Sprite::Create(textureHandle_, {0, 0}));

	textureHandle_ = TextureManager::Load("0.png");
	numberSprite_[0].reset(Sprite::Create(textureHandle_, {1200, 20}));

	textureHandle_ = TextureManager::Load("1.png");
	numberSprite_[1].reset(Sprite::Create(textureHandle_, {1160, 20}));

	textureHandle_ = TextureManager::Load("2.png");
	numberSprite_[2].reset(Sprite::Create(textureHandle_, {1160, 20}));

	textureHandle_ = TextureManager::Load("3.png");
	numberSprite_[3].reset(Sprite::Create(textureHandle_, {1160, 20}));

	soundDataHandle_ = audio_->LoadWave("loop100201.wav");
	voiceHandle_ = audio_->PlayWave(soundDataHandle_,true);

	model_.reset(Model::Create());

	playerModel_.reset(Model::CreateFromOBJ("player", true));
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelFighterHammer_.reset(Model::CreateFromOBJ("hammer", true));

	enemyModel_.reset(Model::CreateFromOBJ("Enemy", true));

	std::vector<Model*> playerModels = { modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get(), modelFighterHammer_.get()};

	std::vector<Model*> enemyModels = {enemyModel_.get()};

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(playerModels);

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels);

	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	//AxisIndicator::GetInstance()->SetVisible(true);
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));

	groundModel_.reset(Model::CreateFromOBJ("ground", true));

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GameScene::Update() 
{
	if (!Input::GetInstance()->GetJoystickState(0, joyState_)) 
	{
		return;
	}

	if (scene_ == Scene::start && joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		scene_ = Scene::play;
	}

	if (scene_ == Scene::play && stageTimer_ <= 0)
	{
		scene_ = Scene::clear;
	}

	if (scene_ == Scene::clear && joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		scene_ = Scene::start;
	}

	if (scene_ == Scene::play && hpCount_ == 0)
	{
		scene_ = Scene::end;
	}

	if (scene_ == Scene::end && joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		scene_ = Scene::start;
	}

	switch (scene_)
	{ 
	case Scene::start:
	default:
		skydome_->Update();

		ground_->Update();

		stageTimer_ = 1800;

	    lifeTimer_ = 90;
		hpCount_ = 3;
		isHit_ = 0;

		player_->SetWorldPosition(0.0f, 0.0f, -30.0f);

		enemy_->SetWorldPosition(30.0f, 0.0f, 30.0f);
		enemy_->SetRotationY(0.0f);
		enemy_->SetAttackTimer(180);
		enemy_->SetMoveCount(0);

		break;

	case Scene::play:
		stageTimer_--;
		skydome_->Update();

		ground_->Update();
		// 自キャラの更新
		player_->Update();

		enemy_->Update();

		viewProjection_.UpdateMatrix();

		CheckAllCollisions();

		break;

	case Scene::clear:
		break;

	case Scene::end:
		break;
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN))
	{
		isDebugCameraActive_ = true;
	} else if (input_->TriggerKey(DIK_RETURN) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}
#endif 

	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
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
	
	switch (scene_)
	{
	case Scene::start:
	default:
		skydome_->Draw(viewProjection_);

		ground_->Draw(viewProjection_);
		break;

	case Scene::play:
		skydome_->Draw(viewProjection_);

		ground_->Draw(viewProjection_);

		player_->Draw(viewProjection_);

		enemy_->Draw(viewProjection_);
		break;

	case Scene::clear:
		skydome_->Draw(viewProjection_);

		ground_->Draw(viewProjection_);
		break;

	case Scene::end:
		skydome_->Draw(viewProjection_);

		ground_->Draw(viewProjection_);
		break;
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

	switch (scene_) {
	case Scene::start:
	default:
		sceneTitleSprite_->Draw();
		break;

	case Scene::play:
		for (int i = 0; i < 3; i++)
		{
			if (hpCount_ == 3)
			{
				playerHpSprite_[i]->Draw();
			}

			if (hpCount_ == 2) 
			{
				playerHpSprite_[0]->Draw();
				playerHpSprite_[1]->Draw();
			}

			if (hpCount_ == 1) 
			{
				playerHpSprite_[0]->Draw();
			}
		}

		
		if (stageTimer_ <= 1800 && stageTimer_ > 1200) 
		{
			numberSprite_[3]->Draw();
			numberSprite_[0]->Draw();
		}
	
		if (stageTimer_ <= 1200 && stageTimer_ > 600)
		{
			numberSprite_[2]->Draw();
			numberSprite_[0]->Draw();
		}

		if (stageTimer_ <= 600 && stageTimer_ > 0)
		{
			numberSprite_[1]->Draw();
			numberSprite_[0]->Draw();
		}

		break;

	case Scene::clear:
		sceneClearSprite_->Draw();
		break;

	case Scene::end:
		sceneGameOverSprite_->Draw();
		break;
	}
	

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	Vector3 posA, posB;
	float PlayerRadius = 3.0f;
	float EnemyRadius = 10.0f;

	#pragma region 自キャラと敵キャラ

	posA = player_->GetWorldPosition();
	posB = enemy_->GetWorldPosition();

	Vector3 distance = {
	    (posB.x - posA.x) * (posB.x - posA.x), (posB.y - posA.y) * (posB.y - posA.y),
	    (posB.z - posA.z) * (posB.z - posA.z)};

	if (isHit_ == 1)
	{
		lifeTimer_--;
	}

	if (distance.x + distance.y + distance.z  <=
	    (PlayerRadius + EnemyRadius) * (PlayerRadius + EnemyRadius))
	{
		isHit_ = 1;
		if (isHit_ == 1 && lifeTimer_ >= 90)
		{
			player_->OnCollision();
			hpCount_ -= 1;
		}

		if (lifeTimer_ <= 0)
		{
			lifeTimer_ = 90;
			isHit_ = 0;
		}

	} else if (lifeTimer_ <= 0) {
		lifeTimer_ = 90;
		isHit_ = 0;
	}

	#pragma endregion
}