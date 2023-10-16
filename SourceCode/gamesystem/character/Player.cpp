#include "Player.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "GameMode.h"
#include "StagePanel.h"
Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}
//リソース読み込み
void Player::LoadResource() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	m_Object->SetScale({ 2.f,2.f,2.f });
	m_Object->SetPosition({ 0.0f,2.0f,0.0f });
	m_Object->VertexCheck();
}
//初期化
bool Player::Initialize()
{

	LoadCSV();
	//CSV読み込み
	return true;
}
//CSV読み込み
void Player::LoadCSV() {
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "speed")));
}
//ステータスの初期化
void Player::InitState(const XMFLOAT3& pos) {
	m_Position = pos;
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	_charaState = STATE_MOVE;
	//移動処理用
	velocity /= 5.0f;
	//攻撃先
	m_TargetPos = {};
	//戻り先
	m_ReturnPos = {};
	//イージング
	m_Frame = {};
	m_CoolTime = {};
	_AttackState = ATTACK_ENEMY;
	m_AllActCount = {};
	m_AttackTimer = {};
	//要素の全削除は一旦ここで
	actui.clear();
	m_Act.clear();
	attackarea.clear();
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Player::* Player::stateTable[])() = {
	&Player::Move,//移動
	&Player::SpecialAct,//特別な行動
};
//更新処理
void Player::Update()
{
	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();
	Obj_SetParam();
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->SetActCount(i);
		actui[i]->Update();

		if (!actui[i]->GetAlive()) {
			actui.erase(cbegin(actui) + i);
		}
	}

	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Update();

		if (!attackarea[i]->GetAlive()) {
			attackarea.erase(cbegin(attackarea) + i);
		}
	}

	BirthParticle();
}
//描画
void Player::Draw(DirectXCommon* dxCommon)
{
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Draw(dxCommon);
	}
	Obj_Draw();
}
void Player::ActUIDraw() {
	IKESprite::PreDraw();
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->Draw();
	}
	IKESprite::PostDraw();
}
//ImGui
void Player::ImGuiDraw() {
	ImGui::Begin("Player");
	ImGui::Text("NowWidth:%d", m_NowWidth);
	ImGui::End();
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->ImGuiDraw();
	}
}

