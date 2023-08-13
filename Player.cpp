#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
}

void Player::Update() 
{ 
	worldTransform_.UpdataMatrix();

	ImGui::Begin("Player");
	ImGui::Text("DebugCamera ENTER");
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}