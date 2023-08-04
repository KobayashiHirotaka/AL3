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

	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	void Update(ViewProjection viewProjection);

	void Rotate();

	void Attack();

	void Draw(ViewProjection viewProjection);

	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* parent);

	void DrawUI();

private:

	WorldTransform worldTransform_;
	
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	std::list<PlayerBullet*> bullets_;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	Vector2 ReticlePos_;
};
