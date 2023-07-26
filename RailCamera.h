#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Matrix4x4.h"
#include "Matrix.h"

class RailCamera
{
public:
	void Initialize(const Vector3& velocity);

	void Updata();

private:
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	Vector3 velocity_;
};
