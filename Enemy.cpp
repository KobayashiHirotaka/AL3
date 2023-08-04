#include "Enemy.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "Player.h"
#include "GameScene.h"
#include <cassert>


Enemy::~Enemy()
{

}

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 position)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_.x = position.x;
	worldTransform_.translation_.y = position.y;
	worldTransform_.translation_.z = position.z;

	ApproachPhaseInitialize();

	isDead_ = false;
}

void Enemy::Update()
{ 
	worldTransform_.TransferMatrix(); 


	const float kApproachEnemySpeed = 0.05f;
	Vector3 move = {0, 0, 0};

	move.z -= kApproachEnemySpeed;

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	switch (phase_)
	{
	case Phase::Approach:
	default:
		ApproachPhaseUpdate();

		break;

	case Phase::Leave:
		LeavePhaseUpdate();
		break;
	}
	float inputFloat3[3] = {
	    worldTransform_.translation_.x + 1, worldTransform_.translation_.y + 1,
	    worldTransform_.translation_.z + 1};

	ImGui::Begin("Enemy");
	ImGui::SliderFloat3("EnemyTranslation", inputFloat3, -10.0f, 3.0f);
	ImGui::End();

	worldTransform_.translation_.x = inputFloat3[0] - 1;
	worldTransform_.translation_.y = inputFloat3[1] - 1;
	worldTransform_.translation_.z = inputFloat3[2] - 1;
}

void Enemy::Draw(ViewProjection viewProjection)
{
	if (isDead_ == false)
	{
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
}

void Enemy::Fire()
{
	assert(player_);

	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 自キャラのワールド座標を取得
	Vector3 playerWorldPos = player_->GetWorldPosition();
	// 敵のワールド座標を取得
	Vector3 enemyWorldPos = GetWorldPosition();
	// 敵キャラから自キャラへの差分ベクトルを求める
	Vector3 diffVector = Subtract(playerWorldPos, enemyWorldPos);

	diffVector = Normalize(diffVector);

	// ベクトルの長さを速さに合わせる
    velocity = VectorScale(diffVector, kBulletSpeed);

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	gameScene_->AddEnemyBullet(newBullet);
}

Vector3 Enemy::GetWorldPosition() 
{
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

void Enemy::OnCollision()
{ 
	isDead_ = true;
}
