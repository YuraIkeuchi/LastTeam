#include "StagePanel.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Player.h"
#include "Collision.h"
#include "ImageManager.h"
#include <Easing.h>
#include <SkillManager.h>

#include "GameStateManager.h"
Player* StagePanel::player = nullptr;
StagePanel* StagePanel::GetInstance() {
	static StagePanel instance;

	return &instance;
}
//リソース読み込み
void StagePanel::LoadResource() {
	float scale = PANEL_SIZE * 0.1f;
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			panels[i][j].object = make_unique<IKEObject3d>();
			panels[i][j].object->Initialize();
			panels[i][j].object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
			panels[i][j].object->SetScale({ PANEL_SIZE,0.01f,PANEL_SIZE });
			panels[i][j].line = make_unique<IKETexture>(ImageManager::PANNELLINE,XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,1.f,1.f,1.f });
			panels[i][j].line->TextureCreate();
			panels[i][j].line->Initialize();
			panels[i][j].line->SetScale({ scale ,scale ,scale });
			panels[i][j].line->SetRotation({ 90.0f,0.0f,0.0f });
		}
	}
	
}
//初期化
bool StagePanel::Initialize(const float PosY) {
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			panels[i][j].position = { (PANEL_SIZE * i) - (PANEL_HEIGHT * PANEL_SIZE),PosY,(PANEL_SIZE * j) };
			panels[i][j].color = { 1.f,1.f,1.f,1.f };
			panels[i][j].type = NO_PANEL;
			panels[i][j].isHit = false;
			panels[i][j].isPoison = false;
			panels[i][j].PoisonTimer = {};
			panels[i][j].Frame = {};
			panels[i][j].TargetTimer = (i * 5) + (j * 40);
		}
	}
	m_SelectHeight = 0;
	m_SelectWidth = 0;
	if (!actions.empty()) {
		actions.clear();
	}
	m_CreateTimer = {};
	m_CreateFinish = false;
	onomatope = make_unique<Onomatope>();
	//CSV読み込み
	return true;
}

//更新処理
void StagePanel::Update() {
	BattleUpdate();
	PoisonUpdate();
	for (auto i = 0; i < actions.size(); i++) {
		if (actions[i] == nullptr)continue;
		actions[i]->Update();
		if (actions[i]->GetAlive() &&
			actions[i]->GetDelete()) {
			if(actions[i]->GetDiscard()){
			GameStateManager::GetInstance()->GetDiscardSkill(actions[i]->GetSkillID());
			}
			m_ActionCount--;
			actions[i]->SetAlive(false);
		}
	}
	auto result = std::remove_if(actions.begin(), actions.end(),
		[](unique_ptr<InterAction>& act) { return act->GetDelete(); });
	actions.erase(result, actions.end());
	if (m_ActionCount == 0) {
		m_AllDelete = true;
	}
	else {
		m_AllDelete = false;
	}
}
//ステージ形成
void StagePanel::CreateStage() {
	if (m_CreateFinish) { return; }
	const float l_AddFrame = 1 / 45.0f;
	m_CreateTimer++;

	if (m_CreateTimer % 30 == 0) {
		XMFLOAT2 l_RandPos = {};
		l_RandPos.x = (float)Helper::GetRanNum(100, 1000);
		l_RandPos.y = (float)Helper::GetRanNum(100, 500);
		onomatope->AddOnomato(BossSpawn, l_RandPos);
	}
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (m_CreateTimer >= panels[i][j].TargetTimer) {
				if (Helper::FrameCheck(panels[i][j].Frame, l_AddFrame)) {
					panels[i][j].Frame = 1.0f;
				}

				panels[i][j].position.y = Ease(In, Cubic, panels[i][j].Frame, panels[i][j].position.y, 0.0f);
			}
		}
	}

	if (panels[PANEL_WIDTH - 1][PANEL_HEIGHT - 1].Frame == 1.0f) {
		m_CreateFinish = true;
	}

	onomatope->Update();
}

