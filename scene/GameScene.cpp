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

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	playerModel_.reset(Model::CreateFromOBJ("player", true));

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(playerModel_.get());

	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));

	groundModel_.reset(Model::CreateFromOBJ("ground", true));

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());
}

void GameScene::Update() 
{
	// 自キャラの更新
	player_->Update();

	skydome_->Updata();

	ground_->Updata();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN))
	{
		isDebugActive_ = true;
	}
#endif 

	if (isDebugActive_)
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

	skydome_->Draw(viewProjection_);

	ground_->Draw(viewProjection_);

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