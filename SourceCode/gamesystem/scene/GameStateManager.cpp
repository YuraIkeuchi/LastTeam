﻿#include <GameStateManager.h>
#include <Helper.h>
#include <StagePanel.h>
#include <Input.h>
#include <Easing.h>
#include <ImageManager.h>
#include <SkillManager.h>
#include <Player.h>
#include <TutorialTask.h>

GameStateManager* GameStateManager::GetInstance() {
	static GameStateManager instance;
	return &instance;
}
//初期化
void GameStateManager::Initialize() {
	//カウンター
	m_Counter = false;
	m_CounterTimer = {};
	m_CounterScore = {};
	//位置のスコア
	m_PosScore = {};
	//グレイズのスコア(後々intにする)
	m_GrazeScore = 0.0f;

	//全体スコア
	m_AllScore = {};

	//終了関連
	isFinish = false;
	isChangeScene = false;


	//要素の全削除は一旦ここで
	m_AllActCount = {};
	actui.clear();
	m_Act.clear();
	attackarea.clear();
	//一旦クリア方式で
	GotPassives.clear();
	PassiveCheck();
	skillUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,600.f }, { 1.f,1.f,1.f,1.f }, { 0.5f,1.f });
	skillUI->SetSize(basesize);
	gaugeUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,600.f }, { 0.f,1.f,0.f,1.f }, { 0.5f,1.f });
	gaugeUI->SetSize({ basesize.x,0.f });

	resultSkill = make_unique<ResultSkill>();
	resultSkill->Initialize();
	//デッキの初期化
	DeckInitialize();

	//デッキにないカードを検索する
	const int CARD_MAX = 7;
	m_NotDeckNumber.clear();
	for (int i = 0; i < m_DeckNumber.size(); i++) {
		for (int j = 0; j < CARD_MAX; j++) {
			bool found = std::find(m_DeckNumber.begin(), m_DeckNumber.end(), j) != m_DeckNumber.end();		//最初から探す
			if (!found) {
				m_NotDeckNumber.push_back(j);		//なかったら追加する
			}
		}
		break;
	}
	predictarea.reset(new PredictArea());
	predictarea->Initialize();

	m_GaugeCount = {};
	m_NotCount = (int)(m_NotDeckNumber.size()) - 1;//無いカードの枚数を検索してる(ImGui用)
}
//更新
void GameStateManager::Update() {
	if (ResultUpdate()) { return; }
	const int l_AddCounterScore = 10;
	m_AllScore = m_CounterScore + (int)(m_PosScore)+(int)(m_GrazeScore);

	//カウンターの処理
	if (m_Counter) {
		if (m_CounterTimer == 0) {		//カウンターのスコアに加算
			m_CounterScore += l_AddCounterScore;
		}
		m_CounterTimer++;

		if (Helper::GetInstance()->CheckMin(m_CounterTimer, 20, 1)) {		//一定フレームでカウンター終了
			m_Counter = false;
			m_CounterTimer = {};
		}
	}

	//手に入れたスキルのUIの更新
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->SetActCount(i);
		actui[i]->Update();

		if (!actui[i]->GetAlive()) {
			actui.erase(cbegin(actui) + i);
		}
	}

	//攻撃エリアの更新(実際はスキルになると思う)
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Update();

		if (!attackarea[i]->GetAlive()) {
			attackarea.erase(cbegin(attackarea) + i);
		}
	}

	GaugeUpdate();
	//攻撃した瞬間
	AttackTrigger();
	UseSkill();
	if (m_ResetPredict) {
		PredictManager();
		m_ResetPredict = false;
	}
	SkillManager::GetInstance()->Update();

}
//攻撃した瞬間
void GameStateManager::AttackTrigger() {
	Input* input = Input::GetInstance();
	if (m_AllActCount == 0) { return; }
	if (actui[0]->GetUse()) { return; }
	if (Player::GetInstance()->GetCharaState() == 1) { return; }
	//スキルが一個以上あったらスキル使える
	if (input->TriggerButton(input->A)) {
		m_BirthSkill = true;
		Player::GetInstance()->SetDelayTimer(m_Act[0].ActDelay);
		Player::GetInstance()->SetDelayStart(true);
	}
}
void GameStateManager::Draw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	skillUI->Draw();
	gaugeUI->Draw();
	SkillManager::GetInstance()->UIDraw();
	IKESprite::PostDraw();

	resultSkill->Draw();

	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Draw(dxCommon);
	}

	if (m_AllActCount != 0) {
		predictarea->Draw(dxCommon);
	}
}
//描画
void GameStateManager::ImGuiDraw() {
	ImGui::Begin("GameState");
	if (!m_Act.empty()) {
		ImGui::Text("damage:%f", m_Act[0].ActDamage);
		ImGui::Text("delay:%d", m_Act[0].ActDelay);
	}
	ImGui::SliderInt("Count",&m_NotCount, 0, (int)(m_NotDeckNumber.size() - 1));		//追加するカードを選べる
	if (ImGui::Button("in", ImVec2(90, 50))) {
		InDeck();		//デッキに入っていないカードをデッキに組み込む
	}
	ImGui::End();
	SkillManager::GetInstance()->ImGuiDraw();
	StagePanel::GetInstance()->ImGuiDraw();
}
//手に入れたUIの描画
void GameStateManager::ActUIDraw() {
	IKESprite::PreDraw();
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->Draw();
	}
	IKESprite::PostDraw();
	for (unique_ptr<Passive>& passive : GotPassives) {
		passive->Draw();
	}
}
//スキルを入手(InterActionCPPで使ってます)
void GameStateManager::AddSkill(const int ID, const float damage,const int Delay, vector<std::vector<int>> area) {
	ActState act;
	act.ActID = ID;
	act.ActDamage = damage;
	act.ActDelay = Delay;
	act.AttackArea.resize(3);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			act.AttackArea[i].push_back(j);
		}
	}
	act.AttackArea = area;
	m_Act.push_back(act);
	//手に入れたスキルの総数を加算する
	m_AllActCount++;
	BirthActUI(ID);//UIも増えるよ
	SkillManager::GetInstance()->GetAreaDate(m_DistanceX, m_DistanceY);
	PredictManager();
}
//スキルUIの生成
void GameStateManager::BirthActUI(const int ID) {
	//アクションUIのセット
	ActionUI* newactUi = nullptr;
	newactUi = new ActionUI();
	newactUi->Initialize();
	newactUi->InitState(m_AllActCount,ID);
	actui.emplace_back(newactUi);

	Audio::GetInstance()->PlayWave("Resources/Sound/SE/cardget.wav", 0.3f);
}
//攻撃エリアの生成(無理やり処理)
void GameStateManager::BirthArea() {
	int l_BirthBaseX = {};
	int l_BirthBaseY = {};

	l_BirthBaseX = m_NowWidth + m_DistanceX;		//生成の初めの位置を見てる
	l_BirthBaseY = m_NowHeight + m_DistanceY;
	int AreaX = {};
	int AreaY = {};
	for (auto i = 0; i < m_Act[0].AttackArea.size(); i++) {
		for (auto j = 0; j < m_Act[0].AttackArea.size(); j++) {
			AreaX = l_BirthBaseX + i;
			AreaY = l_BirthBaseY - j;
			if (m_Act[0].AttackArea[i][j] == 1 && ((AreaY < 4) && (AreaY >= 0))) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				AttackArea* newarea = nullptr;
				newarea = new AttackArea();
				newarea->Initialize();
				newarea->InitState(AreaX, AreaY);
				newarea->SetDamage(m_Act[0].ActDamage);
				attackarea.push_back(newarea);
			}
		}
	}
}
//予測エリア関係
void GameStateManager::PredictManager() {
	if (m_AllActCount == 0) { return; }
	predictarea->ResetPredict();
	int l_BirthBaseX = {};
	int l_BirthBaseY = {};

	l_BirthBaseX = m_NowWidth + m_DistanceX;		//生成の初めの位置を見てる
	l_BirthBaseY = m_NowHeight + m_DistanceY;

	for (auto i = 0; i < m_Act[0].AttackArea.size(); i++) {
		for (auto j = 0; j < m_Act[0].AttackArea.size(); j++) {
			
			int AreaX = {};
			int AreaY = {};
			AreaX = l_BirthBaseX + i;
			AreaY = l_BirthBaseY - j;
			if (m_Act[0].AttackArea[i][j] == 1 && ((AreaY < 4) && (AreaY >= 0))) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				predictarea->SetPredict(AreaX, AreaY, true);
			}
		}
	}

	predictarea->Update();
}
//プレイヤーの現在パネル
void GameStateManager::PlayerNowPanel(const int NowWidth, const int NowHeight) {
	m_NowWidth = NowWidth, m_NowHeight = NowHeight;
}
//スキルの使用
void GameStateManager::UseSkill() {
	if (!Player::GetInstance()->GetDelayStart() && m_BirthSkill) {
		BirthArea();
		FinishAct();
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/SkillUse.wav", 0.3f);
		m_BirthSkill = false;
	}
}
//行動の終了
void GameStateManager::FinishAct() {
	m_Act.erase(m_Act.begin());
	m_AllActCount--;
	actui[0]->SetUse(true);
	//デッキがない且つ手札を使い切ってたらまた再配布
	if (m_AllActCount == 0 && StagePanel::GetInstance()->GetAllDelete()) {
		//デッキの初期化
		DeckInitialize();
	}
}

