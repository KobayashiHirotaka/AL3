#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	//worldTransform_.rotation_.y = 182.5f;
	worldTransform_.translation_.x = 30.0f;
	worldTransform_.translation_.z = 30.0f;
}

void Enemy::Update()
{
	attackTimer_--;
	const float kEnemySpeed = -0.3f;
	Vector3 velocity{kEnemySpeed, 0.0f, 0.0f};

	//Vector3 playerWorldPos = player_->GetWorldPosition();
	//
	//GetWorldPosition();
	//
	//Vector3 diffVector;

	//diffVector = Subtract(playerWorldPos, worldPos);
	//// ベクトルの正規化
	//Vector3 normalizeDiffVector;

	//normalizeDiffVector = Normalize(diffVector);

	//// ベクトルの長さを速さに合わせる
	//velocity = VectorScale(normalizeDiffVector, kBulletSpeed);

	//velocity = TransformNormal(velocity, worldTransform_.matWorld_)

	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	if (moveCount == 0 && worldTransform_.translation_.x > 0)
	{
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
		if (worldTransform_.translation_.x <= 0)
		{
			moveCount = 1;
		}
	}

	if (moveCount == 1 && worldTransform_.translation_.x < 35)
	{
		worldTransform_.translation_ = Subtract(worldTransform_.translation_, velocity);
		if (worldTransform_.translation_.x >= 35) 
		{
			moveCount = 0;
		}
	}
	
	/*worldTransform_.rotation_.y += 0.03f;
	worldTransform_.translation_.z = 30.0f;*/


	/*if (attackTimer_ <= 0)
	{
		worldTransform_.rotation_.y = 0.0f;
		worldTransform_.translation_.x += 0.03f;
	}*/

	ICharacter::Update();

	ImGui::Begin("Enemy");
	ImGui::Text("attackTimer %d", attackTimer_);
	ImGui::Text("attackTimer %f", worldTransform_.translation_.x);
	ImGui::Text("attackTimer %d", moveCount);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{ 
	ICharacter::Draw(viewProjection);
}

Vector3 Enemy::GetWorldPosition() 
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}