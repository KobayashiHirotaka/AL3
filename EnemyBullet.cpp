#include "EnemyBullet.h"
#include "ImGuiManager.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& positon, const Vector3& velocity) {

	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = positon;

	velocity_ = velocity;

	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(~kCollisionAttributeEnemy);
}

void EnemyBullet::Update() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("EBullet");
	ImGui::Text("isDead = %d", isDead_);
	ImGui::End();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	if (isDead_ == false)
	{
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
}

void EnemyBullet::OnCollision() 
{ 
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
