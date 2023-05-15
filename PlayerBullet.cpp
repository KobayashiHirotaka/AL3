#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& positon) {
   
    assert(model);

    model_ = model;
   
    textureHandle_ = TextureManager::Load("sample.png");

    worldTransform_.Initialize();
    
    worldTransform_.translation_ = positon;

}

void PlayerBullet::Update() { 
    worldTransform_.UpdateMatrix(); 

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
    model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
