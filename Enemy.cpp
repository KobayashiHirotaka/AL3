#include "Enemy.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "Player.h"
#include <cassert>

Enemy::~Enemy()
{
	for (EnemyBullet* bullet : bullets_)
	{
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_.x = 7.0f;
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 30.0f;

	ApproachPhaseInitialize();
}

void Enemy::Update()
{ 
	worldTransform_.TransferMatrix(); 

	Vector3 move = {0, 0, 0};

	switch (phase_)
	{
	case Phase::Approach:
	default:
		ApproachPhaseUpdate();
		for (EnemyBullet* bullet : bullets_)
		{
			bullet->Update();
		}

		break;

	case Phase::Leave:
		LeavePhaseUpdate();
		break;
	}
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) 
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);
}

Vector3 Enemy::GetWorldPosition() {
	// ���[���h���W������ϐ�
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::ApproachPhaseInitialize()
{ 
	shotTimer_ = 0;
}

void Enemy::ApproachPhaseUpdate()
{
	const float kApproachEnemySpeed = 0.2f;
	Vector3 move = {0, 0, 0};

	move.z -= kApproachEnemySpeed;
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	shotTimer_--;

	if (shotTimer_ < 0)
	{
		Fire();

		shotTimer_ = kFireInterval;
	}
}

void Enemy::LeavePhaseUpdate()
{
	const float kLeaveEnemyLPSpeed = 0.3f;
	Vector3 move = {0, 0, 0};

	move.x -= kLeaveEnemyLPSpeed;
	move.y += kLeaveEnemyLPSpeed;
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}
