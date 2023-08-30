#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	worldTransform_.translation_.x = 30.0f;
	worldTransform_.translation_.z = 30.0f;

	attackTimer_ = 180;

	moveCount = 0;
}

void Enemy::Update()
{
	attackTimer_--;

	if (moveCount == 0)
	{
		worldTransform_.translation_.x -= 0.5f;
		if (worldTransform_.translation_.x <= -35)
		{
			moveCount = 1;
		}

		if (attackTimer_ <= 0 && worldTransform_.translation_.z > -70)
		{
			moveCount = 2;
		}

		if (attackTimer_ <= 0 && worldTransform_.translation_.z < -70) 
		{
			moveCount = 3;
		}
	}

	if (moveCount == 1)
	{
		worldTransform_.translation_.x += 0.5f;
		if (worldTransform_.translation_.x >= 35) 
		{
			moveCount = 0;
		}

		if (attackTimer_ <= 0 && worldTransform_.translation_.z > -70)
		{
			moveCount = 2;
		}

		if (attackTimer_ <= 0 && worldTransform_.translation_.z < -70)
		{
			moveCount = 3;
		}
	}

	if (moveCount == 2)
	{
		worldTransform_.translation_.z -= 0.8f;
		worldTransform_.rotation_.y += 0.2f;

		if (worldTransform_.translation_.z <= -70) 
		{
			worldTransform_.rotation_.y = 3.2f;
			moveCount = 0;
			attackTimer_ = rand() % 180;
		}
	}

	if (moveCount == 3)
	{
		worldTransform_.translation_.z += 0.8f;
		worldTransform_.rotation_.y -= 0.2f;

		if (worldTransform_.translation_.z >= 30)
		{
			worldTransform_.rotation_.y = 6.4f;
			moveCount = 0;
			attackTimer_ = 120;
		}
	}


	ICharacter::Update();

	/*ImGui::Begin("Enemy");
	ImGui::Text("attackTimer %d", attackTimer_);
	ImGui::Text("WTX %f", worldTransform_.translation_.z);
	ImGui::Text("count %d", moveCount);
	ImGui::Text("hp %f", hp);
	ImGui::End();*/
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
	worldPos.z = worldTransform_.translation_.z + 9.0f;

	return worldPos;
}

void Enemy::OnCollision()
{ 
	hp -= 0.01f;
}