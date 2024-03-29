#include "HealEnemy.h"
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
HealEnemy::HealEnemy() {
	//共通の初期化処理
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::HEAL));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	chanting.tex.reset(new IKETexture(ImageManager::CHANTING_HEAL, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	chanting.tex->TextureCreate();
	chanting.tex->Initialize();
	chanting.tex->SetRotation({ 90.0f,0.0f,0.0f });

	//shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	//shadow_tex->TextureCreate();
	//shadow_tex->Initialize();
	//shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//初期化
bool HealEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,180.0f,0.0f };
	m_Scale = { 0.6f,0.6f,0.6f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/HealEnemy.csv", "hp")));
	m_MaxHP = m_HP;
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/HealEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/HealEnemy.csv", m_Limit, "Interval");

	m_CheckPanel = true;
	m_ShadowScale = { 0.05f,0.05f,0.05f };

	magic.Alive = false;
	magic.Frame = {};
	magic.Scale = {};
	magic.AfterScale = 0.2f;
	magic.Pos = {};
	magic.State = {};

	chanting.Alive = false;
	chanting.Frame = {};
	chanting.Scale = 0.2f;
	chanting.AfterScale = 0.2f;
	chanting.Rotate = { 90.0f,0.0f,0.0f };
	chanting.Pos = {};
	chanting.State = {};

	enemywarp.AfterScale = {};
	enemywarp.Scale = 0.5f;

	m_AddDisolve = 2.0f;
	m_RandTimer = Helper::GetRanNum(0, 40);
	return true;
}

void (HealEnemy::* HealEnemy::stateTable[])() = {
	&HealEnemy::Inter,//動きの合間
	&HealEnemy::Attack,//動きの合間
	&HealEnemy::Teleport,//瞬間移動
};

//行動
void HealEnemy::Action() {
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
	ChantingHeal();//ヒール詠唱時に出るやつ
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

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	/*shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();*/

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();

	chanting.tex->SetPosition({ m_Position.x,m_Position.y + 0.2f,m_Position.z });
	chanting.tex->SetScale({ chanting.Scale,chanting.Scale,chanting.Scale });
	chanting.tex->SetRotation(chanting.Rotate);
	chanting.tex->Update();
}

//描画
void HealEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	if (chanting.Alive) {
		chanting.tex->Draw();
	}
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();
	Obj_Draw();
	BaseBackDraw(dxCommon);

}
//ImGui描画
void HealEnemy::ImGui_Origin() {
	ImGui::Begin("Heal");
	ImGui::Text("CoolT:%d", coolTimer);
	ImGui::Text("Warp:%d", m_Warp);
	ImGui::Text("frame:%f", magic.Frame);
	ImGui::End();
}
//開放
void HealEnemy::Finalize() {

}
//待機
void HealEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	chanting.Alive = true;
	m_CanCounter = true;
	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		coolTimer = 0;
		chanting.Scale = 0.2f;
		chanting.Alive = false;
		_charaState = STATE_ATTACK;
	}
}
//攻撃
void HealEnemy::Attack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];
	m_CanCounter = false;
	Audio::GetInstance()->PlayWave("Resources/Sound/SE/Heal01.wav", 0.05f);
	GameStateManager::GetInstance()->SetIsHeal(true);
	m_Jump = true;
	m_AddPower = 0.2f;
	m_Rot = true;
	_charaState = STATE_SPECIAL;
	coolTimer = {};
}

//ワープ
void HealEnemy::Teleport() {
	const int l_RandTimer = Helper::GetRanNum(0, 30);
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];

	if (Helper::CheckMin(coolTimer, l_RandTimer + l_RandTimer, 1)) {
		magic.Alive = true;
		coolTimer = {};
	}

	if (m_Warp) {
		WarpEnemy();
	}
}

//魔法陣生成
void HealEnemy::BirthMagic() {
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
			m_Warp = false;
		}
		magic.Scale = Ease(In, Cubic, magic.Frame, magic.Scale, magic.AfterScale);
	}
}

void HealEnemy::ChantingHeal()
{
	if (!chanting.Alive) { return; }
	static float addFrame = 1.f / 15.f;
	const int l_TargetTimer = 20;
	{			//魔法陣を縮める
		chanting.Scale -= 0.00055f;
		chanting.Rotate.y -= 5.0f;
		//chanting.State == CHANTING_BIRTH;
	}
}

void HealEnemy::WarpEnemy() {
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
//攻撃時の動き
void HealEnemy::AttackMove() {
	if (!m_Rot) { return; }
	const float l_AddFrame = 1 / 20.0f;
	if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
		m_Rotation.y = 180.0f;
		m_Rot = false;
		m_AttackFrame = {};
	}

	m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 540.0f);
}
//クリアシーンの更新
void HealEnemy::ClearAction() {
	const int l_TargetTimer = 70;
	const float l_AddFrame = 1 / 200.0f;
	m_Rotation.y = 90.0f;
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
void HealEnemy::GameOverAction() {
	const float l_AddFrame = 1 / 30.0f;
	if (_GameOverState == OVER_STOP) {
		m_Position = { 0.0f,0.0f,2.5f };
		m_Rotation = { 0.0f,90.0f,0.0f };
		m_AddDisolve = 0.0f;
		if (player->GetOverType() == 3) {
			if (player->GetSelectType() == 1) {
				_GameOverState = OVER_YES;
				m_AddPower = 0.3f;
				m_Rot = true;
			}
			else if (player->GetSelectType() == 2) {
				_GameOverState = OVER_NO;
			}
		}
	}
	else if (_GameOverState == OVER_YES) {
		if (Helper::CheckMin(m_OverTimer, 80, 1)) {
			m_OverTimer = {};
			m_Rot = true;
		}

		if (m_Rot) {
			if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
				m_Rotation.y = 90.0f;
				m_Rot = false;
				m_AttackFrame = {};
			}

			m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 450.0f);
		}
	}
	else {
		float l_AddRotZ = {};
		float l_AddFrame2 = {};

		l_AddRotZ = float(Helper::GetRanNum(30, 100)) / 100;
		l_AddFrame2 = float(Helper::GetRanNum(1, 10)) / 500;
		float RotPower = 4.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//最初はイージングで回す
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//最後は倒れる
				m_Rotation.z = 90.0f;
			}
		}
		else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 35.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}
void HealEnemy::DeathSpecial() {
}