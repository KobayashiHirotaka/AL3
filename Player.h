#pragma once
#include"Model.h"
#include "WorldTransform.h"
#include"Input.h"
#include"Affine.h"
#include"ImGuiManager.h"
#include"WorldTransformEx.h"
#include"PlayerBullet.h"
#include<list>

class Player
{
public:
	Player();
	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Updete();
	void Draw(const ViewProjection viewProjection_);

private:
	void Move();
	void Rotate();
	void Attack();

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	WorldTransformEx worldTransformEx_;
};