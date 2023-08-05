#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Vector3.h"
#include "Sprite.h"
#include <list>

class Player {
public:
	
	~Player();

	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

	void Update(ViewProjection viewProjection);

	void Draw(ViewProjection viewProjection);

	void Rotate();

	void Attack();

	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* parent);
	
	void GetMouse(ViewProjection viewProjection);

	void DrawUI();

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	Vector2 ReticlePos_ = {640, 320};
	Vector3 positionReticle_;

	XINPUT_STATE joyState_;
	
};