void GameStateManager::GaugeUpdate() {
	if (SkillManager::GetInstance()->GetDeckNum() != 0 && (TutorialTask::GetInstance()->GetTutorialState() >= TASK_BIRTH_BEFORE)) {
		m_GaugeCount += 1.0f * m_DiameterGauge;
	}
	if (m_GaugeCount >= kGaugeCountMax) {
		if (m_IsReload) {
			StagePanel::GetInstance()->ResetAction();
			StagePanel::GetInstance()->ResetPanel();
		}
		//パネル置く数
		int panel_num = 3;
		SkillManager::GetInstance()->ResetBirth();
		if (SkillManager::GetInstance()->GetDeckNum() >= 3) {
			StagePanel::GetInstance()->RandomPanel(panel_num);
		}
		else {
			StagePanel::GetInstance()->RandomPanel(SkillManager::GetInstance()->GetDeckNum());
		}
		m_GaugeCount = 0;
		if (TutorialTask::GetInstance()->GetTutorialState() == TASK_BIRTH_BEFORE) {		//チュートリアル専用
			TutorialTask::GetInstance()->SetTutorialState(TASK_BIRTHSKIL);
		}
	}
	float per = (m_GaugeCount / kGaugeCountMax);
	float size = Ease(In, Quad, 0.5f, gaugeUI->GetSize().y, basesize.y * per);
	gaugeUI->SetSize({ basesize.x,size });
}

