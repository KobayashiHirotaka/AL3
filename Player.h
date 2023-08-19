#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix.h"

class Player
{
public:
	
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);
	
	void Update();

	void Draw(ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransformBase() { return worldTransformBase_; }
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	void SetParent(const WorldTransform* parent);

	Vector3 GetWorldPosition();

	void FloatingGimmickInitialize();

	void FloatingGimmickUpdate();

private:
	const ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	Model* modelBody_;
	Model* modelHead_;
	Model* modelL_arm_;
	Model* modelR_arm_;

	XINPUT_STATE joyState_;

	float floatingParameter_ = 0.0f;
};
