#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Player.h"
#include "EnemyBullet.h"
#include <list>
#include "Collider.h"
#include "CollisionConfig.h"

class Player;

class Enemy : public Collider 
{
public:

	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Fire();

	void Draw(ViewProjection viewProjection);

	void ApproachPhaseInitialize();

	void ApproachPhaseUpdate();

	void LeavePhaseUpdate();

	enum class Phase
	{
		Start,
		Approach,
		Leave,
	};

	static const int kFireInterval = 60;

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	Phase phase_ = Phase::Start;

	std::list<EnemyBullet*> bullets_;

	int32_t shotTimer_ = 0;

	Player* player_ = nullptr;
};
