#include "Player.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include <cassert>

Player::Player(){};
Player::~Player()
{
	delete sprite2DReticle_;
	for (PlayerBullet* bullet : bullets_)
 {
		delete bullet;
	}
}

void Player::Initialize(Model* model, Vector3 position) {
	assert(model);

	model_ = model;
	worldTransform_.translation_ = position;
	textureHandle_ = TextureManager::Load("sample.png");
	worldTransform3DReticle_.Initialize();
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	assert(model_);
	uint32_t textureReticle = TextureManager::Load("reticle.png");
	ReticlePos.x = 760;
	ReticlePos.y = 320;
	sprite2DReticle_ =
	    Sprite::Create(textureReticle, ReticlePos, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}
void Player::Attack() {

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {

		// 弾の速度

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		velocity.x = worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x;
		velocity.y = worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y;
		velocity.z = worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z;
		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
void Player::Update(ViewProjection viewProjection) {
	/*画像*/
	worldTransform_.TransferMatrix();
	/*操作キー*/
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;
	float inputFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	// コントローラー
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}
	// キーボード
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.UpdateMatrix();
	// 移動制限
	const float kMoveLimitX = 33;
	const float kMoveLimitY = 18;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	//

	const float kDistancePlayerTo3DRetocle = 50.0f;

	// 3Dレティクル
	Vector3 Pos;
	Pos.x = worldTransform_.matWorld_.m[3][0];
	Pos.y = worldTransform_.matWorld_.m[3][1];
	Pos.z = worldTransform_.matWorld_.m[3][2];
	Vector3 offset = {0, 0, 1.0f};
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Normalize(offset);
	offset.x *= kDistancePlayerTo3DRetocle;
	offset.y *= kDistancePlayerTo3DRetocle;
	offset.z *= kDistancePlayerTo3DRetocle;
	worldTransform3DReticle_.translation_.x = offset.x + Pos.x;
	worldTransform3DReticle_.translation_.y = offset.y + Pos.y;
	worldTransform3DReticle_.translation_.z = offset.z + Pos.z;
	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();

	// 2Dレティクル

	positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];

	// コントローラー
	// Vector2 spritePosition = sprite2DReticle_->GetPosition();

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		ReticlePos.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		ReticlePos.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		sprite2DReticle_->SetPosition(ReticlePos);
	}

	//
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//
	Matrix4x4 matViewProjectionViewport =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	GetMouse(viewProjection);

	/*弾*/
	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 回転
	const float kRotSpeed = 0.2f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {

		worldTransform_.rotation_.y += kRotSpeed;
	}
	// 速度
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	ImGui::Begin(" ");
	ImGui::SliderFloat3("Player", inputFloat3, -33.0f, 33.0f);
	ImGui::End();
}

void Player::GetMouse(ViewProjection viewProjection) {
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	Vector2 Reticle;
	Reticle.x = float(ReticlePos.x);
	Reticle.y = float(ReticlePos.y);
	sprite2DReticle_->SetPosition(Reticle);

	Matrix4x4 matVPV = Multiply(
	    viewProjection.matView,
	    Multiply(
	        viewProjection.matProjection,
	        MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1)));
	Matrix4x4 matInveresVPV = Inverse(matVPV);

	Vector3 PosNear = Vector3(float(ReticlePos.x), float(ReticlePos.y), 0);
	Vector3 PosFar = Vector3(float(ReticlePos.x), float(ReticlePos.y), 1);
	//
	PosNear = Transform(PosNear, matInveresVPV);
	PosFar = Transform(PosFar, matInveresVPV);
	// マウスレイ
	Vector3 mouseDirection = Add(PosFar, PosNear);
	mouseDirection = Normalize(mouseDirection);
	const float kDistanceTestobject = 50.0f;
	worldTransform3DReticle_.translation_.x = PosNear.x + mouseDirection.x * kDistanceTestobject;
	worldTransform3DReticle_.translation_.y = PosNear.y + mouseDirection.y * kDistanceTestobject;
	worldTransform3DReticle_.translation_.z = PosNear.z + mouseDirection.z * kDistanceTestobject;
	//
	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection_) {
	/*画像*/
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	// model_->Draw(worldTransform3DReticle_, viewProjection_);
	/*操作キー*/
	input_ = Input::GetInstance();
	/*弾*/
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}
void Player::DrawUI() { sprite2DReticle_->Draw(); }