//描画
void StagePanel::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			panels[i][j].object->Draw();
		}
	}
	IKEObject3d::PostDraw();
	IKETexture::PreDraw2(dxCommon, AddBlendType);
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			panels[i][j].line->Draw();
		}
	}
	IKETexture::PostDraw();
}
//カードの描画
void StagePanel::ActDraw(DirectXCommon* dxCommon) {
	for (auto i = 0; i < actions.size(); i++) {
		if (actions[i] == nullptr)continue;
		actions[i]->Draw(dxCommon);
	}
}
//ImGui
void StagePanel::ImGuiDraw() {
	for (auto i = 0; i < actions.size(); i++) {
		if (actions[i] == nullptr)continue;
		actions[i]->ImGuiDraw();
	}

	/*ImGui::Begin("Panel");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			ImGui::Text("Close[%d][%d]:%d",i,j ,panels[i][j].isClose);
		}
	}*/
}
//オノマトペの描画
void StagePanel::OnomatoDraw() {
	if (m_CreateFinish) { return; }
	IKESprite::PreDraw();
	onomatope->Draw();
	IKESprite::PostDraw();
}
//スキルセットの更新(バトル前)
void StagePanel::SetUpdate() {	
}
//バトルの更新
void StagePanel::BattleUpdate() {
	//敵がが居るマスが赤くなる
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
			panels[i][j].line->Update();
			panels[i][j].line->SetPosition({ panels[i][j].position.x, panels[i][j].position.y+0.05f, panels[i][j].position.z });
			if (i < PANEL_WIDTH / 2) {
				panels[i][j].line->SetColor({ 0.f,0.f,1.f,1.f });
			} else {
				panels[i][j].line->SetColor({ 1.f,0.f,0.f,1.f });
			}
		}
	}
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
bool StagePanel::AllCleanCheack() {
	if (actions.size()>0) {
		return false;
	}
	return true;
}
//パネルをセットする
void StagePanel::RandomPanel(int num) {
	
	int freeNum = 0;
	int p_height = player->GetNowHeight();
	int p_width = player->GetNowWidth();

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
		int width = Helper::GetRanNum(0, 3);
		int height = Helper::GetRanNum(0, 3);

		//パネル探索（開いてるのが3追加の場合書いてない）

		while (!isSet) {
			if (panels[width][height].type != NO_PANEL || panels[width][height].isEnemyHit ||
				(width == p_width && height == p_height)) {
				width = Helper::GetRanNum(0, 3);
				height = Helper::GetRanNum(0, 3);
			} else {
				isSet = true;
			}
		}
		//これは変えなくていい
		int r_type = Helper::GetRanNum(1, 3);

		panels[width][height].type = SKILL_PANEL;
		//アクションのセット
		InterAction* newAction = nullptr;
		switch (panels[width][height].type) {
		case SKILL_PANEL:
			newAction = new SkillAction(SkillManager::GetInstance()->IDSearch(i));
			break;
		default:
			break;
		}
		newAction->Initialize();
		newAction->SetPlayer(player);
		//ステージに配布されるパネルに情報を読み取ってる
		newAction->GetSkillData();
		newAction->SetPosition({ panels[width][height].position.x,0.5f,panels[width][height].position.z });
		actions.emplace_back(newAction);

		panels[width][height].object->Update();
		panels[width][height].object->SetPosition(panels[width][height].position);
		panels[width][height].object->SetColor(panels[width][height].color);
		m_ActionCount++;
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
//敵とパネルの当たり判定
void StagePanel::SetEnemyHit(IKEObject3d* obj, int& width, int& height, bool m_Alive) {

	m_OBB1.SetParam_Pos(obj->GetPosition());
	m_OBB1.SetParam_Rot(obj->GetMatrot());
	m_OBB1.SetParam_Scl(obj->GetScale());
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_OBB2.SetParam_Pos(panels[i][j].position);
			m_OBB2.SetParam_Rot(panels[i][j].object->GetMatrot());
			m_OBB2.SetParam_Scl({ 0.5f,1.0f,0.5f });
			if ((Collision::OBBCollision(m_OBB1, m_OBB2))) {
				width = i;
				height = j;
				if (m_Alive) {
					panels[i][j].isEnemyHit = true;
				}
				else {
					panels[i][j].isEnemyHit = false;
				}
			}
		}
	}
}
void StagePanel::ClosePanel(IKEObject3d* obj, bool Alive) {
	m_OBB1.SetParam_Pos(obj->GetPosition());
	m_OBB1.SetParam_Rot(obj->GetMatrot());
	m_OBB1.SetParam_Scl(obj->GetScale());
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_OBB2.SetParam_Pos(panels[i][j].position);
			m_OBB2.SetParam_Rot(panels[i][j].object->GetMatrot());
			m_OBB2.SetParam_Scl({ 0.5f,1.0f,0.5f });
			if ((Collision::OBBCollision(m_OBB1, m_OBB2))) {
				if (Alive) {
					panels[i][j].isClose = true;
				}
				else {
					panels[i][j].isClose = false;
				}
			}
		}
	}
}
//敵の弾とパネルの当たり判定
void StagePanel::SetPanelSearch(IKEObject3d* obj, int& width, int& height) {
	float l_BaseScale = 0.4f;
	m_OBB1.SetParam_Pos({ obj->GetPosition().x, 0.0f, obj->GetPosition().z });
	m_OBB1.SetParam_Rot(obj->GetMatrot());
	m_OBB1.SetParam_Scl({ l_BaseScale,l_BaseScale,l_BaseScale });
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_OBB2.SetParam_Pos({ panels[i][j].position.x, 0.0f, panels[i][j].position.z });
			m_OBB2.SetParam_Rot(panels[i][j].object->GetMatrot());
			m_OBB2.SetParam_Scl({ 0.5f,1.0f,0.5f });
			if ((Collision::OBBCollision(m_OBB1, m_OBB2))) {
				width = i;
				height = j;
			}
		}
	}
}
//パネルの色を決める
XMFLOAT4 StagePanel::ChangeColor(const int Widht, const int Height) {
	XMFLOAT4 color;
	if (Widht % 2 == 0) {
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
void StagePanel::EnemyHitReset() {
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			panels[i][j].isEnemyHit = false;
		}
	}
}
XMFLOAT3 StagePanel::EnemySetPanel(const bool LastEnemy) {
	bool isSet = false;
	//乱数の設定
	int width = 0;
	if (!LastEnemy) {
		width = Helper::GetRanNum(4, 7);
	}
	else {		//最後の敵はなるべく前にワープさせる(ぐだらないように)
		width = Helper::GetRanNum(4, 5);
	}
	int height = Helper::GetRanNum(0, 3);

	//パネル探索（敵がいる場合は再検索）

	while (!isSet) {
		if (panels[width][height].isEnemyHit) {
			if (!LastEnemy) {
				width = Helper::GetRanNum(4, 7);
			}
			else {		//最後の敵はなるべく前にワープさせる(ぐだらないように)
				width = Helper::GetRanNum(4, 5);
			}
			height = Helper::GetRanNum(0, 3);
		}
		else {
			isSet = true;
		}
	}

	return SetPositon(width, height);
}
XMFLOAT3 StagePanel::FrontPlayerSetPanel() {
	return SetPositon(player->GetNowWidth() + 1, player->GetNowHeight());		//プレイヤーの正面
}
void StagePanel::PoisonSetPanel(int& width, int& height) {
	bool isSet = false;
	//乱数の設定
	int l_width = Helper::GetRanNum(0, 3);
	int l_height = Helper::GetRanNum(0, 3);

	//パネル探索（敵がいる場合は再検索）

	while (!isSet) {
		if (panels[l_width][l_height].isPoison) {
			l_width = Helper::GetRanNum(0, 3);
			l_height = Helper::GetRanNum(0, 3);
		}
		else {
			panels[l_width][l_height].isPoison = true;
			width = l_width;
			height = l_height;
			isSet = true;
		}
	}
}
void StagePanel::PoisonUpdate() {
	const int l_TargetTimer = 500;
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (panels[i][j].isPoison) {
				panels[i][j].PoisonTimer++;

				if (Helper::CheckMin(panels[i][j].PoisonTimer, l_TargetTimer, 1)) {
					panels[i][j].isPoison = false;
					panels[i][j].PoisonTimer = {};
				}
			}
		}
	}
}
void StagePanel::DeleteAction() {
	m_ActionCount = {};
	actions.clear();
}