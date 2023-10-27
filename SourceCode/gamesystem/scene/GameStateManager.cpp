#include <GameStateManager.h>
#include <Helper.h>
#include <StagePanel.h>
#include <Input.h>
#include <Easing.h>
#include <ImageManager.h>
#include <SkillManager.h>
#include <Player.h>

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

	//要素の全削除は一旦ここで
	m_AllActCount = {};
	actui.clear();
	m_Act.clear();
	attackarea.clear();

	GotPassives.push_back(std::move(make_unique<Passive>(1)));
	GotPassives.push_back(std::move(make_unique<Passive>(2, XMFLOAT2{ 70.f,0.f })));
	GotPassives.push_back(std::move(make_unique<Passive>(3, XMFLOAT2{ 140.f,0.f })));

	PassiveCheck();
	skillUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,600.f }, { 1.f,1.f,1.f,1.f }, { 0.5f,1.f });
	skillUI->SetSize(basesize);
	gaugeUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,600.f }, { 0.f,1.f,0.f,1.f }, { 0.5f,1.f });
	gaugeUI->SetSize({ basesize.x,0.f });
}
//更新
void GameStateManager::Update() {

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
		Player::GetInstance()->SetDelayStart(true);
		UseSkill();
		
	}
	
}
void GameStateManager::Draw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	skillUI->Draw();
	gaugeUI->Draw();
	IKESprite::PostDraw();

	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Draw(dxCommon);
	}
}
//描画
void GameStateManager::ImGuiDraw() {
	ImGui::Begin("GameState");
	if (ImGui::Button("NORMALSKILL", ImVec2(50, 50))) {
		_SkillType = SKILL_NORMAL;
	}
	if (ImGui::Button("STRONGSKILL", ImVec2(50, 50))) {
		_SkillType = SKILL_STRONG;
	}
	if (ImGui::Button("SPECIALSKILL", ImVec2(50, 50))) {
		_SkillType = SKILL_SPECIAL;
	}
	ImGui::End();
	SkillManager::GetInstance()->ImGuiDraw();
	/**/
	
	/*
	
	/*if (!m_Act.empty()) {
		for (auto i = 0; i < m_Act.size(); i++) {
			ImGui::Text("Act[%d]:%d", i, m_Act[i].ActID);
		}
	}*/
	//StagePanel::GetInstance()->ImGuiDraw();
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
void GameStateManager::AddSkill(const int ID, const float damage) {
	ActState act;
	act.ActID = ID;
	act.actDamage = damage;
	m_Act.push_back(act);
	//手に入れたスキルの総数を加算する
	m_AllActCount++;
	BirthActUI(ID);//UIも増えるよ
}
//スキルUIの生成
void GameStateManager::BirthActUI(const int ID) {
	//アクションUIのセット
	ActionUI* newactUi = nullptr;
	newactUi = new ActionUI();
	newactUi->Initialize();
	newactUi->InitState(m_AllActCount);
	newactUi->SetID(ID);
	actui.emplace_back(newactUi);

	Audio::GetInstance()->PlayWave("Resources/Sound/SE/cardget.wav", 0.3f);
}
//攻撃エリアの描画(無理やり処理)
void GameStateManager::BirthArea() {
	int l_BirthNumX = {};//パネルのマックス数

	int l_BirthCountX = {};
	int l_BirthCountZ = {};

	if (_SkillType == SKILL_NORMAL) {		//普通に一個右
		l_BirthCountX = m_NowWidth + 1;
		AttackArea* newarea = nullptr;
		newarea = new AttackArea();
		newarea->Initialize();
		newarea->InitState(l_BirthCountX, m_NowHeight);
		newarea->SetDamage(m_Act[0].actDamage);
		attackarea.push_back(newarea);
	} else if (_SkillType == SKILL_STRONG) {		//プレイヤーの一から右一列全部
		l_BirthNumX = PANEL_WIDTH - (m_NowWidth + 1);
		for (int i = 0; i < l_BirthNumX; i++) {
			l_BirthCountX = (m_NowWidth + 1) + i;
			AttackArea* newarea = nullptr;
			newarea = new AttackArea();
			newarea->Initialize();
			newarea->InitState(l_BirthCountX, m_NowHeight);
			newarea->SetDamage(m_Act[0].actDamage);
			attackarea.push_back(newarea);
		}
	} else {				//プレイヤーから3 * 2のマス
		for (int j = 0; j < 3; j++) {
			l_BirthCountZ = (m_NowHeight - 1) + j;
			if (l_BirthCountZ < 0 || l_BirthCountZ > 3) {
				continue;
			}
			for (int i = 0; i < 2; i++) {
				l_BirthCountX = (m_NowWidth + 1) + i;
				AttackArea* newarea = nullptr;
				newarea = new AttackArea();
				newarea->Initialize();
				newarea->InitState(l_BirthCountX, l_BirthCountZ);
				newarea->SetDamage(m_Act[0].actDamage);
				attackarea.push_back(newarea);
			}
		}
	}

}
//プレイヤーの現在パネル
void GameStateManager::PlayerNowPanel(const int NowWidth, const int NowHeight) {
	m_NowWidth = NowWidth, m_NowHeight = NowHeight;
}
//スキルの使用
void GameStateManager::UseSkill() {

	BirthArea();
	FinishAct();
	Audio::GetInstance()->PlayWave("Resources/Sound/SE/SkillUse.wav", 0.3f);
	m_BirthSkill = false;
	if (!Player::GetInstance()->GetDelayStart() && m_BirthSkill) {

	}
}
//行動の終了
void GameStateManager::FinishAct() {
	m_Act.erase(m_Act.begin());
	m_AllActCount--;
	actui[0]->SetUse(true);
}

void GameStateManager::GaugeUpdate() {

	m_GaugeCount += 1.0f * m_DiameterGauge;
	if (m_GaugeCount >= kGaugeCountMax) {
		if (m_IsReload) {
			StagePanel::GetInstance()->ResetAction();
			StagePanel::GetInstance()->ResetPanel();
		}
		//パネル置く数
		int panel_num = 3;
		SkillManager::GetInstance()->ResetBirth();
		StagePanel::GetInstance()->RandomPanel(panel_num);
		m_GaugeCount = 0;
	}
	float per = (m_GaugeCount / kGaugeCountMax);
	float size = Ease(In, Quad, 0.5f, gaugeUI->GetSize().y, basesize.y * per);
	gaugeUI->SetSize({ basesize.x,size });
}

void GameStateManager::PassiveCheck() {
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
}
