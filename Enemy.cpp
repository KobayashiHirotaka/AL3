#include "Enemy.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 30.0f;
}

void (Enemy::*Enemy::spFuncTable[])() = {
	&Enemy::ApproachPhaseUpdate, 
	&Enemy::LeavePhaseUpdate
};

void Enemy::Update()
{ 
	worldTransform_.TransferMatrix(); 

	Vector3 move = {0, 0, 0};

	/*switch (phase_)
	{
	case Phase::Approach:
	default:
		ApproachPhaseUpdate();
		break;

	case Phase::Leave:
		LeavePhaseUpdate();
		break;
	}*/

	static_cast<size_t>(phase_);

	(this->*spFuncTable[0])();

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	if (worldTransform_.translation_.z < 0.0f) {
		(this->*spFuncTable[1])();
	}

}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachPhaseUpdate()
{
	const float kApproachEnemySpeed = 0.2f;
	Vector3 move = {0, 0, 0};

	move.z -= kApproachEnemySpeed;
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
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
