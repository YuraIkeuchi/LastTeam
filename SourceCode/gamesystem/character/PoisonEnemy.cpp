#include "PoisonEnemy.h"
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
PoisonEnemy::PoisonEnemy() {
	//共通の初期化処理
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::MISSILE));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	//予測
	predictarea.reset(new PredictArea("ENEMY"));
	predictarea->Initialize();
}
//初期化
bool PoisonEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_BaseScale = 0.7f;
	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "hp")));
	m_MaxHP = m_HP;
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/PoisonEnemy.csv", m_Limit, "Interval");
	m_BulletNum = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "POISON_NUM")));

	m_CheckPanel = true;
	m_ShadowScale = { 0.05f,0.05f,0.05f };

	magic.Alive = false;
	magic.Frame = {};
	magic.Scale = {};
	magic.AfterScale = 0.2f;
	magic.Pos = {};
	magic.State = {};

	enemywarp.AfterScale = {};
	enemywarp.Scale = 0.7f;
	m_RandTimer = Helper::GetRanNum(0, 40);
	m_AddDisolve = 2.0f;
	return true;
}

void (PoisonEnemy::* PoisonEnemy::stateTable[])() = {
	&PoisonEnemy::Inter,//動きの合間
	&PoisonEnemy::Attack,//動きの合間
	&PoisonEnemy::Teleport,//瞬間移動
};

//行動
void PoisonEnemy::Action() {
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

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	//shadow_tex->SetPosition(m_ShadowPos);
	//shadow_tex->SetScale(m_ShadowScale);
	//shadow_tex->Update();

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();

	//どくの削除
	for (int i = 0; i < poisonarea.size(); i++) {
		if (poisonarea[i] == nullptr) {
			continue;
		}

		poisonarea[i]->Update();
		if (!poisonarea[i]->GetAlive()) {
			poisonarea.erase(cbegin(poisonarea) + i);
		}
	}
}

