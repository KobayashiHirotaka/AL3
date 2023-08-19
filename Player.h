#pragma once
#include "Input.h"
#include "Matrix.h"
#include "ICharacter.h"

class Player : public ICharacter
{
public:
	
	void Initialize(const std::vector<Model*>& models)override;
	
	void Update()override;

	void Draw(const ViewProjection& viewProjection)override;

	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	void SetParent(const WorldTransform* parent);

	Vector3 GetWorldPosition();

	void FloatingGimmickInitialize();

	void FloatingGimmickUpdate();

private:
	const ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	const uint16_t kMaxModelParts = 2;

	XINPUT_STATE joyState_;

	float floatingParameter_ = 0.0f;
};
