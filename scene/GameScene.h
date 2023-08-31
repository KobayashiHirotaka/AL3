#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Enemy.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "FollowCamera.h"
#include"Skydome.h"
#include "Ground.h"
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_= 0;
	std::unique_ptr<Sprite> playerHpSprite_[3];

	std::unique_ptr<Sprite> sceneTitleSprite_;
	std::unique_ptr<Sprite> sceneClearSprite_;
	std::unique_ptr<Sprite> sceneGameOverSprite_;

	std::unique_ptr<Sprite> numberSprite_[4];

	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isDebugCameraActive_ = false;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Model> groundModel_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;
	std::unique_ptr<Model> modelFighterHammer_;

	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Model> enemyModel_;

	std::unique_ptr<GameScene> gameScene_;

	enum class Scene
	{ 
		start, 
		play,
		clear,
		end
	};

	Scene scene_ = Scene::start;

	XINPUT_STATE joyState_;

	int lifeTimer_;
	int hpCount_;
	bool isHit_;

	int stageTimer_;

	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;
};