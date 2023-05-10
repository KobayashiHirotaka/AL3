#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 自キャラ
/// </summary>

class Player {
public:
	/// <summary>
	/// 初期化
	///  </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	///  </summary>
	void Update();

	/// <summary>
	/// 描画
	///  </summary>
	void Draw(ViewProjection viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
