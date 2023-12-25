#include "BattleScene.h"
#include <ImageManager.h>
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <SceneChanger.h>
#include <Helper.h>
#include "InterEnemy.h"
#include <StageBack.h>
#include <Slow.h>
BattleScene::~BattleScene() {
	Finalize();
}
// 初期化
void BattleScene::Initialize(DirectXCommon* dxCommon)
{
	//シーンの基本初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	// ポストエフェクト
	PlayPostEffect = false;

	// パーティクル
	ParticleEmitter::GetInstance()->AllDelete();

	player_ = make_unique<Player>();
	player_->LoadResource();
	player_->InitState({ -PANEL_SIZE * 2.f,0.1f,PANEL_SIZE });
	player_->Initialize();
	//ゲームステート初期化
	GameStateManager::GetInstance()->SetDxCommon(dxCommon);
	if (s_Countinue) {		//コンティニューをしていた場合CSVからゲームデータ引き継ぎ
		player_->SetHp(GameStateManager::GetInstance()->GetOpenHP());
		s_Countinue = false;
	}
	GameStateManager::SetPlayer(player_.get());
	GameStateManager::GetInstance()->Initialize();
	
	//ステージパネルの初期化
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->SetPlayer(player_.get());
	StagePanel::GetInstance()->Initialize(6.0f);
	//ビヘイビア試しました！
	{
		//auto test_enemy_1 = GameObject::CreateObject<TestEnemy>();
	}

	//敵
	EnemyManager::SetPlayer(player_.get());
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();

	//パッシブスキルによるエネミーの能力変更
	if (GameStateManager::GetInstance()->GetPoisonSkill()) {
		enemyManager->PoizonGauge();
	}
	if (GameStateManager::GetInstance()->GetPoisonVenom()) {
		enemyManager->PoizonVenom();
	}

	StagePanel::GetInstance()->DeleteAction();

	gameoversprite = IKESprite::Create(ImageManager::GAMEOVERBACK, { 0.0f,0.0f });
	Slow::GetInstance()->Initialize();
	skipUI = IKESprite::Create(ImageManager::RESULTSKIP, { 10.f,10.f }, { 1.f,1.f, 1.f, 1.f });
	skipUI->SetSize({ 512.f * scale_skip,128.f * scale_skip });

	Feed* feed_ = new Feed();
	feed.reset(feed_);
}
//更新
void BattleScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	//ライト更新
	lightGroup->Update();
	//�e�N���X�X�V
	//カメラワーク更新
	camerawork->Update(camera);
	//ゲームオーバー処理
	if (player_->GetHp() <= 0.0f) {
		Audio::GetInstance()->StopWave(AUDIO_MAIN);
		_ChangeType = CHANGE_OVER;
		player_->GameOverUpdate();
		if (player_->GetFinishGameOver()) {
			if (Helper::FrameCheck(m_GameOverFrame, 0.01f)) {
				m_ChangeTimer++;
				if (m_ChangeTimer == 1) {
					Audio::GetInstance()->PlayWave("Resources/Sound/SE/GameOver.wav", 0.1f);
				}

				//タイトル戻るかコンティニューしてマップ行くか決まる
				if ((input->TriggerButton(input->A)) && nowHierarchy >= 6) {		//コンティニュー
					SceneChanger::GetInstance()->SetChangeStart(true);
					s_Countinue = true;
				}
				else if ((input->TriggerButton(input->B))) {
					SceneChanger::GetInstance()->SetChangeStart(true);
					s_Countinue = false;
				}
			}
			else {
				m_GameOverPos.y = { Ease(In,Cubic,m_GameOverFrame,m_GameOverPos.y,0.0f) };
			}
		}
	}
	else {		//ゲームオーバー以外
		if (m_IsBackKey) {
			const float frameMax = 5.f;
			m_Skip = true;
			m_IsBackKey = false;
			return;
		}

		if ((input->TriggerButton(input->A) ||
			input->TriggerKey(DIK_BACK)) &&
			!m_IsBackKey &&
			!m_Skip) {
			m_IsBackKey = true;
		}
		skip_alpha += 0.05f;
		skipUI->SetColor({ 1.f,1.f,1.f,abs(sinf(skip_alpha)) });
		SkipUpdate();

		if (m_SkipFeed) {
			feed->FeedIn(Feed::FeedType::WHITE, 1.0f / 30.0f, m_SkipFeed);
		}
		player_->AwakeUpdate();
		player_->Update();
		GameStateManager::GetInstance()->Update();
		if (GameStateManager::GetInstance()->GetIsReloadDamage()) {
			enemyManager->ReLoadDamage();
			GameStateManager::GetInstance()->SetIsReloadDamage(false);
		}

		if (GameStateManager::GetInstance()->GetHealDamage()) {
			enemyManager->HealingDamage();
			GameStateManager::GetInstance()->SetHealDamage(false);
		}


		if (GameStateManager::GetInstance()->GetIsBombDamage()) {
			enemyManager->BombDamage();
			GameStateManager::GetInstance()->SetIsBombDamage(false);
		}

		if (GameStateManager::GetInstance()->GetIsHeal()) {
			enemyManager->Heal();
			GameStateManager::GetInstance()->SetIsHeal(false);
		}

		//後々変更する(酷い処理)
		//エネミーが全員死亡したら
		if (enemyManager->BossDestroy() && !m_FeedStart) {
			Slow::GetInstance()->SetSlow(true);
			if (!s_LastStage) {
				m_Feed = true;
				m_BattleEnd = true;
				m_FeedStart = true;
			}
			else {
				Audio::GetInstance()->StopWave(AUDIO_MAIN);
				SceneChanger::GetInstance()->SetChangeStart(true);
			}
		}
		if (m_Feed) {
			feed->FeedIn(Feed::FeedType::WHITE, 1.0f / 60.0f, m_Feed);
		}
		if (feed->GetFeedEnd() && m_BattleEnd) {
			m_FeedEnd = true;
		}

		StagePanel::GetInstance()->CreateStage();
		StagePanel::GetInstance()->Update();
		enemyManager->AwakeUpdate();
		enemyManager->Update();
		ParticleEmitter::GetInstance()->Update();

		if (m_FeedEnd) {
			Audio::GetInstance()->StopWave(AUDIO_MAIN);
			//クリア処理が終らなかったら
			if (!GameStateManager::GetInstance()->GetIsChangeScene()) {
				//クリア処理準備
				GameStateManager::GetInstance()->StageClearInit();
			}
			else {
				//マップに戻る
				_ChangeType = CHANGE_MAP;
				SceneChanger::GetInstance()->SetChangeStart(true);
			}
		}

		//クリア条件に達するとプレイヤーを動かせなくする
		if (GameStateManager::GetInstance()->GetIsFinish()) {
			player_->SetDelay(true);
		}
	}
	//共通の更新
	SceneChanger::GetInstance()->Update();
	//シーン切り替え処理
	if (SceneChanger::GetInstance()->GetChange()) {
		GameStateManager::GetInstance()->SetSaveHP(player_->GetHp());
		//ゲームクリア
		if (_ChangeType == CHANGE_MAP) {
			if (!s_LastStage) {
				SceneManager::GetInstance()->ChangeScene("MAP");
			}
			else {
				SceneManager::GetInstance()->ChangeScene("CLEAR");
			}
		}
		else {		//ゲームオーバー
			if (!s_Countinue) {
				SceneManager::GetInstance()->ChangeScene("TITLE");
			}
			else {		//コンティニューをしてマップに戻る
				SceneManager::GetInstance()->ChangeScene("MAP");
			}
		}
		player_->PlayerSave();
		SceneChanger::GetInstance()->SetChange(false);
	}
	gameoversprite->SetPosition(m_GameOverPos);
}
//描画
void BattleScene::Draw(DirectXCommon* dxCommon) {
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
//前方描画(奥に描画するやつ)
void BattleScene::FrontDraw(DirectXCommon* dxCommon) {
	if (!m_Skip && !GameStateManager::GetInstance()->GetGameStart()) {
		IKESprite::PreDraw();
		skipUI->Draw();
		IKESprite::PostDraw();
	}
	if (!m_FeedEnd){
		if (player_->GetHp() > 0.0f && GameStateManager::GetInstance()->GetGameStart()) {
			ParticleEmitter::GetInstance()->FlontDrawAll();
			player_->UIDraw();
			enemyManager->UIDraw();
			GameStateManager::GetInstance()->ActUIDraw();
		}
	}
	StagePanel::GetInstance()->OnomatoDraw();
	IKESprite::PreDraw();
	if (player_->GetFinishGameOver()) {
		gameoversprite->Draw();
	}
	if (m_Feed || m_SkipFeed) {
		feed->Draw();
	}
	IKESprite::PostDraw();
	SceneChanger::GetInstance()->Draw();
}
//後方描画(主にSprite)
void BattleScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	StageBack::GetInstance()->Draw(dxCommon);
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	player_->Draw(dxCommon);
	if (player_->GetHp() > 0.0f) {
		enemyManager->Draw(dxCommon);
		if (GameStateManager::GetInstance()->GetGameStart()) {
			GameStateManager::GetInstance()->Draw(dxCommon);
			if (!enemyManager->BossDestroy()) {
				StagePanel::GetInstance()->ActDraw(dxCommon);
			}
		}
	}
	IKEObject3d::PostDraw();
}
//ImGui
void BattleScene::ImGuiDraw() {
	//GameStateManager::GetInstance()->ImGuiDraw();
	StagePanel::GetInstance()->ImGuiDraw();
	enemyManager->ImGuiDraw();
	player_->ImGuiDraw();
}

void BattleScene::Finalize() {

}
void BattleScene::SkipUpdate() {
	if (GameStateManager::GetInstance()->GetGameStart()) { return; }
	Input* input = Input::GetInstance();
	if (m_Skip) {
		if (!m_FeedStart) {
			m_SkipFeed = true;
		}
		
		if (feed->GetFeedEnd()) {
			m_SkipEnd = true;
		}

		if (m_SkipEnd) {
			player_->SkipInitialize();
			enemyManager->SkipInitialize();
			StagePanel::GetInstance()->Initialize();
			StagePanel::GetInstance()->SetCreateFinish(true);
			GameStateManager::GetInstance()->SetGameStart(true);
		}
	}
}