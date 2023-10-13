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
		NormalMAP,
		BossMAP,
		Max
	};
	struct UI {
		unique_ptr<IKESprite> sprite;
		XMFLOAT2 pos;
		XMFLOAT2 size;
	};



	std::list<UI> UIs;

};

