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
//リソース読み込み
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
//初期化
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

	//CSV読み込み
	return true;
}

//更新処理
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

//描画
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
	//ImGui::Begin("Panel");
	//ImGui::Text("Can:%d", m_CanSet);
	//ImGui::Text("Height:%d", m_SelectHeight);
	//ImGui::Text("Width:%d", m_SelectWidth);
	//for (int i = 0; i < PANEL_WIDTH; i++) {
	//	for (int j = 0; j < PANEL_HEIGHT; j++) {
	//		ImGui::Text("PanelType[%d][%d]:%d", i, j, m_PanelType[i][j]);
	//	}
	//}
	//ImGui::End();
}

//スキルセットの更新(バトル前)
void StagePanel::SetUpdate() {
	const int l_TimerMax = 10;
	Input* input = Input::GetInstance();
	//スティックでマスを選ぶ
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f)) {
		if (input->TiltPushStick(Input::L_UP, 0.0f) && m_SelectHeight < PANEL_HEIGHT - 1) {
			m_Timer[DIR_UP]++;
		} else if (input->TiltPushStick(Input::L_DOWN, 0.0f) && m_SelectHeight > 0) {
			m_Timer[DIR_DOWN]++;
		} else if (input->TiltPushStick(Input::L_RIGHT, 0.0f) && m_SelectWidth < PANEL_WIDTH - 1) {
			m_Timer[DIR_RIGHT]++;
		} else if (input->TiltPushStick(Input::L_LEFT, 0.0f) && m_SelectWidth > 0) {
			m_Timer[DIR_LEFT]++;
		}
	} else {
		for (int i = 0; i < DIR_MAX; i++) {
			m_Timer[i] = {};
		}
	}

	//一定フレーム立つと選択マス移動
	if (m_Timer[DIR_UP] == l_TimerMax) {
		m_SelectHeight++;
		m_Timer[DIR_UP] = {};
	} else if (m_Timer[DIR_DOWN] == l_TimerMax) {
		m_SelectHeight--;
		m_Timer[DIR_DOWN] = {};
	} else if (m_Timer[DIR_RIGHT] == l_TimerMax) {
		m_SelectWidth++;
		m_Timer[DIR_RIGHT] = {};
	} else if (m_Timer[DIR_LEFT] == l_TimerMax) {
		m_SelectWidth--;
		m_Timer[DIR_LEFT] = {};
	}

	//選択マスの色が変わる
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (panels[i][j].type == NO_PANEL) {
				if (m_SelectHeight == j && m_SelectWidth == i) {
					panels[i][j].color = { 0.8f,0.8f,0.0f,1.0f };
				} else {
					panels[i][j].color = { 1.0f,1.0f,1.0f,1.0f };
				}
			} else {
				panels[i][j].color = { 0.5f,0.5f,0.5f,1.0f };
			}
			panels[i][j].object->Update();
			panels[i][j].object->SetPosition(panels[i][j].position);
			panels[i][j].object->SetColor(panels[i][j].color);
		}
	}

	//セレクトしているものの座標
//m_SelectPos = panels[m_SelectWidth][m_SelectHeight].position;

	//パネルを置けるかどうかをチェックする
	//if ((panels[m_SelectWidth][m_SelectHeight].type == NO_PANEL) &&
	//	!panels[m_SelectWidth][m_SelectHeight].isHit) {
	//	m_CanSet = true;
	//} else {
	//	m_CanSet = false;
	//}
}

//バトルの更新
void StagePanel::BattleUpdate() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		RandomPanel(3);
	}

	//プレイヤーが居るマスが黄色くなる
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
//パネルの除去
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
		//乱数の設定
		int width = Helper::GetInstance()->GetRanNum(0, 3);
		int height = Helper::GetInstance()->GetRanNum(0, 3);
		while (!isSet) {
			if (panels[width][height].type!= NO_PANEL) {
				width = Helper::GetInstance()->GetRanNum(0, 3);
				height = Helper::GetInstance()->GetRanNum(0, 3);
			} else {
				isSet = true;
			}
		}
		int r_type= Helper::GetInstance()->GetRanNum(1, 3);
		
		panels[width][height].type = r_type;
		//アクションのセット
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
