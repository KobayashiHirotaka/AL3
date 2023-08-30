#pragma once
#include "ICharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Matrix.h"
#include "Player.h"

class Player;

class Enemy : public ICharacter 
{
public:
	void Initialize(const std::vector<Model*>& models) override;

	void Update()override;

	void Draw(const ViewProjection& viewProjection) override;

	void OnCollision();

	Vector3 GetWorldPosition();

	void SetWorldPosition(float enemyX, float enemyY, float enemyZ)
	{
		worldTransform_.translation_.x = enemyX;
		worldTransform_.translation_.y = enemyY;
		worldTransform_.translation_.z = enemyZ;
	}

	void SetRotationY(float rotationY) { worldTransform_.rotation_.y = rotationY; }

	void SetAttackTimer(int timer) { attackTimer_ = timer; }

	void SetMoveCount(int count) { moveCount = count; }

private:
	int attackTimer_;

	int moveCount;

	Player* player_ = nullptr;

	float hp = 3.0f;
};
