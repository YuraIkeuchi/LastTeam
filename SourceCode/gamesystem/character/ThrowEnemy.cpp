#include "ThrowEnemy.h"
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
ThrowEnemy::ThrowEnemy() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::THROW));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//初期化
bool ThrowEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_Scale = { 0.6f,0.4f,0.6f };
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ThrowEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/ThrowEnemy.csv", m_Limit, "Interval");

	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ThrowEnemy.csv", "hp")));

	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_ShadowScale = { 0.05f,0.05f,0.05f };

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

void (ThrowEnemy::* ThrowEnemy::stateTable[])() = {
	&ThrowEnemy::Inter,//動きの合間
	&ThrowEnemy::Attack,//動きの合間
	&ThrowEnemy::Teleport,//瞬間移動
};

//行動
void ThrowEnemy::Action() {
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
	//敵の弾
	for (unique_ptr<Boomerang>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Update();
		}
	}

	//障害物の削除
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i] == nullptr) {
			continue;
		}

		if (!bullets[i]->GetAlive()) {
			bullets.erase(cbegin(bullets) + i);
		}
	}

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();

	if (_charaState == STATE_ATTACK) {
		m_RotPower = Ease(In, Cubic, 0.5f, m_RotPower, 20.0f);
	}
	else {
		m_RotPower = Ease(In, Cubic, 0.5f, m_RotPower, 2.0f);
	}
	m_Rotation.y += m_RotPower;
}

//描画
void ThrowEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();
	//敵の弾
	for (unique_ptr<Boomerang>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Draw(dxCommon);
		}
	}
	if (m_Color.w != 0.0f) {
		Obj_Draw();
	}
	BaseBackDraw(dxCommon);
}
//ImGui描画
void ThrowEnemy::ImGui_Origin() {
	//敵の弾
	/*for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->ImGuiDraw();
		}
	}*/
	ImGui::Begin("Throw");
	ImGui::Text("Height:%d", m_NowHeight);
	ImGui::Text("ShotDir:%d", m_ShotDir);
	ImGui::End();
}
//開放
void ThrowEnemy::Finalize() {

}
//待機
void ThrowEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		m_CanCounter = true;
		m_RandTimer = Helper::GetRanNum(0, 40);
	}
}
//攻撃
void ThrowEnemy::Attack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];

	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		BirthBullet();
		m_CanCounter = false;
		m_CheckPanel = true;
		m_AttackCount = {};
		coolTimer = {};
		StagePanel::GetInstance()->EnemyHitReset();
		_charaState = STATE_SPECIAL;
		m_RandTimer = Helper::GetRanNum(0, 40);
	}
}

//ワープ
void ThrowEnemy::Teleport() {
	const float l_AddFrame = 1 / 30.0f;
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];

	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
	}
}
//弾の生成
void ThrowEnemy::BirthBullet() {
	/// <summary>
	///	音入れ(ぶん回す音希望(ブーメランが回ってる音みたいなやつ、無理なら振り回してるやつ))
	/// </summary>
	Audio::GetInstance()->PlayWave("Resources/Sound/SE/Damage.wav", 0.02f);
	//弾の発生
	unique_ptr<Boomerang> newbullet = make_unique<Boomerang>();
	newbullet->Initialize();
	newbullet->SetPlayer(player);
	newbullet->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
	bullets.emplace_back(std::move(newbullet));
}
//魔法陣生成
void ThrowEnemy::BirthMagic() {
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
void ThrowEnemy::WarpEnemy() {
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
			m_RandTimer = Helper::GetRanNum(0, 40);
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}

	m_Scale = { enemywarp.Scale,enemywarp.Scale, enemywarp.Scale };
}
//クリアシーンの更新
void ThrowEnemy::ClearAction() {
	const int l_TargetTimer = 10;
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
void ThrowEnemy::GameOverAction() {
	const float l_AddRot = 15.0f;
	if (_GameOverState == OVER_STOP) {
		m_Position = { -3.0f,0.0f,3.5f };
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
		m_Rotation.y -= l_AddRot;
	}
	else {
		float l_AddRotZ = {};
		float l_AddFrame2 = {};

		l_AddRotZ = float(Helper::GetRanNum(30, 100)) / 100;
		l_AddFrame2 = float(Helper::GetRanNum(1, 10)) / 500;
		float RotPower = 3.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//最初はイージングで回す
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//最後は倒れる
				m_Rotation.z = 90.0f;
			}
		}
		else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 25.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}