//描画
void PoisonEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	predictarea->Draw(dxCommon);
	IKETexture::PostDraw();

	//毒エリアの描画
	for (int i = 0; i < poisonarea.size(); i++) {
		if (poisonarea[i] == nullptr) {
			continue;
		}

		poisonarea[i]->Draw(dxCommon);
	}
	if (m_Color.w != 0.0f) {
		Obj_Draw();
	}
	BaseBackDraw(dxCommon);
}
//ImGui描画
void PoisonEnemy::ImGui_Origin() {
	//どくの削除
	for (int i = 0; i < poisonarea.size(); i++) {
		if (poisonarea[i] == nullptr) {
			continue;
		}

		poisonarea[i]->ImGuiDraw();
	}
	ImGui::Begin("Poison");
	ImGui::Text("m_RandHeight:%d", m_RandHeight);
	ImGui::Text("m_RandWidth:%d", m_RandWidth);
	ImGui::Text("coolT:%d", coolTimer);
	ImGui::End();
}
//開放
void PoisonEnemy::Finalize() {

}
//待機
void PoisonEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		coolTimer = 100;
		_charaState = STATE_ATTACK;
	}
}
//攻撃
void PoisonEnemy::Attack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];
	float l_AfterScale = {};
	float l_AddFrame = {};
	if (_PoisonType == Poison_SET) {
		l_AfterScale = 0.3f;
		l_AddFrame = 1 / 30.0f;
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
			m_CanCounter = true;
			predictarea->ResetPredict();
			if (Helper::FrameCheck(m_ScaleFrame,l_AddFrame)) {
				coolTimer = {};
				_PoisonType = Poison_THROW;
				m_ScaleFrame = {};
				StagePanel::GetInstance()->PoisonSetPanel(m_RandWidth, m_RandHeight);
				BirthPredict(m_RandWidth, m_RandHeight);
			}

			m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
		}
	}
	else if (_PoisonType == Poison_THROW) {
		l_AfterScale = 0.7f;
		l_AddFrame = 1 / 20.0f;
		if (Helper::FrameCheck(m_ScaleFrame, l_AddFrame)) {
			m_AttackCount++;
			BirthPoison();
			if (m_AttackCount != m_BulletNum) {
				_PoisonType = Poison_SET;
			}
			else {
				_PoisonType = Poison_END;
			}
			m_CanCounter = false;
			m_ScaleFrame = {};
		}
		m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
	}
	else {
		if (Helper::CheckMin(coolTimer, 130, 1)) {
			m_CheckPanel = true;
			m_AttackCount = {};
			_charaState = STATE_SPECIAL;
			coolTimer = {};
			_PoisonType = Poison_SET;
			StagePanel::GetInstance()->EnemyHitReset();
			predictarea->ResetPredict();
		}
	}

	m_Scale = { m_Scale.x,m_BaseScale,m_Scale.z };

	predictarea->SetTargetTimer(l_TargetTimer);
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//ワープ
void PoisonEnemy::Teleport() {
	const int l_RandTimer = Helper::GetRanNum(0, 30);
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];
	if (Helper::CheckMin(coolTimer, l_TargetTimer + l_RandTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
	}
}
//毒の生成
void PoisonEnemy::BirthPoison() {
	/// <summary>
	///	音入れ(弾を打つ音希望(ポンッみたいなやつ))
	/// </summary>
	Audio::GetInstance()->PlayWave("Resources/Sound/SE/shot.wav", 0.02f);

	std::unique_ptr<PoisonArea> newarea = std::make_unique<PoisonArea>();
	newarea->SetPosition({ m_Position.x,m_Position.y + 0.5f,m_Position.z });
	newarea->InitState(m_RandWidth, m_RandHeight);
	newarea->SetPlayer(player);
	poisonarea.push_back(std::move(newarea));
}
//予測エリア
void PoisonEnemy::BirthPredict(const int Width, const int Height) {
	predictarea->SetPredict(Width, Height, true);
	predictarea->SetFlashStart(true);
}
//魔法陣生成
void PoisonEnemy::BirthMagic() {
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
void PoisonEnemy::WarpEnemy() {
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel(m_LastEnemy);
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//キャラが小さくなる
		if (Helper::FrameCheck(enemywarp.Frame, addFrame)) {
			enemywarp.Frame = {};
			enemywarp.AfterScale = 0.7f;
			enemywarp.State = WARP_END;
			coolTimer = {};
			m_Position = l_RandPos;
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
			m_RandTimer = Helper::GetRanNum(0, 40);
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}

	m_Scale = { enemywarp.Scale,enemywarp.Scale, enemywarp.Scale };
}
//クリアシーンの更新
void PoisonEnemy::ClearAction() {
	const int l_TargetTimer = 130;
	const float l_AddFrame = 1 / 200.0f;
	if (m_ClearTimer == 0) {
		m_Position.y = 10.0f;
	}

	if (Helper::CheckMin(m_ClearTimer, l_TargetTimer, 1)) {
		if (Helper::FrameCheck(m_ClearFrame, l_AddFrame)) {
			m_ClearFrame = 1.0f;
		}
		else {
			m_Position.y = Ease(In, Cubic, m_ClearFrame, m_Position.y, 0.1f);
		}
	}
	m_AddDisolve = {};
	Obj_SetParam();
}
//ゲームオーバーシーンの更新
void PoisonEnemy::GameOverAction() {
	float l_AfterScale = {};
	float l_AddFrame = {};
	if (_GameOverState == OVER_STOP) {
		m_Position = { -6.0f,0.0f,3.5f };
		m_Rotation = { 0.0f,180.0f,0.0f };
		m_AddDisolve = 0.0f;
		if (player->GetOverType() == 3) {
			if (player->GetSelectType() == 1) {
				_GameOverState = OVER_YES;
				m_AddPower = 0.3f;
				_PoisonType = Poison_SET;
			}
			else if (player->GetSelectType() == 2) {
				_GameOverState = OVER_NO;
			}
		}
	}
	else if (_GameOverState == OVER_YES) {
		if (_PoisonType == Poison_SET) {
			l_AfterScale = 0.3f;
			l_AddFrame = 1 / 30.0f;
			if (Helper::FrameCheck(m_ScaleFrame, l_AddFrame)) {
				if (Helper::CheckMin(m_OverTimer, 10, 1)) {
					_PoisonType = Poison_THROW;
					m_OverTimer = {};
					m_ScaleFrame = {};
				}
			}
			m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
		}
		else if (_PoisonType == Poison_THROW) {
			l_AfterScale = 0.7f;
			l_AddFrame = 1 / 10.0f;
			if (Helper::FrameCheck(m_ScaleFrame, l_AddFrame)) {
				if (Helper::CheckMin(m_OverTimer, 40, 1)) {
					_PoisonType = Poison_SET;
					m_OverTimer = {};
					m_ScaleFrame = {};
				}
			}
			m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
			m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
		}
	}
	else {
		float l_AddRotZ = {};
		float l_AddFrame2 = {};

		l_AddRotZ = float(Helper::GetRanNum(30, 100)) / 100;
		l_AddFrame2 = float(Helper::GetRanNum(1, 10)) / 500;
		float RotPower = 5.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//最初はイージングで回す
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//最後は倒れる
				m_Rotation.z = 90.0f;
			}
		}
		else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 10.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}
void PoisonEnemy::DeathSpecial() {
}