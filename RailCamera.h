#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

class RailCamera
{
public:
	void Initialize();

	void Updata();

private:
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;
};
