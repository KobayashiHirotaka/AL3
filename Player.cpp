#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	worldTransformL_arm_.translation_.x = 1.5f;
	worldTransformL_arm_.translation_.y = 4.5f;

	worldTransformR_arm_.translation_.x = -1.5f;
	worldTransformR_arm_.translation_.y = 4.5f;

	SetParent(&GetWorldTransformBody());
	worldTransformBody_.parent_ = worldTransform_.parent_;

	FloatingGimmickInitialize();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformHammer_.Initialize();

	float s = 10;
	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->SetValue(groupName, "Test", s);
}

void Player::Update() 
{ 
	ICharacter::Update();

	if (!Input::GetInstance()->GetJoystickState(0, joyState_))
	{
		return;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) 
	{
		behaviorRequest_ = Behavior::kAttack;
	}

	if (behaviorRequest_)
	{
		behavior_ = behaviorRequest_.value();

		switch (behavior_) 
		{ 
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;

		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) 
	{ 
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;

	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::Text("Attack RightButtom");
	ImGui::Text("DebugCamera ENTER");
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

	if (behavior_ == Behavior::kAttack)
	{
		models_[kModelIndexHammer]->Draw(worldTransformHammer_, viewProjection);
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent)
{
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

	ImGui::Begin("Model");
	ImGui::DragFloat3("Head", &worldTransformHead_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmL", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmR", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::End();
}

void Player::BehaviorRootInitialize()
{
	worldTransformL_arm_.rotation_.x = 0.0f;
	worldTransformR_arm_.rotation_.x = 0.0f;
	worldTransformHammer_.rotation_.x = 0.0f;

	FloatingGimmickInitialize();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformHammer_.Initialize();
}

void Player::BehaviorRootUpdate()
{
	if (Input::GetInstance()->GetJoystickState(0, joyState_))
	{
		const float kPlayerSpeed = 0.2f;

		Vector3 move = {
		    (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState_.Gamepad.sThumbLY / SHRT_MAX};

		move = Multiply(kPlayerSpeed, Normalize(move));

		Matrix4x4 cameraRotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);

		move = TransformNormal(move, cameraRotateMatrix);

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransformBody_.translation_ = worldTransform_.translation_;

		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformBody_.rotation_.y = worldTransform_.rotation_.y;
	}
	
	FloatingGimmickUpdate();
}

void Player::BehaviorAttackInitialize() 
{ 
	worldTransformL_arm_.rotation_.x = (float)M_PI;
	worldTransformR_arm_.rotation_.x = (float)M_PI;
	worldTransformHammer_.rotation_.x = 0.0f;
	attackAnimationFrame = 0;
}

void Player::BehaviorAttackUpdate()
{
	if (attackAnimationFrame < 10)
	{
		worldTransformL_arm_.rotation_.x -= 0.05f;
		worldTransformR_arm_.rotation_.x -= 0.05f;

		worldTransformHammer_.rotation_.x -= 0.05f;

	} else if(worldTransformHammer_.rotation_.x <= 2.0f * (float)M_PI / 4) {
		worldTransformL_arm_.rotation_.x += 0.1f;
		worldTransformR_arm_.rotation_.x += 0.1f;
	
		worldTransformHammer_.rotation_.x += 0.1f;

	} else {
		behaviorRequest_ = Behavior::kRoot;
	}
	attackAnimationFrame++;
}