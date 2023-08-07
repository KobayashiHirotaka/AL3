#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Vector3.h"
#include <list>
#include "Collider.h"

class Player : public Collider
{
public:
	
	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Rotate();

	void Attack();

	void Draw(ViewProjection viewProjection);

	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

private:

	WorldTransform worldTransform_;
	
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	std::list<PlayerBullet*> bullets_;
};
