#pragma once
#include "IKESprite.h"
#include "BaseScene.h"
#include "EnemyManager.h"
/// �^�C�g���V�[��
class ClearScene : public BaseScene {
public:
	~ClearScene() override {};
public:
	/// ������
	void Initialize(DirectXCommon* dxCommon) override;
	/// �I��
	void Finalize() override;
	/// ���t���[���X�V
	void Update(DirectXCommon* dxCommon) override;
	/// �`��
	void Draw(DirectXCommon* dxCommon) override;

private:
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw(DirectXCommon* dxCommon);
private://�����o�ϐ�
	unique_ptr<IKESprite> sprite;
	struct ShineEffect {
		unique_ptr<IKESprite> tex;
		float frame = 0.f;
		float kFrame = 30.f;
		XMFLOAT2 position = { 0.f,0.f };
		XMFLOAT2 size = { 0.f,0.f };
		bool isVanish = false;
	};

	void RandShineInit();
	void ShineEffectUpdate();
	std::list<ShineEffect> shines;
	unique_ptr<EnemyManager> enemyManager = nullptr;
};

