#pragma once
#include "BaseScene.h"
#include"BossText.h"
#include "MessageWindow.h"
#include "EnemyManager.h"

//�Q�[���v���C�V�[��
class BattleScene : public BaseScene
{
public:
	//������
	void Initialize(DirectXCommon* dxCommon) override;
	//�X�V
	void Update(DirectXCommon* dxCommon) override;
	//�`��
	void Draw(DirectXCommon* dxCommon) override;
	//�J��
	void Finalize() override;
	
private:
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw();
private:

	unique_ptr<EnemyManager> enemyManager = nullptr;

	unique_ptr<IKESprite> ui = nullptr;

	enum ChangeType {
		CHANGE_MAP,
		CHANGE_OVER,
	}_ChangeType = CHANGE_MAP;
};