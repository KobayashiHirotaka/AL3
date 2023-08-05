#include "Player.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include <cassert>

Player::~Player()
{
	for (PlayerBullet* bullet : bullets_)
    {
		delete bullet;
	}

	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform3DReticle_.Initialize();

	input_ = Input::GetInstance();

	uint32_t textureReticle = TextureManager::Load("reticle.png");

	ReticlePos_.x = 760;
	ReticlePos_.y = 320;

	sprite2DReticle_ =
	    Sprite::Create(textureReticle, ReticlePos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void Player::Update(ViewProjection viewProjection) 
{
	worldTransform_.TransferMatrix();

	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (Input::GetInstance()->GetJoystickState(0, joyState_)) 
	{
		move.x += (float)joyState_.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState_.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}
	
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

	float inputFloat3[3] = {move.x + 1, move.y + 1, move.z + 1};

	ImGui::Begin("Player");
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 2.0f);
	ImGui::Text("DebugCamera ENTER");
	ImGui::Text("Shot SPACE");
	ImGui::End();

	move.x = inputFloat3[0] - 1;
	move.y = inputFloat3[1] - 1;
	move.z = inputFloat3[2] - 1;

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.UpdateMatrix();

	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	const float kDistancePlayerTo3DRetocle = 50.0f;

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


	positionReticle_.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle_.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle_.z = worldTransform3DReticle_.matWorld_.m[3][2];


	if (Input::GetInstance()->GetJoystickState(0, joyState_)) 
	{
		ReticlePos_.x += (float)joyState_.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		ReticlePos_.y -= (float)joyState_.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		sprite2DReticle_->SetPosition(ReticlePos_);
	}

	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewport =  Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

	positionReticle_ = Transform(positionReticle_, matViewProjectionViewport);

	sprite2DReticle_->SetPosition(Vector2(positionReticle_.x, positionReticle_.y));


	GetMouse(viewProjection);

	Rotate();

	Attack();

	for (PlayerBullet* bullet : bullets_) 
	{
		bullet->Update();
	}

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// model_->Draw(worldTransform3DReticle_, viewProjection_);

	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}


void Player::DrawUI()
{ 
	sprite2DReticle_->Draw();
}

void Player::Rotate()
{
	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;

	} else if (input_->PushKey(DIK_D)) {

		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack()
{

	if (!Input::GetInstance()->GetJoystickState(0, joyState_)) 
	{
		return;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
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

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() 
{

}

void Player::SetParent(const WorldTransform* parent) 
{ 
	worldTransform_.parent_ = parent;
}

void Player::GetMouse(ViewProjection viewProjection) 
{
	POINT mousePosition;

	GetCursorPos(&mousePosition);

	HWND hwnd = WinApp::GetInstance()->GetHwnd();

	ScreenToClient(hwnd, &mousePosition);

	Vector2 Reticle;
	Reticle.x = float(ReticlePos_.x);
	Reticle.y = float(ReticlePos_.y);
	sprite2DReticle_->SetPosition(Reticle);

	Matrix4x4 matVPV = Multiply(
	    viewProjection.matView,
	    Multiply(
	        viewProjection.matProjection,
	        MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1)));

	Matrix4x4 matInveresVPV = Inverse(matVPV);

	Vector3 PosNear = Vector3(float(ReticlePos_.x), float(ReticlePos_.y), 0);
	Vector3 PosFar = Vector3(float(ReticlePos_.x), float(ReticlePos_.y), 1);
	
	PosNear = Transform(PosNear, matInveresVPV);
	PosFar = Transform(PosFar, matInveresVPV);

	Vector3 mouseDirection = Add(PosFar, PosNear);
	mouseDirection = Normalize(mouseDirection);

	const float kDistanceTestobject = 50.0f;
	worldTransform3DReticle_.translation_.x = PosNear.x + mouseDirection.x * kDistanceTestobject;
	worldTransform3DReticle_.translation_.y = PosNear.y + mouseDirection.y * kDistanceTestobject;
	worldTransform3DReticle_.translation_.z = PosNear.z + mouseDirection.z * kDistanceTestobject;

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();
}