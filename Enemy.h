#pragma once
#include "ICharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Matrix.h"

class Enemy : public ICharacter 
{
public:
	void Initialize(const std::vector<Model*>& models) override;

	void Update()override;

	void Draw(const ViewProjection& viewProjection) override;
};
