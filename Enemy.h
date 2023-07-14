#pragma once
#include "EnemyState.h"
#include "Matrix.h"
#include "Model.h"
#include "WorldTransform.h"

class IEnemyState;

class Enemy {
public:
	Enemy();

	~Enemy();

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void Move(Vector3 speed);

	void ChangePhase(IEnemyState* newState);

	Vector3 GetTranslation() { return worldTransform_.translation_; };

private:
	static void (Enemy::*phasetable_[])();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	IEnemyState* phase_ = nullptr;
};