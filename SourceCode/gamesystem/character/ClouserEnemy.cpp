#include "ClouserEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
#include <StagePanel.h>
//モデル読み込み
ClouserEnemy::ClouserEnemy() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	/*shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });*/
	//予測
	predictarea.reset(new PredictArea("ENEMY"));
	predictarea->Initialize();
}
//初期化
bool ClouserEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_Color = { 0.0f,0.0f,1.0f,1.0f };
	m_Scale = { 0.4f,0.4f,0.4f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ClouserEnemy.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ClouserEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/ClouserEnemy.csv", m_Limit, "Interval");

	m_MaxHP = m_HP;
	m_CheckPanel = true;
	magic.Alive = false;
	magic.Frame = {};
	magic.Scale = {};
	magic.AfterScale = 0.2f;
	magic.Pos = {};
	magic.State = {};

	enemywarp.AfterScale = {};
	enemywarp.Scale = 0.5f;
	m_AddDisolve = 2.0f;
	return true;
}
//状態遷移
void (ClouserEnemy::* ClouserEnemy::stateTable[])() = {
	&ClouserEnemy::Inter,//動きの合間
	&ClouserEnemy::Attack,//動きの合間
	&ClouserEnemy::Teleport,//瞬間移動
};

//行動
void ClouserEnemy::Action() {
	if (!m_Induction) {
		(this->*stateTable[_charaState])();
	}
	else {
		InductionMove();
	}
	Obj_SetParam();
	//当たり判定
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定
	PoisonState();//毒
	BirthMagic();//魔法陣
	AttackMove();//攻撃時の動き
	//攻撃時ジャンプする
	if (m_Jump) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
			m_AddPower = {};
			m_Jump = false;
			m_Position.y = 0.1f;
		}
	}

	//攻撃エリアの更新(実際はスキルになると思う)
	for (auto i = 0; i < enerock.size(); i++) {
		if (enerock[i] == nullptr)continue;
		enerock[i]->Update();

		if (!enerock[i]->GetAlive()) {
			enerock.erase(cbegin(enerock) + i);
		}
	}


	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	/*shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();*/

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();
}

//描画
void ClouserEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	if (m_SuperPoison) { poison_tex->Draw(); }
	if (m_HealDamage) { healdamage_tex->Draw(); }
	IKETexture::PostDraw();
	for (auto i = 0; i < enerock.size(); i++) {
		if (enerock[i] == nullptr)continue;
		enerock[i]->Draw(dxCommon);
	}
	predictarea->Draw(dxCommon);
	if (m_Color.w != 0.0f)
		Obj_Draw();
}
//ImGui描画
void ClouserEnemy::ImGui_Origin() {
	/*ImGui::Begin("Area");
	ImGui::Text("Height:%d", m_NowWidth);
	ImGui::Text("Induction:%d", m_Induction);
	ImGui::Text("InductionFrame:%f", m_InductionFrame);
	ImGui::Text("InductionPos:%f", m_InductionPos);
	ImGui::End();
	predictarea->ImGuiDraw();*/
	for (auto i = 0; i < enerock.size(); i++) {
		if (enerock[i] == nullptr)continue;
		enerock[i]->ImGui_Origin();
	}
}
//開放
void ClouserEnemy::Finalize() {

}
//待機
void ClouserEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
	}
}
//攻撃
void ClouserEnemy::Attack() {
	//プレイヤーの現在マス
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];
	if (coolTimer == 0) {
		//プレイヤーからの距離(-1~1)
		int l_RandWigth = Helper::GetRanNum(-1, 1);
		int l_RandHeight = Helper::GetRanNum(-1, 1);
		m_RandWigth = l_PlayerWidth + l_RandWigth;
		m_RandHeight = l_PlayerHeight + l_RandHeight;
		Helper::Clamp(m_RandWigth, 0, 3);
		Helper::Clamp(m_RandHeight, 0, 3);
		BirthPredict(m_RandWigth, m_RandHeight);
	}
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		BirthArea(m_RandWigth, m_RandHeight);
		coolTimer = {};
		m_AttackCount++;
		m_Jump = true;
		m_AddPower = 0.2f;
		m_Rot = true;
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
	}
	predictarea->SetTargetTimer(l_TargetTimer);
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//ワープ
void ClouserEnemy::Teleport() {
	const int l_RandTimer = Helper::GetRanNum(0, 30);
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL - 1];

	if (Helper::CheckMin(coolTimer, l_TargetTimer + l_RandTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
	}
}

//攻撃エリア
void ClouserEnemy::BirthArea(const int Width, const int Height) {
	std::unique_ptr<EnemyRock> newarea = std::make_unique<EnemyRock>();
	newarea->Initialize();
	newarea->InitState(Width, Height);
	newarea->SetPlayer(player);
	enerock.emplace_back(std::move(newarea));
	predictarea->ResetPredict();

}
//予測エリア
void ClouserEnemy::BirthPredict(const int Width, const int Height) {
	predictarea->SetPredict(Width, Height, true);
	predictarea->SetFlashStart(true);
}
//魔法陣生成
void ClouserEnemy::BirthMagic() {
	if (!magic.Alive) { return; }
	static float addFrame = 1.f / 15.f;
	const int l_TargetTimer = 20;
	if (magic.State == MAGIC_BIRTH) {			//魔法陣を広げる
		magic.Pos = { m_Position.x,m_Position.y + 0.2f,m_Position.z };

		if (Helper::FrameCheck(magic.Frame, addFrame)) {
			if (Helper::CheckMin(magic.Timer, l_TargetTimer, 1)) {
				m_Warp = true;
				magic.Frame = {};
				magic.AfterScale = {};
				magic.State = MAGIC_VANISH;
				magic.Timer = {};
			}
		}
		magic.Scale = Ease(In, Cubic, magic.Frame, magic.Scale, magic.AfterScale);
	}
	else {			//魔法陣を縮める
		if (Helper::FrameCheck(magic.Frame, addFrame)) {
			magic.Frame = {};
			magic.AfterScale = 0.2f;
			magic.Alive = false;
			magic.State = MAGIC_BIRTH;
		}
		magic.Scale = Ease(In, Cubic, magic.Frame, magic.Scale, magic.AfterScale);
	}
}
void ClouserEnemy::WarpEnemy() {
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel(m_LastEnemy);
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//キャラが小さくなる
		if (Helper::FrameCheck(enemywarp.Frame, addFrame)) {
			enemywarp.Frame = {};
			enemywarp.AfterScale = 0.5f;
			enemywarp.State = WARP_END;
			coolTimer = {};
			m_Position = l_RandPos;
			m_RotFrame = {};
			m_Rotation.y = 270.0f;
			StagePanel::GetInstance()->EnemyHitReset();
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}
	else {			//キャラが大きくなっている
		if (Helper::FrameCheck(enemywarp.Frame, addFrame)) {
			enemywarp.Frame = {};
			enemywarp.AfterScale = 0.0f;
			m_Warp = false;
			_charaState = STATE_INTER;
			enemywarp.State = WARP_START;
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}

	m_Scale = { enemywarp.Scale,enemywarp.Scale, enemywarp.Scale };
}
//攻撃時の動き
void ClouserEnemy::AttackMove() {
	if (!m_Rot) { return; }
	const float l_AddFrame = 1 / 20.0f;
	if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
		m_Rotation.y = 270.0f;
		m_Rot = false;
		m_AttackFrame = {};
	}

	m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 630.0f);
}