#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Matrix.h"
#include "Matrix4x4.h"

class FollowCamera
{
public:
	void Initialize(const Vector3& position, const Vector3 rotation);

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	const WorldTransform* target_ = nullptr;
};
