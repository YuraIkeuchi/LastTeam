#include "LastBomb.h"
#include "Collision.h"
#include "Player.h"
#include "Helper.h"
#include <StagePanel.h>
#include <Easing.h>
#include <ImageManager.h>
#include "GameStateManager.h"

LastBomb::LastBomb() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BULLET);
	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//初期化
bool LastBomb::Initialize() {
	m_Alive = false;
	m_Rotation.y = 270.0f;
	m_Scale = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/SupportEnemy.csv", "BOMB_DAMAGE")));
	return true;
}

void LastBomb::InitState(const XMFLOAT3& pos, const int width, const int height) {
	m_Position = pos;
	m_Scale = { 0.0f,0.0f,0.0f };
	m_AfterPos = { m_Position.x - 1.5f,m_Position.y,m_Position.z };
	if (m_BirthCount == 0) {
		m_AfterPos.x = -1.5f;
	}
	m_AddPower = {};
	m_BirthCount++;
	m_BaseScale = {};
	m_Alive = true;
	m_Hit = false;
	m_Frame = {};
	_BombState = BOMB_SET;
	m_NowWidth = width;
	m_NowHeight = height;
}
//更新
void LastBomb::Update() {
	Throw();
	//タイプによって色を一旦変えてる
	Obj_SetParam();
	if (m_BirthCount == 4) {
		m_BirthCount = {};
	}
	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };

	//敵の爆発
	//攻撃エリアの更新(実際はスキルになると思う)
	for (auto i = 0; i < explosion.size(); i++) {
		if (explosion[i] == nullptr)continue;
		explosion[i]->Update();

		if (!explosion[i]->GetAlive()) {
			explosion.erase(cbegin(explosion) + i);
		}
	}
	Collide();		//当たり判定
	//m_Rotation.y += 20.0f;
}
//描画
void LastBomb::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui描画
void LastBomb::ImGuiDraw() {
	//for (auto i = 0; i < explosion.size(); i++) {
	//	if (explosion[i] == nullptr)continue;
	//	explosion[i]->ImGuiDraw();
	//}

	ImGui::Begin("Bomb");
	for (int i = 0; i < (PANEL_WIDTH / 2); i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			ImGui::Text("Attack[%d][%d]:%d", i, j, m_Attack[i][j]);
		}
	}
	ImGui::Text("width:%d,height:%d", m_NowWidth, m_NowHeight);
	ImGui::End();
}

//当たり判定
bool LastBomb::Collide() {
	if (m_Hit) { return false; }
	if (!m_Alive) { return false; }
	if (m_Position.y >= 0.2f) { return false; }
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 0.15f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius)) {
		player->RecvDamage(m_Damage, "BOUND");
		m_Hit = true;
		return true;
	}
	else {
		return false;
	}

	return false;
}
//追従
void LastBomb::Throw() {
	if (!m_Alive) { return; }
	const int l_BaseTimer = 40;
	const float l_AddFrame = 1 / 30.0f;
	const float l_AfterScale = 0.2f;

	//弾のマスを取得する
	StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
	//弾のセット(だんだん浮かび逢ふがるような感じ)
	if (_BombState == BOMB_SET) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			_BombState = BOMB_THROW;
			m_Jump = true;
			m_AddPower = 0.3f;
		}
		m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, l_AfterScale);
	}
	//実際に狙っちゃう
	else {
		if (m_Jump) {
			m_AddPower -= m_Gravity;
			if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
				m_AddPower = {};
				m_Jump = false;
				m_Position.y = 0.1f;
				StagePanel::GetInstance()->SetClose(m_NowWidth, m_NowHeight, true);
			}
		}

		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
		}
		else {
			m_Position.x = Ease(In, Cubic, m_Frame, m_Position.x, m_AfterPos.x);
		}
	}
}

//爆弾が爆発する
void LastBomb::BirthExplosion() {
	if (!m_Alive) { return; }
	int l_SoundTimer = {};
	for (int i = 0; i < (PANEL_WIDTH / 2); i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (m_NowWidth == i || m_NowHeight == j) {
				m_Attack[i][j] = true;		//爆発する場所を決める
			}

			if (m_Attack[i][j]) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				std::unique_ptr<Explosion> newarea = std::make_unique<Explosion>();
				newarea->Initialize();
				newarea->InitState(i, j);
				newarea->SetPlayer(player);
				if (l_SoundTimer == 0) {
					newarea->SetSound(true);
				}
				l_SoundTimer++;
				explosion.emplace_back(std::move(newarea));
			}
		}
	}

	for (int i = 0; i < (PANEL_WIDTH / 2); i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_Attack[i][j] = false;
		}
	}

	StagePanel::GetInstance()->SetClose(m_NowWidth, m_NowHeight, false);
	StagePanel::GetInstance()->SetRock(m_NowWidth, m_NowHeight, false);
	m_Alive = false;
}
void LastBomb::DeleteBomb() {
	StagePanel::GetInstance()->SetClose(m_NowWidth, m_NowHeight, false);
	StagePanel::GetInstance()->SetRock(m_NowWidth, m_NowHeight, false);
	m_Alive = false;
}