#pragma once
#include "Affine.h"
#include "WorldTransform.h"
class WorldTransformEx {
public:
	WorldTransformEx();
	~WorldTransformEx();

	void UpdateMatrix(
	    WorldTransform& worldTransform, const Vector3& scale, const Vector3& rotate,
	    const Vector3& translate);

private:
};