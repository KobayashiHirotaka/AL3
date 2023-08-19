#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm)
{
	assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);

	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	worldTransformL_arm_.translation_.x = 1.5f;
	worldTransformL_arm_.translation_.y = 5.0f;

	worldTransformR_arm_.translation_.x = -1.5f;
	worldTransformR_arm_.translation_.y = 5.0f;

	SetParent(&GetWorldTransformBody());

	FloatingGimmickInitialize();

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
}

void Player::Update() 
{ 
	if (Input::GetInstance()->GetJoystickState(0, joyState_))
	{
		const float kPlayerSpeed = 0.2f;

		Vector3 move = {(float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX};

		move = Multiply(kPlayerSpeed, Normalize(move));

		Matrix4x4 cameraRotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);

		move = TransformNormal(move, cameraRotateMatrix);

		worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);
		worldTransformBody_.translation_ = worldTransformBase_.translation_;

		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformBody_.rotation_.y = worldTransformBase_.rotation_.y;
	}

	FloatingGimmickUpdate();

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	ImGui::Begin("DebugCamera");
	ImGui::Text("DebugCamera ENTER");
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection)
{
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent)
{
	worldTransformBase_.parent_ = parent;
	worldTransformHead_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
}

void Player::FloatingGimmickInitialize()
{
	floatingParameter_ = 0.0f;
}

void Player::FloatingGimmickUpdate()
{
	const uint16_t T = 120;

	const float step = 2.0f * (float)M_PI / T;

	floatingParameter_ += step;
	floatingParameter_ = (float)std::fmod(floatingParameter_, 2.0f * M_PI);

	const float floatingAmplitude = 1.0f;

	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude;

	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;

	ImGui::Begin("Player");
	ImGui::DragFloat3("Head", &worldTransformHead_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmL", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmR", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::End();
}
