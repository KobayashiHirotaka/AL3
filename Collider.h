#pragma once
#include "Vector3.h"

class Collider
{
public:
	//半径を取得
	float GetRadius() const { return radius_; }

	//半径を設定
	void SetRadius(float& radius) { radius_ = radius; }

	virtual void OnCollision() = 0;

	virtual Vector3 GetWorldPosition() = 0;

	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }

	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	uint32_t GetCollisionMask() const { return collisionMask_; }

	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private:
	float radius_ = 1.0f;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};
