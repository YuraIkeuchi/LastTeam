#pragma once
#include "BaseScene.h"
#include"BossText.h"
#include "MessageWindow.h"
#include "EnemyManager.h"

//�Q�[���v���C�V�[��
class BattleScene : public BaseScene
{
public:
	~BattleScene() override;
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

	void SkipUpdate();
private:
	unique_ptr<IKESprite> skipUI = nullptr;
	float skip_alpha = 0.f;
	float scale_skip = 0.4f;
	unique_ptr<EnemyManager> enemyManager = nullptr;

	unique_ptr<IKESprite> ui = nullptr;
	unique_ptr<IKESprite> vignette = nullptr;

	enum ChangeType {
		CHANGE_MAP,
		CHANGE_OVER,
	}_ChangeType = CHANGE_MAP;

	XMFLOAT2 m_GameOverPos = { 0.0f,-1280.0f };
	float m_GameOverFrame = {};
	int m_ChangeTimer = {};
	bool m_Skip = false;
	bool m_IsBackKey = false;
	float m_Frame = {};
	bool m_SkipFeed = false;
	bool m_SkipEnd = false;
	bool m_BattleEnd = false;
};