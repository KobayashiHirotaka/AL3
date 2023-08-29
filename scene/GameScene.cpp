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

	model_.reset(Model::Create());

	playerModel_.reset(Model::CreateFromOBJ("player", true));
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	/*modelFighterHammer_.reset(Model::CreateFromOBJ("hammer", true));*/

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

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

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
	if (input_->TriggerKey(DIK_1))
	{
		scene_ = Scene::play;
	}

	if (scene_ == Scene::play && input_->TriggerKey(DIK_2))
	{
		scene_ = Scene::clear;
	}

	if (scene_ == Scene::play && input_->TriggerKey(DIK_3))
	{
		scene_ = Scene::end;
	}

	switch (scene_)
	{ 
	case Scene::start:
	default:
		skydome_->Update();

		ground_->Update();
		break;

	case Scene::play:
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
		ImGui::Begin("END");
		ImGui::Text("Clear");
		ImGui::End();
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

	if (distance.x + distance.y + distance.z -5.0f <=
	    (PlayerRadius + EnemyRadius) * (PlayerRadius + EnemyRadius))
	{
		player_->OnCollision();
	}

	#pragma endregion
}