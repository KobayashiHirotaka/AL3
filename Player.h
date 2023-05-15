#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"

class Player {
public:
	
	void Initialize(Model* model, uint32_t textureHandle);

	
	void Update();

	void Rotate();

	void Attack();

	void Draw(ViewProjection viewProjection);

private:
	
	WorldTransform worldTransform_;

	
	Model* model_ = nullptr;


	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	PlayerBullet* bullet_ = nullptr;
};
