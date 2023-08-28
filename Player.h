#pragma once
#include "Input.h"
#include "Matrix.h"
#include "ICharacter.h"
#include "GlobalVariables.h"
#include <optional>

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

	void BehaviorRootInitialize();

	void BehaviorRootUpdate();

	void BehaviorAttackInitialize();

	void BehaviorAttackUpdate();

	void ApplyGlobalVariables();

	void OnCollision();

private:
	enum class Behavior
	{
		kRoot,
		kAttack
	};

	const ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	const uint16_t kMaxModelParts = 2;

	XINPUT_STATE joyState_;

	float floatingParameter_[2];

	int floatingCycle_[2];

	float floatingAmplitude_;

	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	int attackAnimationFrame;

	WorldTransform worldTransformHammer_;

	float hp = 3.0f;
};
