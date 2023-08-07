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

	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = 10.0f;
	worldTransform_.translation_.z = 100.0f;

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

	//弾の速度
	const float kBulletSpeed = 0.5f;

	// 自キャラのワールド座標を取得
	Vector3 playerWorldPos = player_->GetWorldPosition();
	// 敵のワールド座標を取得
	Vector3 enemyWorldPos = GetWorldPosition();

	Vector3 diffVector = Subtract(playerWorldPos, enemyWorldPos);

	diffVector = Normalize(diffVector);

	diffVector.x *= kBulletSpeed;
	diffVector.y *= kBulletSpeed;
	diffVector.z *= kBulletSpeed;

	EnemyBullet* newBullet = new EnemyBullet();

	newBullet->Initialize(model_, worldTransform_.translation_, diffVector);

	bullets_.push_back(newBullet);
}

Vector3 Enemy::GetWorldPosition() 
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::ApproachPhaseInitialize()
{ 
	shotTimer_ = 0;
}

void Enemy::ApproachPhaseUpdate()
{
	const float kApproachEnemySpeed = 0.05f;
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
