#include <GameStateManager.h>
#include <Helper.h>
#include <StagePanel.h>
#include <Input.h>
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
}
//更新
void GameStateManager::Update() {
	Input* input = Input::GetInstance();
	const int l_AddCounterScore = 10;
	m_AllScore = m_CounterScore + (int)(m_PosScore) + (int)(m_GrazeScore);

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

	//スキルが一個以上あったらスキル使える
	if (input->TriggerButton(input->A) && m_AllActCount != 0 && !actui[0]->GetUse()) {
		UseSkill();
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/SkillUse.wav", 0.3f);
	}
}
void GameStateManager::Draw(DirectXCommon* dxCommon) {
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Draw(dxCommon);
	}
}
//描画
void GameStateManager::ImGuiDraw() {
	ImGui::Begin("GameState");
	ImGui::Text(m_Counter ? "CLEAR" : "NONE");
	ImGui::Text("CounterTimer:%d", m_CounterTimer);
	ImGui::Text("CounteScore:%d", m_CounterScore);
	ImGui::Text("PosScore:%d", (int)(m_PosScore));
	ImGui::Text("Graze:%d", (int)m_GrazeScore);
	ImGui::Text("All:%d", m_AllScore);
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
}
//手に入れたUIの描画
void GameStateManager::ActUIDraw() {
	IKESprite::PreDraw();
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->Draw();
	}
	IKESprite::PostDraw();
}
//スキルを入手(InterActionCPPで使ってます)
void GameStateManager::AddSkill(const string& Tag) {
	if (Tag == "Attack") {
		m_Act.push_back(ACT_ATTACK);
	}
	else if (Tag == "Guard") {
		m_Act.push_back(ACT_GUARD);
	}
	else if (Tag == "Skill") {
		m_Act.push_back(ACT_SKILL);
	}
	else {
		assert(0);
	}

	//手に入れたスキルの総数を加算する
	m_AllActCount++;
	BirthActUI(Tag);//UIも増えるよ
}
//スキルUIの生成
void GameStateManager::BirthActUI(const string& Tag) {
	//アクションUIのセット
	ActionUI* newactUi = nullptr;
	newactUi = new ActionUI();
	newactUi->Initialize();
	newactUi->InitState(m_AllActCount, Tag);
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
		attackarea.push_back(newarea);
	}
	else if (_SkillType == SKILL_STRONG) {		//プレイヤーの一から右一列全部
		l_BirthNumX = PANEL_WIDTH - (m_NowWidth + 1);
		for (int i = 0; i < l_BirthNumX; i++) {
			l_BirthCountX = (m_NowWidth + 1) + i;
			AttackArea* newarea = nullptr;
			newarea = new AttackArea();
			newarea->Initialize();
			newarea->InitState(l_BirthCountX, m_NowHeight);
			attackarea.push_back(newarea);
		}
	}
	else {				//プレイヤーから3 * 2のマス
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
}
//行動の終了
void GameStateManager::FinishAct() {
	m_Act.erase(m_Act.begin());
	m_AllActCount--;
	actui[0]->SetUse(true);
}