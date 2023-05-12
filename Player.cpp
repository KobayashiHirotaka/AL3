#include "Player.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();
}

void Player::Update() { 
	worldTransform_.TransferMatrix();

	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	float inputFloat3[3] = {move.x + 1, move.y + 1, move.z + 1};

	ImGui::Begin("Player");
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 2.0f);
	ImGui::Text("Player %d %d %d", 2050, 12, 31);
	ImGui::End();

	move.x = inputFloat3[0] - 1;
	move.y = inputFloat3[1] - 1;
	move.z = inputFloat3[2] - 1;

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	const float kMoveLimitX = 50;
	const float kMoveLimitY = 1250;

	move.x = max(move.x, -kMoveLimitX);
	move.x = min(move.x, +kMoveLimitX);
	move.y = max(move.y, -kMoveLimitY);
	move.y = min(move.y, +kMoveLimitY);


}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}