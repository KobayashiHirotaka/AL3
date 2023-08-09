#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// modelチェック
	assert(model);
	// モデル読み込み
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");
	// プレイヤーの向きを玉にも反映
	velocity_ = velocity;
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 初期値セット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {

	// 玉の移動
	Move();

	worldTransformEx_.UpdateMatrix(
	    worldTransform_, worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 時間経過でデスフラグを立てる
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection viewProjection_) 
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

// private関数
void PlayerBullet::Move() {
	// 加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}
