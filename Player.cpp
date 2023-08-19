#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	input_ = Input::GetInstance();
}

void Player::Update() 
{ 
	/*worldTransform_.TransferMatrix();*/

	if (Input::GetInstance()->GetJoystickState(0, joyState_))
	{
		const float kPlayerSpeed = 0.2f;

		Vector3 move = {(float)joyState_.Gamepad.sThumbLX, 0.0f, (float)joyState_.Gamepad.sThumbLY};

		move = Multiply(kPlayerSpeed, Normalize(move));

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransform_.matWorld_ = MakeAffineMatrix(
		    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();

	ImGui::Begin("Player");
	ImGui::Text("DebugCamera ENTER");
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

