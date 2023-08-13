#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
}

void Ground::Updata()
{ 
	worldTransform_.UpdataMatrix();
}

void Ground::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
