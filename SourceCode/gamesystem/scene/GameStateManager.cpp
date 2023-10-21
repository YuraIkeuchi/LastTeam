#include <GameStateManager.h>
#include <Helper.h>
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
}
//�X�V
void GameStateManager::Update() {
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
}
//�`��
void GameStateManager::ImGuiDraw() {
	ImGui::Begin("GameState");
	ImGui::Text(m_Counter ? "CLEAR" : "NONE");
	ImGui::Text("CounterTimer:%d", m_CounterTimer);
	ImGui::Text("CounteScore:%d", m_CounterScore);
	ImGui::Text("PosScore:%d", (int)(m_PosScore));
	ImGui::Text("Graze:%d", (int)m_GrazeScore);
	ImGui::Text("All:%d", m_AllScore);
	ImGui::End();
}