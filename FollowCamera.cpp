#include "FollowCamera.h"
#include "ImGuiManager.h"

void FollowCamera::Initialize(const Vector3& position, const Vector3 rotation) 
{
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	viewProjection_.Initialize();
}

void FollowCamera::Update() 
{
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    {1.0f, 1.0f, 1.0f}, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//追従対象がいれば
	if (target_)
	{
		//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		viewProjection_.translation_ = Add(target_->translation_, offset);
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();

	ImGui::Begin("Rail Camera");
	ImGui::DragFloat3("Camera Position", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Camera Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();

}
