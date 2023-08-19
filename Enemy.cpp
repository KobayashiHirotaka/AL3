#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);
}

void Enemy::Update()
{
	const float kEnemySpeed = 0.3f;
	Vector3 velocity{0.0f, 0.0f, kEnemySpeed};

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
	worldTransform_.rotation_.y += 0.03f;

	ICharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{ 
	ICharacter::Draw(viewProjection);
}