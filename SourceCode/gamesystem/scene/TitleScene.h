#pragma once
#include "BaseScene.h"
#include "TextManager.h"
#include<memory>
#include"InterEnemy.h"
#include "IKESprite.h"
#include <array>
/// �^�C�g���V�[��
class TitleScene : public BaseScene {
public:
	~TitleScene() override {};
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
	unique_ptr<TextManager> text_;

	enum SceneType {
		PLAY,
		MAP,
		TUTORIAL
	}_SceneType = PLAY;

	unique_ptr<InterEnemy> enemy = nullptr;

	array<unique_ptr<IKESprite>, 2> title_;

	enum TitleParts {
		TITLE_BACK,
		TITLE_TEXT
	};
};