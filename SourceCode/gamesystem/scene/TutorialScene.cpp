#include "TutorialScene.h"
#include <Player.h>
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <SceneManager.h>
#include <GameStateManager.h>
#include <SceneChanger.h>
#include "SkillManager.h"
#include <TutorialTask.h>
#include <Helper.h>
#include "BattleScene.h"
#include "TutorialEnemy.h"

//状態遷移
/*stateの並び順に合わせる*/
void (TutorialScene::* TutorialScene::stateTable[])() = {
	&TutorialScene::IntroState,//
	&TutorialScene::MoveState,//
	&TutorialScene::GetState,//スキルゲット
	&TutorialScene::AttackState,//攻撃
	&TutorialScene::DamageState,//ダメージを与えた
	&TutorialScene::TutorialEnd,//終わり
};
//初期化
void TutorialScene::Initialize(DirectXCommon* dxCommon)
{
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	//ポストエフェクト
	PlayPostEffect = false;

	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	//プレイヤー
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ -8.0f,1.0f,0.0f });
	Player::GetInstance()->Initialize();
	//スキル
	SkillManager::GetInstance()->Initialize();
	//ゲームの状態
	GameStateManager::GetInstance()->Initialize();
	//ステージの床
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();
	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::TUTORIAL_START);
	//敵
	enemy = make_unique<TutorialEnemy>();
	enemy->Initialize();

	_nowstate = TUTORIAL_INTRO;

}
//更新
void TutorialScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	// 全オブジェクト更新
	game_object_manager_->Update();

	//各クラス更新
	camerawork->Update(camera);
	lightGroup->Update();
	Player::GetInstance()->Update();
	StagePanel::GetInstance()->Update();
	GameStateManager::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemy->Update();
	//敵を倒したらシーン以降(仮)
	if (m_End) {
		_ChangeType = CHANGE_TITLE;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}
	//ぷれいやーのHPが無くなっても遷移する
	if (Player::GetInstance()->GetHp() <= 0.0f) {
		_ChangeType = CHANGE_OVER;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);
		SceneManager::GetInstance()->ChangeScene<BattleScene>();
		/*if (_ChangeType == CHANGE_TITLE) {
			SceneManager::GetInstance()->PopScene();
		}
		else {
			
		}*/
		SceneChanger::GetInstance()->SetChange(false);
	}

	//状態移行(stateに合わせる)
	(this->*stateTable[static_cast<size_t>(_nowstate)])();
}

void TutorialScene::Draw(DirectXCommon* dxCommon) {
	//描画方法
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw();
		dxCommon->PostDraw();
	}
}
//ポストエフェクトかからない
void TutorialScene::FrontDraw(DirectXCommon* dxCommon) {
	////完全に前に書くスプライト
	text_->TestDraw(dxCommon);
	ParticleEmitter::GetInstance()->FlontDrawAll();
	GameStateManager::GetInstance()->ActUIDraw();

	//Player::GetInstance()->UIDraw();
	enemy->UIDraw();
	SceneChanger::GetInstance()->Draw();
}
//ポストエフェクトかかる
void TutorialScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	GameStateManager::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	enemy->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui
void TutorialScene::ImGuiDraw() {
	ImGui::Begin("Tutorial");
	ImGui::Text("Timer:%d", m_Timer);
	ImGui::Text("State:%d", _nowstate);
	ImGui::End();
	
	TutorialTask::GetInstance()->ImGuiDraw();
}

void TutorialScene::Finalize() {

}
//最初の語り
void TutorialScene::IntroState() {
	if (Helper::GetInstance()->CheckMin(m_Timer, 150, 1)) {
		_nowstate = TUTORIAL_MOVE;
		m_Timer = {};
	
	}
}
//移動
void TutorialScene::MoveState() {
	if (Helper::GetInstance()->CheckMin(m_Timer, 150, 1)) {
		m_Timer = {};
		TutorialTask::GetInstance()->SetTutorialState(TASK_BIRTH_BEFORE);
	}

	if (TutorialTask::GetInstance()->GetTutorialState() == TASK_BIRTHSKIL) {
		text_->SetConversation(TextManager::TUTORIAL_GET);
		_nowstate = TUTORIAL_GETSKILL;
	}
}
//スキルゲット
void TutorialScene::GetState() {

	if (TutorialTask::GetInstance()->GetTutorialState() == TASK_ATTACK) {
		text_->SetConversation(TextManager::TUTORIAL_EXPLAIN);
		_nowstate = TUTORIAL_ATTACK;
		m_Timer = {};
	}
}
//攻撃
void TutorialScene::AttackState() {
	Helper::GetInstance()->CheckMin(m_Timer, 410, 1);

	if (m_Timer == 200) {
		text_->SetConversation(TextManager::TUTORIAL_MARK);
	}
	else if (m_Timer == 400) {
		text_->SetConversation(TextManager::TUTORIAL_TEXT_ATTACK);
	}

	if (TutorialTask::GetInstance()->GetTutorialState() == TASK_DAMAGE) {
		text_->SetConversation(TextManager::TUTORIAL_TEXT_DAMAGE);
		_nowstate = TUTORIAL_DAMAGE;
		m_Timer = {};
	}
}
//ダメージが入った
void TutorialScene::DamageState() {
	if (enemy->GetHP() <= 0.0f) {
		m_Timer++;
		if (m_Timer == 10) {
			text_->SetConversation(TextManager::TUTORIAL_SKILL);
		}
		else if (m_Timer == 200) {
			text_->SetConversation(TextManager::TUTORIAL_END);
		}
		else if (m_Timer == 400) {
			_nowstate = TUTORIAL_FINISH;
		}
		GameStateManager::GetInstance()->StageClearInit();
	}
}
//チュートリアル終わり
void TutorialScene::TutorialEnd() {
	if (Helper::GetInstance()->CheckMin(m_Timer, 150, 1)) {
		m_Timer = {};
		m_End = true;
	}
}