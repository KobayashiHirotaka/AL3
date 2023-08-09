#include <WorldTransformEx.h>
WorldTransformEx::WorldTransformEx() {}

WorldTransformEx::~WorldTransformEx() {}

void WorldTransformEx::UpdateMatrix(
    WorldTransform& worldTransform, const Vector3& scale, const Vector3& rotate,
    const Vector3& translate) {
	worldTransform.matWorld_ = MakeAffineMatrix(scale, rotate, translate);
	worldTransform.TransferMatrix();
}