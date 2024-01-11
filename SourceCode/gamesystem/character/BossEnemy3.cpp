#include "BossEnemy3.h"
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
BossEnemy3::BossEnemy3() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::FIRST_BOSS));

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

	bullets = make_unique<EnemyBullet>();
	bullets->Initialize();
	bullets->SetPlayer(player);
}
//初期化
bool BossEnemy3::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,-90.0f,0.0f };
	m_Color = { 1.f,0.f,1.f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy3.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy3.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/BossEnemy3.csv", m_Limit, "Interval");

	auto AttackLimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy3.csv", "ATTACK_LIMIT_NUM")));

	m_AttackLimit.resize(AttackLimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/BossEnemy3.csv", m_AttackLimit, "Attack_Interval");

	m_BulletNum = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy3.csv", "BULLET_NUM")));

	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	CreateSkill(1);
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
void (BossEnemy3::* BossEnemy3::stateTable[])() = {
	&BossEnemy3::Inter,//動きの合間
	&BossEnemy3::Attack,//動きの合間
	&BossEnemy3::Teleport,//瞬間移動
};
//攻撃遷移
void (BossEnemy3::* BossEnemy3::attackTable[])() = {
	&BossEnemy3::BulletAttack,//弾を打つ攻撃
	&BossEnemy3::RowAttack,//列攻撃
	&BossEnemy3::RandomAttack,//ランダム
};

//行動
void BossEnemy3::Action() {
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

	//敵の弾
	bullets->Update();
	//攻撃エリアの更新(実際はスキルになると思う)
	for (auto i = 0; i < enethorn.size(); i++) {
		if (enethorn[i] == nullptr)continue;
		enethorn[i]->Update();

		if (!enethorn[i]->GetAlive()) {
			enethorn.erase(cbegin(enethorn) + i);
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
void BossEnemy3::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();
	//敵の弾
	if (bullets->GetAlive()) {
		bullets->Draw(dxCommon);
	}
	for (auto i = 0; i < enethorn.size(); i++) {
		if (enethorn[i] == nullptr)continue;
		enethorn[i]->Draw(dxCommon);
	}
	predictarea->Draw(dxCommon);
	if (m_Color.w != 0.0f)
		Obj_Draw();
	BaseBackDraw(dxCommon);
}
//ImGui描画
void BossEnemy3::ImGui_Origin() {
	//ImGui::Begin("Boss");
	//ImGui::Text("PosX:%f", m_Position.x);
	//ImGui::Text("PosZ:%f", m_Position.z);
	//ImGui::End();
	//predictarea->ImGuiDraw();
	bullets->ImGuiDraw();
}
//開放
void BossEnemy3::Finalize() {

}
//待機
void BossEnemy3::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		int l_RandState = 2;
		_AttackState = (AttackState)(l_RandState);
	}
}
//攻撃
void BossEnemy3::Attack() {
	(this->*attackTable[_AttackState])();
	PlayerCollide();
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//ワープ
void BossEnemy3::Teleport() {
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
//弾の生成
void BossEnemy3::BirthBullet() {
	/// <summary>
	///	音入れ(弾を打つ音希望(ポンッみたいなやつ)
	/// </summary>
	Audio::GetInstance()->PlayWave("Resources/Sound/SE/Damage.wav", 0.02f);
	//弾の発生
	bullets->InitState({ m_Position.x,m_Position.y + 0.5f,m_Position.z }, m_ShotDir);
}
//攻撃遷移
//弾
void BossEnemy3::BulletAttack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_BULLET];
	const float l_AddFrame = 1 / 30.0f;
	if (_BossType == Boss_SET) {
		if (coolTimer == 10) {		//ここで撃つ方向を決める
			m_ShotDir = Helper::GetRanNum(0, 2);
			//敵が端にいた場合反射によって回転が変に見えるから指定する
			if (m_NowHeight == 0 && m_ShotDir == 2) {
				m_ShotDir = 1;
			}
			else if (m_NowHeight == 3 && m_ShotDir == 1) {
				m_ShotDir = 2;
			}
			//弾を撃つ方向で向きが変わる
			if (m_ShotDir == 0) {
				m_AfterRotY = -90.0f;
			}
			else if (m_ShotDir == 1) {
				m_AfterRotY = -90.0f + 45.f;
			}
			else {
				m_AfterRotY = -90.0f - 45.f;
			}
		}
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
			if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
				m_RotFrame = {};
				coolTimer = {};
				_BossType = Boss_THROW;
				BirthBullet();
			}

			m_Rotation.y = Ease(In, Cubic, m_RotFrame, m_Rotation.y, m_AfterRotY);
		}
	}
	else if (_BossType == Boss_THROW) {
		if (!bullets->GetAlive()) {
			coolTimer = {};
			m_AttackCount++;
			if (m_AttackCount != m_BulletNum) {
				_BossType = Boss_SET;
			}
			else {
				_BossType = Boss_END;
			}
		}
	}
	else {
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
		coolTimer = {};
		_BossType = Boss_SET;
		StagePanel::GetInstance()->EnemyHitReset();
	}
}
//横一列
void BossEnemy3::RowAttack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_ROW];
	if (m_AttackCount != 4) {
		if (coolTimer == 0) {		//予測エリア
			BirthPredict({}, m_AttackCount, "Row");
		}
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {		//実際の攻撃
			m_Jump = true;
			m_AddPower = 0.2f;
			m_Rot = true;
			BirthArea({}, m_AttackCount, "Row");
			coolTimer = {};
			m_AttackCount++;
		}
	}
	else {
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
	}

	predictarea->SetTargetTimer(l_TargetTimer);
}
//ランダムマス攻撃
void BossEnemy3::RandomAttack() {
	//プレイヤーの現在マス
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_RANDOM];
	if (m_AttackCount != 8) {
		if (coolTimer == 0) {
			//プレイヤーからの距離(-1~1)
			int l_RandWigth = Helper::GetRanNum(-1, 1);
			int l_RandHeight = Helper::GetRanNum(-1, 1);
			m_RandWigth = l_PlayerWidth + l_RandWigth;
			m_RandHeight = l_PlayerHeight + l_RandHeight;
			Helper::Clamp(m_RandWigth, 0, 3);
			Helper::Clamp(m_RandHeight, 0, 3);
			BirthPredict(m_RandWigth, m_RandHeight, "Random");
		}
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
			BirthArea(m_RandWigth, m_RandHeight, "Random");
			coolTimer = {};
			m_AttackCount++;
			m_Jump = true;
			m_AddPower = 0.2f;
			m_Rot = true;
		}
	}
	else {
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
	}

	predictarea->SetTargetTimer(l_TargetTimer);
}
//攻撃エリア
void BossEnemy3::BirthArea(const int Width, const int Height, const string& name) {
	if (name == "Row") {			//横一列
		int l_SoundTimer = {};
		for (auto i = 0; i < m_Area.size(); i++) {
			if (m_Area[i][Height] == 1) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				std::unique_ptr<EnemyThorn> newarea = std::make_unique<EnemyThorn>();
				newarea->Initialize();
				newarea->InitState(i, Height);
				newarea->SetPlayer(player);
				if (l_SoundTimer == 0) {
					newarea->SetSound(true);
				}
				l_SoundTimer++;
				enethorn.emplace_back(std::move(newarea));
			}
		}
	}
	else {		//ランダム(プレイヤーから近います)
		std::unique_ptr<EnemyThorn> newarea = std::make_unique<EnemyThorn>();
		newarea->Initialize();
		newarea->InitState(Width, Height);
		newarea->SetPlayer(player);
		newarea->SetSound(true);
		enethorn.emplace_back(std::move(newarea));
	}
	predictarea->ResetPredict();

}
//予測エリア
void BossEnemy3::BirthPredict(const int Width, const int Height, const string& name) {
	if (name == "Row") {			//横一列
		for (auto i = 0; i < m_Area.size(); i++) {
			if (m_Area[i][Height] == 1) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				predictarea->SetPredict(i, Height, true);
			}
		}
	}
	else {//ランダム(プレイヤーから近います)
		predictarea->SetPredict(Width, Height, true);
	}
	predictarea->SetFlashStart(true);
}
//スキルのCSVを読み取る
void BossEnemy3::LoadCsvSkill(std::string& FileName, const int id) {

	std::ifstream file;
	std::stringstream popcom;

	file.open(FileName);
	popcom << file.rdbuf();
	file.close();

	std::string line;
	//アタックエリア用
	std::vector<std::vector<int> > MyVector;

	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("AttackArea") == 0) {
			while (std::getline(line_stream, word)) {
				std::vector<int> row;

				for (char& x : word) {
					int X = x - '0';
					if (x != ' ')
						row.push_back(X);
				}
				MyVector.push_back(row);
			}
		}
		else if (word.find("AttackAreA") == 0) {
			while (std::getline(line_stream, word)) {
				std::vector<int> row;

				for (char& x : word) {
					int X = x - '0';
					if (x != ' ')
						row.push_back(X);
				}
				MyVector.push_back(row);
			}

			m_Area = MyVector;
			break;
		}
	}
}

