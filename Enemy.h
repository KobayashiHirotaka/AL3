#pragma once
#include "ImGuiManager.h"
#include "Model.h"
#include "IEnemyState.h"
#include "PhaseApproach.h"
#include "PhaseLeave.h"
#include "WorldTransform.h"
#include "WorldTransformEx.h"

class Enemy {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection viewProjection);

	Vector3 GetTransform() { return worldTransform_.translation_; }

	void ApproachMove();
	void LeaveMove();

	void PhaseChange(IEnemyState* newState);

private:

	const float kEnemySpeedX_ = -0.3f;
	const float kEnemySpeedY_ = 0.3f;
	const float kEnemySpeedZ_ = -0.5f;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	WorldTransformEx worldTransformEx_;
	IEnemyState* state_;
};