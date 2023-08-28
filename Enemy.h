#pragma once
#include "ICharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Matrix.h"
#include "Player.h"

class Enemy : public ICharacter 
{
public:
	void Initialize(const std::vector<Model*>& models) override;

	void Update()override;

	void Draw(const ViewProjection& viewProjection) override;

	void OnCollision();

	Vector3 GetWorldPosition();

private:
	int attackTimer_ = 120;

	int moveCount = 0;

	Player* player_ = nullptr;
};
