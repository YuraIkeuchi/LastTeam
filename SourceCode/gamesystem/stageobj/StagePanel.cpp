#include "StagePanel.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Player.h"
#include "Collision.h"
#include "ImageManager.h"
#include <Easing.h>
#include <SkillManager.h>

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

	if (actions.size() == 0) {
		m_AllDelete = true;
	}
	else {
		m_AllDelete = false;
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
	//for (auto i = 0; i < actions.size(); i++) {
	//	if (actions[i] == nullptr)continue;
	//	actions[i]->ImGuiDraw();
	//}

	ImGui::Begin("Panel");
	//プレイヤーが居るマスが黄色くなる
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			ImGui::Text("Hit[%d][%d]:%d", i, j, panels[i][j].isEnemyHit);
		}
	}
	ImGui::End();
}

//スキルセットの更新(バトル前)
void StagePanel::SetUpdate() {
}

//バトルの更新
void StagePanel::BattleUpdate() {
	//if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	//	RandomPanel(3);
	//}

	//プレイヤーが居るマスが黄色くなる
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
				if (!panels[i][j].predict) {
					panels[i][j].color = ChangeColor(i, j);
				}
				else {
					panels[i][j].color = { 1.0f,0.3f,0.0f,1.0f };
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
	int freeNum = 0;
	int p_height = Player::GetInstance()->GetNowHeight();
	int p_width = Player::GetInstance()->GetNowWidth();

	for (int i = 0; i < PANEL_WIDTH/2; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (panels[i][j].type != NO_PANEL ||
				(i == p_width && j == p_height)) {
				continue;
			}
			freeNum++;
		}
	}
	if (freeNum == 0) {
		return;
	}
	if (freeNum < num) {
		num = freeNum;
	}

	for (int i = 0; i < num; i++) {
		bool isSet = false;
		//乱数の設定
		int width = Helper::GetInstance()->GetRanNum(0, 3);
		int height = Helper::GetInstance()->GetRanNum(0, 3);

		//パネル探索（開いてるのが3追加の場合書いてない）

		while (!isSet) {
			if (panels[width][height].type != NO_PANEL ||
				(width == p_width && height == p_height)) {
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
		//ステージに配布されるパネルに情報を読み取ってる
		newAction->SetSkillID(SkillManager::GetInstance()->GetID(i));
		newAction->GetSkillData();
		newAction->SetPosition({ panels[width][height].position.x,0.5f,panels[width][height].position.z });
		actions.emplace_back(newAction);

		panels[width][height].object->Update();
		panels[width][height].object->SetPosition(panels[width][height].position);
		panels[width][height].object->SetColor(panels[width][height].color);
	}

	SkillManager::GetInstance()->SetDeckState(SkillManager::GetInstance()->GetDeckNum() - num);
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

void StagePanel::ResetAction() {
	for (auto i = 0; i < actions.size(); i++) {
		actions[i]->SetState(STATE_VANISH);
	}
}
//敵のをパネルの当たり判定
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
//パネルの色を決める
XMFLOAT4 StagePanel::ChangeColor(const int Weight, const int Height) {
	XMFLOAT4 color;
	if (Weight % 2 == 0) {
		if (Height % 2 == 0) {
			color = { 1.0f,0.9f,0.7f,1.0f };
		}
		else {
			color = { 0.5f,0.3f,0.1f,1.0f };
		}
	}
	else {
		if (Height % 2 != 0) {
			color = { 1.0f,0.9f,0.7f,1.0f };
		}
		else {
			color = { 0.5f,0.3f,0.1f,1.0f };
		}
	}

	return color;
}
////チェック
//bool StagePanel::SetPredict(int width, int height, bool Frag) {
//	panels[width][height].predict = Frag;
//}