#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Matrix.h"
#include "Matrix4x4.h"

class FollowCamera
{
public:
	void Initialize();

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }

private:
	ViewProjection viewProjection_;

	const WorldTransform* target_ = nullptr;

	Input* input_ = nullptr;
	XINPUT_STATE joyState_;
};
