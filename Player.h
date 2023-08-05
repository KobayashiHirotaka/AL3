#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Vector3.h"
#include "Sprite.h"
#include <list>
#include "Matrix.h"

class Player {
public:
	
	Player();
	~Player();
	void Initialize(Model* model, Vector3 position);

	void Update(ViewProjection viewProjection);

	void Draw(ViewProjection viewProjection_);

	void Attack();
	void OnCollision();
	Vector3 GetWorldPosition();
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	float GetRadius() { return radius_; }
	void SetParent(const WorldTransform* parent);
	//
	void GetMouse(ViewProjection viewProjection);
	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

private:

		/*画像*/
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	/*操作キー*/
	Input* input_ = nullptr;
	/*弾*/
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	//
	const float radius_ = 2.0f;
	// 3Dレティクル　
	WorldTransform worldTransform3DReticle_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	Vector2 ReticlePos = {640, 320};
	Vector3 positionReticle;
	// コントローラー
	XINPUT_STATE joyState;
	
};
