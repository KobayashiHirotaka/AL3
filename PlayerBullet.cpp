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
    worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

    if (--deathTimer_ <= 0)
    {
		isDead_ = true;
    }

    worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
    model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
