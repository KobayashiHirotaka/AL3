#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(const Vector3& velocity) 
{
	// ワールドトランスフォームの初期設定
	Vector3 worldPos = worldTransform_.translation_;
	Vector3 rotationAngle = worldTransform_.rotation_;

	// ビュープロジェクションの初期化
	viewProjection_;
}

void RailCamera::Updata()
{
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.rotation_ = Add(worldTransform_.rotation_, velocity_);

	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");

}