//移動
void Player::Move() {
	const int l_TargetTimer = 10;
	const float l_Velocity = 2.0f;

	//ボタンでマスを移動する
	if (input->PushButton(input->UP) ||
		input->PushButton(input->DOWN) ||
		input->PushButton(input->RIGHT) ||
		input->PushButton(input->LEFT)) {
		if (input->PushButton(input->UP)) {
			m_InputTimer[DIR_UP]++;
		}
		else if (input->PushButton(input->DOWN)) {
			m_InputTimer[DIR_DOWN]++;
		}
		else if (input->PushButton(input->RIGHT)) {
			m_InputTimer[DIR_RIGHT]++;
		}
		else if (input->PushButton(input->LEFT)) {
			m_InputTimer[DIR_LEFT]++;
		}
	}
	else {			//離した瞬間
		if (m_LimitCount == 0) {
			if (m_InputTimer[DIR_UP] != 0 && m_NowHeight < PANEL_HEIGHT - 1) {
				m_NowHeight++;
				m_InputTimer[DIR_UP] = {};
				m_Position.z += l_Velocity;
			}
			else if (m_InputTimer[DIR_DOWN] != 0 && m_NowHeight > 0) {
				m_NowHeight--;
				m_InputTimer[DIR_DOWN] = {};
				m_Position.z -= l_Velocity;
			}
			else if (m_InputTimer[DIR_RIGHT] != 0 && m_NowWidth < (PANEL_WIDTH / 2) - 1) {
				m_NowWidth++;
				m_InputTimer[DIR_RIGHT] = {};
				m_Position.x += l_Velocity;
			}
			else if (m_InputTimer[DIR_LEFT] != 0 && m_NowWidth > 0) {
				m_NowWidth--;
				m_InputTimer[DIR_LEFT] = {};
				m_Position.x -= l_Velocity;
			}
		}
		for (int i = 0; i < DIR_MAX; i++) {
			m_InputTimer[i] = {};
		}
		m_LimitCount = {};
	}

	//一定フレーム立つと選択マス移動
	if (m_InputTimer[DIR_UP] == l_TargetTimer) {
		if (m_NowHeight < PANEL_HEIGHT - 1) {
			m_NowHeight++;
			m_LimitCount++;
			m_Position.z += l_Velocity;
		}
		m_InputTimer[DIR_UP] = {};
	}
	else if (m_InputTimer[DIR_DOWN] == l_TargetTimer) {
		if (m_NowHeight > 0) {
			m_NowHeight--;
			m_LimitCount++;
			m_Position.z -= l_Velocity;
		}
		m_InputTimer[DIR_DOWN] = {};
	}
	else if (m_InputTimer[DIR_RIGHT] == l_TargetTimer) {
		if (m_NowWidth < (PANEL_WIDTH / 2) - 1) {
			m_NowWidth++;
			m_LimitCount++;
			m_Position.x += l_Velocity;
		}
		m_InputTimer[DIR_RIGHT] = {};
	}
	else if (m_InputTimer[DIR_LEFT] == l_TargetTimer) {
		if (m_NowWidth > 0) {
			m_NowWidth--;
			m_LimitCount++;
			m_Position.x -= l_Velocity;
		}
		m_InputTimer[DIR_LEFT] = {};
	}

	if (input->TriggerButton(input->A) && m_AllActCount != 0) {
		m_ReturnPos = m_Position;
		_charaState = STATE_ACTION;
	}
}
void Player::SpecialAct() {
	//0番目の要素から行動を決める
	if (m_AllActCount != 0) {
		if (m_Act[0] == ACT_ATTACK) {
			Attack();
		}
		else if (m_Act[0] == ACT_GUARD) {
			Guard();
		}
		else if (m_Act[0] == ACT_SKILL) {
			SkillAct();
		}
	}
}
//攻撃
void Player::Attack() {
	m_CoolTime = {};
	BirthArea();
	FinishAct();
}
//防御
void Player::Guard() {
	m_AttackTimer++;
	if (m_AttackTimer == 50) {
		FinishAct();
	}
}
//スキル
void Player::SkillAct() {
	m_AttackTimer++;
	if (m_AttackTimer == 50) {
		FinishAct();
	}
}
//行動力を入手
void Player::AddAct(const string& Tag) {
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

	m_AllActCount++;
	BirthActUI(Tag);
}
//攻撃先指定
void Player::AttackTarget(const XMFLOAT3& pos) {
	m_TargetPos = pos;
}
//行動UIの生成
void Player::BirthActUI(const string& Tag) {
	//アクションのセット
	ActionUI* newactUi = nullptr;
	newactUi = new ActionUI();
	newactUi->Initialize();
	newactUi->InitState(m_AllActCount,Tag);
	actui.emplace_back(newactUi);
}
void Player::BirthParticle() {
	if (m_AllActCount != 0) {
		if (m_Act[0] == ACT_ATTACK) {
			ParticleEmitter::GetInstance()->FireEffect(20, m_Position, 1.0f, 0.0f, { 1.0f,0.0f,0.0f,1.0f }, { 1.0f,0.0f,0.0f,1.0f });
		}	else if (m_Act[0] == ACT_GUARD) {
			ParticleEmitter::GetInstance()->FireEffect(20, m_Position, 1.0f, 0.0f, { 0.0f,0.0f,1.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f });
		}
		else if (m_Act[0] == ACT_SKILL) {
			ParticleEmitter::GetInstance()->FireEffect(20, m_Position, 1.0f, 0.0f, { 0.0f,1.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f,1.0f });
		}
	}
}
//行動の終了
void Player::FinishAct() {
	m_Act.erase(m_Act.begin());
	m_AllActCount--;
	m_AttackTimer = {};
	actui[0]->SetUse(true);
	_charaState = STATE_MOVE;
}
//攻撃エリアの描画
void Player::BirthArea() {
	AttackArea* newarea = nullptr;
	newarea = new AttackArea();
	newarea->Initialize();
	newarea->InitState(m_NowWidth + 1, m_NowHeight);
	attackarea.push_back(newarea);
}