#include "StagePanel.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Player.h"
#include "GameMode.h"
#include "Collision.h"
#include "ImageManager.h"
#include <Easing.h>

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
	actions.clear();
	skillUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,600.f }, { 1.f,1.f,1.f,1.f }, { 0.5f,1.f });
	skillUI->SetSize(basesize);
	gaugeUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,600.f }, { 0.f,1.f,0.f,1.f }, { 0.5f,1.f });
	gaugeUI->SetSize({ basesize.x,0.f });
	//CSV読み込み
	return true;
}

//更新処理
void StagePanel::Update() {
	//if (GameMode::GetInstance()->GetGameTurn() == TURN_BATTLE) {
	BattleUpdate();
	//}
	{
		gaugeCount++;
		if (gaugeCount == kGaugeCountMax) {
			for (auto i = 0; i < actions.size(); i++) {
				actions[i]->SetState(STATE_VANISH);
			}
			ResetPanel();
			//パネル置く数
			int panel_num = 3;
			RandomPanel(panel_num);
			gaugeCount = 0;
		}
		float per = (gaugeCount / kGaugeCountMax);
		float size = Ease(In,Quad,0.5f, gaugeUI->GetSize().y,basesize.y * per);
		gaugeUI->SetSize({ basesize.x,size });
	}
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
	IKESprite::PreDraw();
	skillUI->Draw();
	gaugeUI->Draw();
	IKESprite::PostDraw();
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

//スキルセットの更新(バトル前)
void StagePanel::SetUpdate() {
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
					if (!panels[i][j].isEnemyHit) {
						panels[i][j].color = { 1.0f,1.0f,1.0f,1.0f };
					}
					else {
						panels[i][j].color = { 0.5f,0.0f,0.0f,1.0f };
					}
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
		//パネル探索（開いてるのが3追加の場合書いてない）
		while (!isSet) {
			if (panels[width][height].type != NO_PANEL) {
				width = Helper::GetInstance()->GetRanNum(0, 3);
				height = Helper::GetInstance()->GetRanNum(0, 3);
			} else {
				isSet = true;
			}
		}
		//これは変えなくていい
		int r_type = Helper::GetInstance()->GetRanNum(1, 3);

		panels[width][height].type = SKILL_PANEL;
		//アクションのセット
		InterAction* newAction = nullptr;
		switch (panels[width][height].type) {
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

void StagePanel::ResetPanel() {
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			panels[i][j].color = { 1,1,1,1 };
			panels[i][j].isHit = false;
			panels[i][j].type = NO_PANEL;
		}
	}
}

void StagePanel::SetEnemyHit(IKEObject3d* obj,int& wight, int& height) {
	m_OBB1.SetParam_Pos(obj->GetPosition());
	m_OBB1.SetParam_Rot(obj->GetMatrot());
	m_OBB1.SetParam_Scl(obj->GetScale());
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_OBB2.SetParam_Pos(panels[i][j].position);
			m_OBB2.SetParam_Rot(panels[i][j].object->GetMatrot());
			m_OBB2.SetParam_Scl({ 0.5f,1.0f,0.5f });
			if ((Collision::OBBCollision(m_OBB1, m_OBB2))) {
				panels[i][j].isEnemyHit = true;
				wight = i;
				height = j;
			}
			else {
				panels[i][j].isEnemyHit = false;
			}
		}
	}
}