#include "StagePanel.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Player.h"
#include "GameMode.h"
#include "Collision.h"
StagePanel* StagePanel::GetInstance() {
	static StagePanel instance;

	return &instance;
}
//���\�[�X�ǂݍ���
void StagePanel::LoadResource() {
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			panels[i][j].object.reset(new IKEObject3d());
			panels[i][j].object->Initialize();
			panels[i][j].object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
			panels[i][j].object->SetScale({ 2.f,0.1f,2.f });
		}
	}
}
//������
bool StagePanel::Initialize() {
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			panels[i][j].position = { (2.0f * i) - (PANEL_HEIGHT * 2.0f),0.0f,(2.0f * j) };
			panels[i][j].color = { 1.f,1.f,1.f,1.f };
			panels[i][j].type = NO_PANEL;
			panels[i][j].isHit = false;
		}
	}
	m_SelectHeight = 0;
	m_SelectWidth = 0;
	actions.clear();
	//CSV�ǂݍ���
	return true;
}

//�X�V����
void StagePanel::Update() {
	//if (GameMode::GetInstance()->GetGameTurn() == TURN_BATTLE) {
		BattleUpdate();
	//}

	for (auto i = 0; i < actions.size(); i++) {
		if (actions[i] == nullptr)continue;
		actions[i]->Update();

		if (!actions[i]->GetAlive()) {
			actions.erase(cbegin(actions) + i);
		}
	}
}

//�`��
void StagePanel::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			panels[i][j].object->Draw();
		}
	}
	for (auto i = 0; i < actions.size(); i++) {
		if (actions[i] == nullptr)continue;
		actions[i]->Draw(dxCommon);
	}
	IKEObject3d::PostDraw();
}

//ImGui
void StagePanel::ImGuiDraw() {
}

//�X�L���Z�b�g�̍X�V(�o�g���O)
void StagePanel::SetUpdate() {
}

//�o�g���̍X�V
void StagePanel::BattleUpdate() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		RandomPanel(3);
	}

	//�v���C���[������}�X�����F���Ȃ�
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (!panels[i][j].isHit) {
				if (panels[i][j].type == NO_PANEL) {
					panels[i][j].color = { 1.0f,1.0f,1.0f,1.0f };
				} else {
					panels[i][j].color = { 0.5f,0.5f,0.5f,1.0f };
				}
			} else {
				panels[i][j].color = { 0.8f,0.8f,0.0f,1.0f };
			}
			panels[i][j].object->Update();
			panels[i][j].object->SetPosition(panels[i][j].position);
			panels[i][j].object->SetColor(panels[i][j].color);
		}
	}

	Collide();
}
//�p�l���̏���
void StagePanel::DeletePanel() {
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (panels[i][j].isHit) {
				panels[i][j].type = NO_PANEL;
			}
		}
	}
}

void StagePanel::Collide() {
	m_OBB1.SetParam_Pos(Player::GetInstance()->GetPosition());
	m_OBB1.SetParam_Rot(Player::GetInstance()->GetMatrot());
	m_OBB1.SetParam_Scl(Player::GetInstance()->GetScale());
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_OBB2.SetParam_Pos(panels[i][j].position);
			m_OBB2.SetParam_Rot(panels[i][j].object->GetMatrot());
			m_OBB2.SetParam_Scl({ 0.5f,1.0f,0.5f });
			if ((Collision::OBBCollision(m_OBB1, m_OBB2))) {
				panels[i][j].isHit = true;
			} else {
				panels[i][j].isHit = false;
			}
		}
	}
}


void StagePanel::RandomPanel(int num) {
	for (int i = 0; i < num; i++) {
		bool isSet = false;
		//�����̐ݒ�
		int width = Helper::GetInstance()->GetRanNum(0, 3);
		int height = Helper::GetInstance()->GetRanNum(0, 3);
		//�p�l���T���i�J���Ă�̂�3�ǉ��̏ꍇ�����ĂȂ��j
		while (!isSet) {
			if (panels[width][height].type!= NO_PANEL) {
				width = Helper::GetInstance()->GetRanNum(0, 3);
				height = Helper::GetInstance()->GetRanNum(0, 3);
			} else {
				isSet = true;
			}
		}
		//����͕ς��Ȃ��Ă���
		int r_type= Helper::GetInstance()->GetRanNum(1, 3);
		
		panels[width][height].type = r_type;
		//�A�N�V�����̃Z�b�g
		InterAction* newAction = nullptr;
		switch (panels[width][height].type) {
		case ATTACK_PANEL:
			newAction = new AttackAction();
			break;
		case GUARD_PANEL:
			newAction = new GuardAction();
			break;
		case SKILL_PANEL:
			newAction = new SkillAction();
			break;
		default:
			break;
		}
		newAction->Initialize();
		newAction->SetPosition({ panels[width][height].position.x,0.5f,panels[width][height].position.z });
		actions.emplace_back(newAction);
		
		panels[width][height].object->Update();
		panels[width][height].object->SetPosition(panels[width][height].position);
		panels[width][height].object->SetColor(panels[width][height].color);
	}
}
