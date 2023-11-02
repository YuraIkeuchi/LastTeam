#pragma once
#include "BaseScene.h"
#include "Font.h"
#include<memory>
#include"InterEnemy.h"

/// �^�C�g���V�[��
class TitleScene : public BaseScene {
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

private:
	std::array<unique_ptr<Font>,3> texts;

	enum SceneType {
		PLAY,
		MAP,
		TUTORIAL
	}_SceneType = PLAY;

	unique_ptr<InterEnemy> enemy = nullptr;
};