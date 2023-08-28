#include "Player.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	worldTransform_.translation_ = {0.0f, 0.0f, -30.0f};

	worldTransformL_arm_.translation_.x = 1.5f;
	worldTransformL_arm_.translation_.y = 4.5f;

	worldTransformR_arm_.translation_.x = -1.5f;
	worldTransformR_arm_.translation_.y = 4.5f;

	worldTransformHammer_.translation_.z = -1.5f;

	SetParent(&GetWorldTransformBody());
	worldTransformBody_.parent_ = worldTransform_.parent_;

	FloatingGimmickInitialize();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformHammer_.Initialize();

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_arm_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_arm_.translation_);
	globalVariables->AddItem(groupName, "floatingCycle_Arms", floatingCycle_[0]);
	globalVariables->AddItem(groupName, "floatingCycleBody", floatingCycle_[1]);
	globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
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
	ImGui::Text("hp %f",hp);
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

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent)
{
	worldTransformHead_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
	worldTransformHammer_.parent_ = parent;
}

void Player::FloatingGimmickInitialize()
{ 
	for (int i = 0; i < kMaxModelParts; i++)
	{
		floatingParameter_[i] = 0.0f;
	}
}

void Player::FloatingGimmickUpdate()
{
	floatingCycle_[0] = 120;
	floatingCycle_[1] = 120;
	
	float step[2]{};

	for (int i = 0; i < kMaxModelParts; i++) 
	{
		step[i] = 2.0f * (float)M_PI / floatingCycle_[i];
		
		floatingParameter_[i] += step[i];
	
		floatingParameter_[i] = (float)std::fmod(floatingParameter_[i], 2.0f * M_PI);
	}

	worldTransformBody_.translation_.y = std::sin(floatingParameter_[0]) * floatingAmplitude_;

	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_[1]) * 0.75f;
	worldTransformR_arm_.rotation_.x = -std::sin(floatingParameter_[1]) * 0.75f;

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
		const float kPlayerSpeed = 0.5f;

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

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	worldTransformHead_.translation_ =
	    globalVariables->GetVector3Value(groupName, "Head Translation");

	worldTransformL_arm_.translation_ =
	    globalVariables->GetVector3Value(groupName, "ArmL Translation");

	worldTransformR_arm_.translation_ =
	    globalVariables->GetVector3Value(groupName, "ArmR Translation");

	floatingCycle_[0] = globalVariables->GetIntValue(groupName, "floatingCycle_Arms");
	floatingCycle_[1] = globalVariables->GetIntValue(groupName, "floatingCycleBody");
	floatingAmplitude_ = globalVariables->GetFloatValue(groupName, "floatingAmplitude");
}

void Player::OnCollision()
{ 
	hp -= 0.01f;
}