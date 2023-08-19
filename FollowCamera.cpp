#include "FollowCamera.h"
#include "ImGuiManager.h"

void FollowCamera::Initialize() 
{
	viewProjection_.Initialize();

	input_ = Input::GetInstance();
}

void FollowCamera::Update() 
{
	//追従対象がいれば
	if (target_)
	{
		//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 5.0f, -35.0f};

		Matrix4x4 cameraRotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

		offset = TransformNormal(offset, cameraRotateMatrix);

		viewProjection_.translation_ = Add(target_->translation_, offset);
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState_))
	{
		const float kRotateSpeed = 0.02f;

		viewProjection_.rotation_.y += (float)joyState_.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
	}

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}
