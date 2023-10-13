#pragma once
#include"BaseActor.h"

class MapSceneActor :public BaseActor {
public:
	/// ������
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �I��
	void Finalize() override;
	/// ���t���[���X�V
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// �`��
	void Draw(DirectXCommon* dxCommon) override;
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
private:
	void IntroUpdate(DebugCamera* camera)override;		//�o��V�[��
	void MainUpdate(DebugCamera* camera)override;		//�o�g���V�[��
	void FinishUpdate(DebugCamera* camera)override;		//���j�V�[��

	void ImGuiDraw();
private:
	enum {
		Screen=0,
		StartMAP,
		NormalMAP,
		BossMAP,
		HealMAP,
		Max
	};
	struct UI {
		unique_ptr<IKESprite> sprite;
		XMFLOAT2 pos;
		XMFLOAT2 size;
		bool isPannel = false;
	};

	std::list<UI> UIs;
	XMFLOAT2 basePos = {340.f,360.f};
	XMFLOAT2 interbal = { 300.f,200.f };
	XMFLOAT2 scroll = { 0,0 };
};

