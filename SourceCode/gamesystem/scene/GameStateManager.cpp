#include <GameStateManager.h>
#include <Helper.h>
#include <StagePanel.h>
#include <Input.h>
#include <Easing.h>
#include <ImageManager.h>
#include <SkillManager.h>

GameStateManager* GameStateManager::GetInstance() {
	static GameStateManager instance;
	return &instance;
}
//������
void GameStateManager::Initialize() {
	//�J�E���^�[
	m_Counter = false;
	m_CounterTimer = {};
	m_CounterScore = {};
	//�ʒu�̃X�R�A
	m_PosScore = {};
	//�O���C�Y�̃X�R�A(��Xint�ɂ���)
	m_GrazeScore = 0.0f;

	//�S�̃X�R�A
	m_AllScore = {};

	//�v�f�̑S�폜�͈�U������
	m_AllActCount = {};
	actui.clear();
	m_Act.clear();
	attackarea.clear();

	skillUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,600.f }, { 1.f,1.f,1.f,1.f }, { 0.5f,1.f });
	skillUI->SetSize(basesize);
	gaugeUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,600.f }, { 0.f,1.f,0.f,1.f }, { 0.5f,1.f });
	gaugeUI->SetSize({ basesize.x,0.f });
}
//�X�V
void GameStateManager::Update() {
	Input* input = Input::GetInstance();
	const int l_AddCounterScore = 10;
	m_AllScore = m_CounterScore + (int)(m_PosScore) + (int)(m_GrazeScore);

	//�J�E���^�[�̏���
	if (m_Counter) {
		if (m_CounterTimer == 0) {		//�J�E���^�[�̃X�R�A�ɉ��Z
			m_CounterScore += l_AddCounterScore;
		}
		m_CounterTimer++;

		if (Helper::GetInstance()->CheckMin(m_CounterTimer, 20, 1)) {		//���t���[���ŃJ�E���^�[�I��
			m_Counter = false;
			m_CounterTimer = {};
		}
	}

	//��ɓ��ꂽ�X�L����UI�̍X�V
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->SetActCount(i);
		actui[i]->Update();

		if (!actui[i]->GetAlive()) {
			actui.erase(cbegin(actui) + i);
		}
	}

	//�U���G���A�̍X�V(���ۂ̓X�L���ɂȂ�Ǝv��)
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Update();

		if (!attackarea[i]->GetAlive()) {
			attackarea.erase(cbegin(attackarea) + i);
		}
	}

	GaugeUpdate();

	//�X�L������ȏ゠������X�L���g����
	if (input->TriggerButton(input->A) && m_AllActCount != 0 && !actui[0]->GetUse()) {
		UseSkill();
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/SkillUse.wav", 0.3f);
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
//�`��
void GameStateManager::ImGuiDraw() {
	//ImGui::Begin("GameState");
	//
	///*if (ImGui::Button("NORMALSKILL", ImVec2(50, 50))) {
	//	_SkillType = SKILL_NORMAL;
	//}
	//if (ImGui::Button("STRONGSKILL", ImVec2(50, 50))) {
	//	_SkillType = SKILL_STRONG;
	//}
	//if (ImGui::Button("SPECIALSKILL", ImVec2(50, 50))) {
	//	_SkillType = SKILL_SPECIAL;
	//}*/
	//
	///*
	//ImGui::End();
	if (!m_Act.empty()) {
		for (auto i = 0; i < m_Act.size(); i++) {
			ImGui::Text("Act[%d]:%d", i, m_Act[i].ActID);
		}
	}
	StagePanel::GetInstance()->ImGuiDraw();
}
//��ɓ��ꂽUI�̕`��
void GameStateManager::ActUIDraw() {
	IKESprite::PreDraw();
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->Draw();
	}
	IKESprite::PostDraw();
}
//�X�L�������(InterActionCPP�Ŏg���Ă܂�)
void GameStateManager::AddSkill(const int ID) {
	ActState act;
	act.ActID = ID;
	m_Act.push_back(act);
	//��ɓ��ꂽ�X�L���̑��������Z����
	m_AllActCount++;
	BirthActUI(ID);//UI���������
}
//�X�L��UI�̐���
void GameStateManager::BirthActUI(const int ID) {
	//�A�N�V����UI�̃Z�b�g
	ActionUI* newactUi = nullptr;
	newactUi = new ActionUI();
	newactUi->Initialize();
	newactUi->InitState(m_AllActCount);
	newactUi->SetID(ID);
	actui.emplace_back(newactUi);

	Audio::GetInstance()->PlayWave("Resources/Sound/SE/cardget.wav", 0.3f);
}
//�U���G���A�̕`��(������菈��)
void GameStateManager::BirthArea() {
	int l_BirthNumX = {};//�p�l���̃}�b�N�X��

	int l_BirthCountX = {};
	int l_BirthCountZ = {};

	if (_SkillType == SKILL_NORMAL) {		//���ʂɈ�E
		l_BirthCountX = m_NowWidth + 1;
		AttackArea* newarea = nullptr;
		newarea = new AttackArea();
		newarea->Initialize();
		newarea->InitState(l_BirthCountX, m_NowHeight);
		attackarea.push_back(newarea);
	}
	else if (_SkillType == SKILL_STRONG) {		//�v���C���[�̈ꂩ��E���S��
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
	else {				//�v���C���[����3 * 2�̃}�X
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
//�v���C���[�̌��݃p�l��
void GameStateManager::PlayerNowPanel(const int NowWidth, const int NowHeight) {
	m_NowWidth = NowWidth, m_NowHeight = NowHeight;
}
//�X�L���̎g�p
void GameStateManager::UseSkill() {
	BirthArea();
	FinishAct();
}
//�s���̏I��
void GameStateManager::FinishAct() {
	m_Act.erase(m_Act.begin());
	m_AllActCount--;
	actui[0]->SetUse(true);
}

void GameStateManager::GaugeUpdate() {
	
		m_GaugeCount++;
		if (m_GaugeCount == kGaugeCountMax) {
			StagePanel::GetInstance()->ResetAction();
			StagePanel::GetInstance()->ResetPanel();
			//�p�l���u����
			int panel_num = 3;
			SkillManager::GetInstance()->ResetBirth();
			StagePanel::GetInstance()->RandomPanel(panel_num);
			m_GaugeCount = 0;
		}
		float per = (m_GaugeCount / kGaugeCountMax);
		float size = Ease(In, Quad, 0.5f, gaugeUI->GetSize().y, basesize.y * per);
		gaugeUI->SetSize({ basesize.x,size });
}
