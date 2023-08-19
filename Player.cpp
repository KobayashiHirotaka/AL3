#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	input_ = Input::GetInstance();
	worldTransform_.Initialize();
}

void Player::Update() 
{ 
	worldTransform_.TransferMatrix();

	if (Input::GetInstance()->GetJoystickState(0, joyState_))
	{
		const float kPlayerSpeed = 0.2f;

		Vector3 move = {(float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX};

		move = Multiply(kPlayerSpeed, Normalize(move));

		Matrix4x4 cameraRotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);

		move = TransformNormal(move, cameraRotateMatrix);

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::Text("DebugCamera ENTER");
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

