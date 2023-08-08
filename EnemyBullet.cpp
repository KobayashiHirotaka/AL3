#include "EnemyBullet.h"
#include "Player.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& positon, const Vector3& velocity) {

	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	velocity_ = velocity;

	worldTransform_.translation_ = positon;
}

void EnemyBullet::Update()
{
	Homing();
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) 
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::Homing()
{
	toPlayer_ = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);
	toPlayer_ = Normalize(toPlayer_);

	velocity_ = Normalize(velocity_);
	velocity_ = VectorSLerp(velocity_, toPlayer_, 0.1f);
	velocity_.x *= 0.5f;
	velocity_.y *= 0.5f;
	velocity_.z *= 0.5f;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float VelocityZ = sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, VelocityZ);
}
