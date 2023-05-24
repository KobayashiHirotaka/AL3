#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"

class Enemy {
public:

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection viewProjection);

	void ApproachPhaseUpdate();

	void LeavePhaseUpdate();

	enum class Phase
	{
		Start,
		Approach,
		Leave,
	};


private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	Phase phase_ = Phase::Start;

	static void (Enemy::*spFuncTable[])();
};
