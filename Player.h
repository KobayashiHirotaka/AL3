#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix.h"

class Player
{
public:
	
	void Initialize(Model* model);
	
	void Update();

	void Draw(ViewProjection& viewProjection);

	const Vector3 GetWorldTransform();

private:
	
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;
};
