#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model)
{
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("sample.png");
	
	worldTransform_.Initialize();

	worldTransform_.translation_ = {5, 0, 40};

	state_ = new PhaseApproach();
}

void Enemy::Update() {
	state_->Update(this);

	worldTransformEx_.UpdateMatrix(
	    worldTransform_, worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}

void Enemy::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::PhaseChange(IEnemyState* newState) 
{
	delete state_;
	state_ = newState;
}

void Enemy::ApproachMove() 
{ 
	worldTransform_.translation_.z += kEnemySpeedZ_;
}

void Enemy::LeaveMove()
{ 
	worldTransform_.translation_.x += kEnemySpeedX_; 
	worldTransform_.translation_.y += kEnemySpeedY_;
}
