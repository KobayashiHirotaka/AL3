#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& positon, const Vector3& velocity) {

	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	worldTransform_.translation_ = positon;

	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);

	float VelocityZ = sqrt((velocity.x * velocity.x) + (velocity.z * velocity.z));

	worldTransform_.rotation_.x = std::atan2(-velocity.y, VelocityZ);

	velocity_ = velocity;
}

void EnemyBullet::Update() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) 
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
