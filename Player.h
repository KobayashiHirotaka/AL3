#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Vector3.h"
#include <List>

class Player {
public:
	
	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Rotate();

	void Attack();

	void Draw(ViewProjection viewProjection);

	Vector3 GetWorldPosition();


private:
	
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	std::list<PlayerBullet*> bullets_;
};