bool BossEnemy3::CreateSkill(int id) {

	std::string directory = "Resources/csv/chara/enemy/AttackArea/Boss1/AttackArea";

	std::stringstream ss;
	if (id >= 10) {
		ss << directory << id << ".csv";

	}
	else {
		ss << directory << "0" << id << ".csv";
	}
	std::string csv_ = ss.str();

	LoadCsvSkill(csv_, id);

	//Player::GetInstance()->SetDelayTimer(m_Delay);
	return true;
}
//エリア攻撃の判定
void BossEnemy3::PlayerCollide() {

}
//魔法陣生成
void BossEnemy3::BirthMagic() {
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
void BossEnemy3::WarpEnemy() {
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
			m_Rotation.y = -90.0f;
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
void BossEnemy3::AttackMove() {
	if (!m_Rot) { return; }
	const float l_AddFrame = 1 / 20.0f;
	if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
		m_Rotation.y = -90.0f;
		m_Rot = false;
		m_AttackFrame = {};
	}

	m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 630.0f);
}
//クリアシーンの更新
void BossEnemy3::ClearAction() {
	const int l_TargetTimer = 100;
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
void BossEnemy3::GameOverAction() {
	if (_GameOverState == OVER_STOP) {
		m_Position = { -3.0f,0.0f,1.5f };
		m_Rotation = { 0.0f,180.0f,0.0f };
		m_AddDisolve = 0.0f;
		if (player->GetSelectType() == 1) {
			_GameOverState = OVER_YES;
			m_AddPower = 0.3f;
		}
		else if (player->GetSelectType() == 2) {
			_GameOverState = OVER_NO;
		}
	}
	else if (_GameOverState == OVER_YES) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
			m_Position.y = 0.1f;
			m_AddPower = {};
			if (Helper::CheckMin(m_OverTimer, 25, 1)) {
				m_OverTimer = {};
				m_AddPower = 0.3f;
			}
		}
	}
	else {
		const float l_AddRotZ = 0.5f;
		const float l_AddFrame2 = 0.01f;
		float RotPower = 10.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//最初はイージングで回す
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//最後は倒れる
				m_Rotation.z = 90.0f;
			}
		}
		else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 20.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}