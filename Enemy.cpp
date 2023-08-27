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

	if (moveCount == 0 && worldTransform_.translation_.x > -35)
	{
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
		if (worldTransform_.translation_.x <= -35)
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