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

	if (attackTimer_ <= 0 && worldTransform_.translation_.z>=-70)
	{
		moveCount = 2;
		worldTransform_.translation_.z -= 0.3f;
		if (worldTransform_.translation_.z <= -70)
		{
			worldTransform_.rotation_.y += 1.3f;
			moveCount = 0;
		}
	}
	

	ICharacter::Update();

	ImGui::Begin("Enemy");
	ImGui::Text("attackTimer %d", attackTimer_);
	ImGui::Text("WTX %f", worldTransform_.translation_.x);
	ImGui::Text("count %d", moveCount);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{ 
	ICharacter::Draw(viewProjection);
}

Vector3 Enemy::GetWorldPosition() 
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{

}