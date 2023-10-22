#pragma once
#include "BaseScene.h"
#include "TitleText.h"
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
	unique_ptr<TitleText> text_;
	enum SceneType {
		PLAY,
		MAP
	}_SceneType = PLAY;
};