#include "TackleEnemy.h"
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
TackleEnemy::TackleEnemy() {
	//共通の初期化処理
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::SQUID));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	/*shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });*/
}
//初期化
bool TackleEnemy::Initialize() {

	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.3f,0.3f,0.3f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/TackleEnemy.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/TackleEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/TackleEnemy.csv", m_Limit, "Interval");
	m_Speed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/TackleEnemy.csv", "Speed")));
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/TackleEnemy.csv", "Damage")));

	m_MaxHP = m_HP;
	StagePanel::GetInstance()->EnemyHitReset();
	m_ShadowScale = { 0.03f,0.03f,0.03f };
	//予測
	predictArea = std::make_unique<PredictArea>("ENEMY");
	predictArea->Initialize();

	m_OldWidth = m_NowWidth;
	m_OldHeight = m_NowHeight;
	magic.Alive = false;
	magic.Frame = {};
	magic.Scale = {};
	magic.AfterScale = 0.2f;
	magic.Pos = {};
	magic.State = {};

	enemywarp.AfterScale = {};
	enemywarp.Scale = 0.5f;

	m_AddDisolve = 2.0f;
	m_RandTimer = Helper::GetRanNum(0, 40);
	return true;
}

void (TackleEnemy::* TackleEnemy::stateTable[])() = {
	&TackleEnemy::Inter,//動きの合間
	&TackleEnemy::Attack,//動きの合間
	&TackleEnemy::Teleport,//瞬間移動
	&TackleEnemy::StandBy
};

//行動
void TackleEnemy::Action() {
	if (!m_Induction) {
		(this->*stateTable[_charaState])();
	} else {
		InductionMove();
	}
	Obj_SetParam();
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判
	TackleCollide();//敵→プレイヤーからの当たり判定
	PoisonState();//毒
	BirthMagic();//魔法陣

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	//shadow_tex->SetPosition(m_ShadowPos);
	//shadow_tex->SetScale(m_ShadowScale);
	//shadow_tex->Update();
	predictArea->Update();

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();
	m_OldWidth = m_NowWidth;
	m_OldHeight = m_NowHeight;
}
//描画
void TackleEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	predictArea->Draw(dxCommon);
	IKETexture::PostDraw();

	if (m_Color.w != 0.0f) {
		Obj_Draw();
	}
	BaseBackDraw(dxCommon);
}
//ImGui描画
void TackleEnemy::ImGui_Origin() {
	ImGui::Begin("TackleEnemy");
	ImGui::Text("Height:%d,Width:%d", m_NowHeight, m_NowWidth);
	ImGui::Text("Timer:%d", m_PoisonTimer);
	ImGui::End();
}
//開放
void TackleEnemy::Finalize() {

}

void TackleEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		_charaState = STATE_STANDBY;
		coolTimer = 0;
	}
}

void TackleEnemy::Attack() {
	const float l_TargetX = -8.0f;
	const float l_AddFrame = 1 / 45.0f;
	const float l_AddRot = 20.0f;
	if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
		m_Frame = 1.0f;
		m_Position.x -= m_Speed;
		m_Rotation.x += l_AddRot;
		//通り過ぎたら削除
		predictArea->VanishPredict(m_OldWidth, m_NowHeight);

		if (m_Position.x < l_TargetX) {
			m_CheckPanel = true;
			_charaState = STATE_SPECIAL;
			StagePanel::GetInstance()->EnemyHitReset();
			m_Frame = {};
		}
	}
	m_Rotation.z = Ease(In, Cubic, m_Frame, m_Rotation.z, 90.0f);
	m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.25f);
}
//ワープ
void TackleEnemy::Teleport() {
	const float l_TargetX = -8.0f;
	const float l_AddFrame = 1 / 45.0f;
	const int l_RandTimer = Helper::GetRanNum(0, 30);
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];
	if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
		m_Frame = 1.0f;
	}

	m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.0f);
	if (Helper::CheckMin(coolTimer, l_TargetTimer + l_RandTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
		m_Hit = false;
	}
}

void TackleEnemy::StandBy() {
	int nextWidthPanel = m_NowWidth - (nextPredict + 1);
	if (nextWidthPanel < 0) {
		_charaState = STATE_ATTACK;
		nextPredict = 0;
		predictFrame = 0.f;
		return;
	}

	if (Helper::FrameCheck(predictFrame, 1.f / 5.0f)) {
		predictArea->VersePredict(nextWidthPanel, m_NowHeight);
		nextPredict++;
		predictFrame = 0.f;
	}

}

//魔法陣生成
void TackleEnemy::BirthMagic() {
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
	} else {			//魔法陣を縮める
		if (Helper::FrameCheck(magic.Frame, addFrame)) {
			magic.Frame = {};
			magic.AfterScale = 0.2f;
			magic.Alive = false;
			magic.State = MAGIC_BIRTH;
		}
		magic.Scale = Ease(In, Cubic, magic.Frame, magic.Scale, magic.AfterScale);
	}
}
void TackleEnemy::WarpEnemy() {
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel(m_LastEnemy);
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//キャラが小さくなる
		if (Helper::FrameCheck(enemywarp.Frame, addFrame)) {
			enemywarp.Frame = {};
			enemywarp.AfterScale = 0.3f;
			enemywarp.State = WARP_END;
			coolTimer = {};
			m_Position = l_RandPos;
			StagePanel::GetInstance()->EnemyHitReset();
			m_Frame = {};
			m_Rotation = { 0.0f,0.0f,0.0f };
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	} else {			//キャラが大きくなっている
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

bool TackleEnemy::TackleCollide() {
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const float l_Radius = 0.15f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (!m_Hit)) {
		player->RecvDamage(m_Damage, "NORMAL");
		m_Hit = true;
		return true;
	} else {
		return false;
	}

	return false;
}
//クリアシーンの更新
void TackleEnemy::ClearAction() {
	const int l_TargetTimer = 40;
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
void TackleEnemy::GameOverAction() {
	const float l_AddRot = 20.0f;
	if (_GameOverState == OVER_STOP) {
		m_Position = { 4.0f,0.0f,3.5f };
		m_Rotation = { 0.0f,180.0f,0.0f };
		m_AddDisolve = 0.0f;
		if (player->GetSelectType() == 1) {
			_GameOverState = OVER_YES;
		}
		else if (player->GetSelectType() == 2) {
			_GameOverState = OVER_NO;
		}
	}
	else if (_GameOverState == OVER_YES) {
		m_Rotation.y += l_AddRot;
	}
	else {
		float l_AddRotZ = {};
		float l_AddFrame2 = {};

		l_AddRotZ = float(Helper::GetRanNum(30, 100)) / 100;
		l_AddFrame2 = float(Helper::GetRanNum(1, 10)) / 500;
		float RotPower = 10.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//最初はイージングで回す
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//最後は倒れる
				m_Rotation.z = 90.0f;
			}
		}
		else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 30.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}