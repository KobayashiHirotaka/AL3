#include "Enemy.h"
#include <cassert>

Enemy::Enemy() {}

Enemy::~Enemy() { delete phase_; }

void Enemy::Initialize(Model* model) {
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");

	// フェーズ開始
	phase_ = new EnemyApproach();

	worldTransform_.Initialize();

	worldTransform_.translation_ = {0, 2, 20};
}

void Enemy::Update() {
	phase_->Update(this);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Enemy::ChangePhase(EnemyState* newState) {
	delete phase_;
	phase_ = newState;
}

void Enemy::Move(Vector3 speed) { worldTransform_.translation_ += speed; };

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}