void GameStateManager::PassiveCheck() {

	for (int& id:GotPassiveIDs) {
		GetPassive(id);
	}

	for (unique_ptr<Passive>& passive : GotPassives) {
		switch (passive->GetAbility()) {
		case Passive::ABILITY::RELOAD_UP:
			m_DiameterGauge = passive->GetDiameter();
			break;
		case Passive::ABILITY::HP_UP:
			Player::GetInstance()->SetMaxHp(
				Player::GetInstance()->GetMaxHp()* passive->GetDiameter());
			break;
		case Passive::ABILITY::RELOAD_LOCK:
			m_IsReload = false;
			break;
		default:
			break;
		}
	}
	const int PASSIVE_MAX = 3;
	NotPassiveIDs.clear();
	if (GotPassiveIDs.size() == 0) { 
		for (int j = 0; j < PASSIVE_MAX; j++) {
			NotPassiveIDs.push_back(j);		//なかったら追加する
		}
		return; 
	}
	for (int i = 0; i < GotPassiveIDs.size(); i++) {
		for (int j = 0; j < PASSIVE_MAX; j++) {
			bool found = std::find(GotPassiveIDs.begin(), GotPassiveIDs.end(), j) != GotPassiveIDs.end();		//最初から探す
			if (!found) {
				NotPassiveIDs.push_back(j);		//なかったら追加する
			}
		}
		break;
	}
}
//デッキの初期化
void GameStateManager::DeckInitialize() {
	SkillManager::GetInstance()->DeckClear();
	//デッキに入っているカードの確認
	for (int i = 0; i < m_DeckNumber.size(); i++) {
		SkillManager::GetInstance()->DeckCheck(m_DeckNumber[i],i);
	}
	//デッキの最大数確認
	SkillManager::GetInstance()->SetDeckState((int)(m_DeckNumber.size()));
}

void GameStateManager::GetPassive(int ID) {
	float posX = GotPassives.size() * 70.0f;
	GotPassives.push_back(std::move(make_unique<Passive>(ID, XMFLOAT2{ posX ,0.0f})));
}


bool GameStateManager::ResultUpdate() {
	if (!isFinish) { return false; }

	resultSkill->Update();

	if (Input::GetInstance()->TriggerButton(Input::B)) {
		resultSkill->InDeck(m_DeckNumber);
		resultSkill->InPassive(GotPassiveIDs);
		isChangeScene = true;
		isFinish = false;
	}
	return true;
}

void GameStateManager::InDeck() {
	m_DeckNumber.push_back(m_NotDeckNumber[m_NotCount]);
	m_NotDeckNumber.erase(cbegin(m_NotDeckNumber) + m_NotCount);
}

void GameStateManager::StageClearInit() {
	if (isFinish) { return; }
	resultSkill->CreateResult(m_NotDeckNumber, NotPassiveIDs);
	isFinish = true;
}
