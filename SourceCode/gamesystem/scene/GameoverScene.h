#pragma once
#include "IKESprite.h"
#include "BaseScene.h"
/// �^�C�g���V�[��
class GameoverScene : public BaseScene {
public:
	~GameoverScene() override {};
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
	static const int ATTACH_MAX = 2;
	static const int OVER_MAX = 2;
private://�����o�ϐ�

	int m_Timer = {};
	//�p�l��
	struct GameOverAttach {
		unique_ptr<IKEObject3d> object = nullptr;
		unique_ptr<IKETexture> line = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
	};

	unique_ptr<IKESprite> gameover[OVER_MAX];
	unique_ptr<IKESprite> select[OVER_MAX];

	XMFLOAT2 m_OverPos[OVER_MAX];
	XMFLOAT2 m_SelectPos[OVER_MAX];
	XMFLOAT2 m_OverSize[OVER_MAX];
	GameOverAttach attach[ATTACH_MAX];
};

