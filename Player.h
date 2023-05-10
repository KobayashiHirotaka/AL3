#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// ���L����
/// </summary>

class Player {
public:
	/// <summary>
	/// ������
	///  </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	///  </summary>
	void Update();

	/// <summary>
	/// �`��
	///  </summary>
	void Draw(ViewProjection viewProjection);

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// ���f��
	Model* model = nullptr;

	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};
