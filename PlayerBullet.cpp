#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& positon, const Vector3& velocity) {
   
    assert(model);

    model_ = model;
   
    textureHandle_ = TextureManager::Load("sample.png");

    worldTransform_.Initialize();
    
   worldTransform_.translation_ = positon;

    velocity_ = velocity;
}

void PlayerBullet::Update() { 
      worldTransform_.UpdateMatrix(); 
    worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

    if (--deathTimer_ <= 0)
    {
		isDead_ = true;
    }

  
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
    model_->Draw(worldTransform_, viewProjection);

}

void PlayerBullet::OnCollision()
{ 
    isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() 
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
    
	return worldPos;
}
