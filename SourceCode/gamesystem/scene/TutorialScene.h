#pragma once
#include "BaseScene.h"
#include"BossText.h"
#include "MessageWindow.h"
#include "EnemyManager.h"

//�Q�[���v���C�V�[��
class TutorialScene : public BaseScene
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
	//void TutorialManager();//�`���[�g���A���̐i�s��

	static void (TutorialScene::* stateTable[])();
	void IntroState();
	void MoveState();	//����
	void GetState();	//�X�L���Q�b�g
	void AttackState();		//�U��
	void DamageState();//�_���[�W��^����
	void TutorialEnd();//�I���
	void Skip();		//�X�L�b�v

private:
	std::array<unique_ptr<Font>, 3> texts;
	unique_ptr<EnemyManager> enemyManager = nullptr;

	unique_ptr<IKESprite> ui = nullptr;

	enum ChangeType {
		CHANGE_TITLE,
		CHANGE_OVER,
	}_ChangeType = CHANGE_TITLE;

	int m_Timer = {};

	enum TutorialState {
		TUTORIAL_INTRO,
		TUTORIAL_MOVE,
		TUTORIAL_GETSKILL,
		TUTORIAL_ATTACK,
		TUTORIAL_DAMAGE,
		TUTORIAL_FINISH,
	}_nowstate = TUTORIAL_INTRO;

	bool m_End = false;
};