#include "BossEnemy2.h"
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
BossEnemy2::BossEnemy2() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	m_Object->SetLightEffect(false);
	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });
	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });
	hptex->SetColor({ 0.5f,1.0f,0.5f,1.0f });
	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>(0.5f);
		_drawnumber[i]->Initialize();
	}

	/*shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });*/
	//予測
	predictarea.reset(new PredictArea("ENEMY"));
	predictarea->Initialize();
}
//初期化
bool BossEnemy2::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_Color = { 0.0f,1.0f,0.5f,1.0f };
	m_Scale = { 0.4f,0.4f,0.4f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy2.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy2.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/BossEnemy2.csv", m_Limit, "Interval");

	auto AttackLimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy2.csv", "ATTACK_LIMIT_NUM")));

	m_AttackLimit.resize(AttackLimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/BossEnemy2.csv", m_AttackLimit, "Attack_Interval");

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
void (BossEnemy2::* BossEnemy2::stateTable[])() = {
	&BossEnemy2::Inter,//動きの合間
	&BossEnemy2::Attack,//動きの合間
	&BossEnemy2::Teleport,//瞬間移動
};
//攻撃遷移
void (BossEnemy2::* BossEnemy2::attackTable[])() = {
	&BossEnemy2::SpinningAttack,//回転攻撃
	&BossEnemy2::ShockWaveAttack,//列攻撃
	&BossEnemy2::Recovery,//ランダム
};

//行動
void BossEnemy2::Action() {
	(this->*stateTable[_charaState])();
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
void BossEnemy2::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	IKETexture::PostDraw();

	for (auto i = 0; i < enethorn.size(); i++) {
		if (enethorn[i] == nullptr)continue;
		enethorn[i]->Draw(dxCommon);
	}
	predictarea->Draw(dxCommon);
	if (m_Color.w != 0.0f)
		Obj_Draw();
}
//ImGui描画
void BossEnemy2::ImGui_Origin() {
	ImGui::Begin("Area");
	ImGui::Text("AttackCount:%d", m_AttackCount);
	ImGui::Text("cool:%d", coolTimer);
	ImGui::End();
	predictarea->ImGuiDraw();
}
//開放
void BossEnemy2::Finalize() {

}
//待機
void BossEnemy2::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		int l_RandState = Helper::GetRanNum(0, 2);
		_AttackState = (AttackState)(l_RandState);
	}
}
//攻撃
void BossEnemy2::Attack() {
	(this->*attackTable[_AttackState])();
	PlayerCollide();
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//ワープ
void BossEnemy2::Teleport() {
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

//回転攻撃
void BossEnemy2::SpinningAttack()
{
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_SPINNING];

	if (m_AttackCount != 13) {
		if (coolTimer == 0) {		//予測エリア
			SpinningAttackBirthPredict(m_AttackCount);
		}
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {		//実際の攻撃
			m_Jump = true;
			m_AddPower = 0.2f;
			m_Rot = true;
			SpinningAttackBirthArea(m_AttackCount);
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

//衝撃波攻撃
void BossEnemy2::ShockWaveAttack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_SHOCKWAVE];


	if (m_AttackCount != 3) {
		if (coolTimer == 0) {		//予測エリア
			ShockWaveAttackBirthPredict(m_AttackCount);
		}
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {		//実際の攻撃
			m_Jump = true;
			m_AddPower = 0.2f;
			m_Rot = true;
			ShockWaveAttackBirthArea(m_AttackCount);
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

//回復行動
void BossEnemy2::Recovery() {
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_RECOVERY];
	if (m_AttackCount != 1) {
		GameStateManager::GetInstance()->SetIsHeal(true);
		m_Jump = true;
		m_AddPower = 0.2f;
		m_Rot = true;
		coolTimer = {};
		m_AttackCount++;
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
void BossEnemy2::BirthArea(const int Width, const int Height, const string& name) {
	if (m_Area[Width][Height] == 1) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
		std::unique_ptr<EnemyThorn> newarea = std::make_unique<EnemyThorn>();
		newarea->Initialize();
		newarea->InitState(Width, Height);
		newarea->SetPlayer(player);
		enethorn.emplace_back(std::move(newarea));
	}
	predictarea->ResetPredict();

}

//予測エリア
void BossEnemy2::BirthPredict(const int Width, const int Height, const string& name) {
	predictarea->SetPredict(Width, Height, true);
	predictarea->SetFlashStart(true);
}

void BossEnemy2::SpinningAttackBirthPredict(int AttackCount)
{
	int tmpAttackCount = AttackCount;
	if (AttackCount >= 6) {
		tmpAttackCount = AttackCount - 6;
	}

	if (tmpAttackCount == 0) {
		BirthPredict(0, 0, "Spinning");
		BirthPredict(1, 1, "Spinning");
		BirthPredict(2, 2, "Spinning");
		BirthPredict(3, 3, "Spinning");
	}
	else if (tmpAttackCount == 1) {
		BirthPredict(1, 0, "Spinning");
		BirthPredict(1, 1, "Spinning");
		BirthPredict(2, 2, "Spinning");
		BirthPredict(2, 3, "Spinning");
	}
	else if (tmpAttackCount == 2) {
		BirthPredict(2, 0, "Spinning");
		BirthPredict(2, 1, "Spinning");
		BirthPredict(1, 2, "Spinning");
		BirthPredict(1, 3, "Spinning");
	}else if (tmpAttackCount == 3) {
		BirthPredict(3, 0, "Spinning");
		BirthPredict(2, 1, "Spinning");
		BirthPredict(1, 2, "Spinning");
		BirthPredict(0, 3, "Spinning");
	}
	else if (tmpAttackCount == 4) {
		BirthPredict(2, 1, "Spinning");
		BirthPredict(3, 1, "Spinning");
		BirthPredict(0, 2, "Spinning");
		BirthPredict(1, 2, "Spinning");
	}
	else if (tmpAttackCount == 5) {
		BirthPredict(2, 2, "Spinning");
		BirthPredict(3, 2, "Spinning");
		BirthPredict(0, 1, "Spinning");
		BirthPredict(1, 1, "Spinning");
	}
	else if (tmpAttackCount == 6) {
		BirthPredict(0, 0, "Spinning");
		BirthPredict(1, 1, "Spinning");
		BirthPredict(2, 2, "Spinning");
		BirthPredict(3, 3, "Spinning");
	}
}

void BossEnemy2::SpinningAttackBirthArea(int AttackCount)
{
	int tmpAttackCount = AttackCount;
	if (AttackCount >= 6) {
		tmpAttackCount = AttackCount - 6;
	}

	if (tmpAttackCount == 0) {
		BirthArea(0, 0, "Spinning");
		BirthArea(1, 1, "Spinning");
		BirthArea(2, 2, "Spinning");
		BirthArea(3, 3, "Spinning");
	}
	else if (tmpAttackCount == 1) {
		BirthArea(1, 0, "Spinning");
		BirthArea(1, 1, "Spinning");
		BirthArea(2, 2, "Spinning");
		BirthArea(2, 3, "Spinning");
	}
	else if (tmpAttackCount == 2) {
		BirthArea(2, 0, "Spinning");
		BirthArea(2, 1, "Spinning");
		BirthArea(1, 2, "Spinning");
		BirthArea(1, 3, "Spinning");
	}
	else if (tmpAttackCount == 3) {
		BirthArea(3, 0, "Spinning");
		BirthArea(2, 1, "Spinning");
		BirthArea(1, 2, "Spinning");
		BirthArea(0, 3, "Spinning");
	}
	else if (tmpAttackCount == 4) {
		BirthArea(2, 1, "Spinning");
		BirthArea(3, 1, "Spinning");
		BirthArea(0, 2, "Spinning");
		BirthArea(1, 2, "Spinning");
	}
	else if (tmpAttackCount == 5) {
		BirthArea(2, 2, "Spinning");
		BirthArea(3, 2, "Spinning");
		BirthArea(0, 1, "Spinning");
		BirthArea(1, 1, "Spinning");
	}
	else if (tmpAttackCount == 6) {
		BirthArea(0, 0, "Spinning");
		BirthArea(1, 1, "Spinning");
		BirthArea(2, 2, "Spinning");
		BirthArea(3, 3, "Spinning");
	}
}

void BossEnemy2::ShockWaveAttackBirthPredict(int AttackCount)
{
	if (AttackCount == 0) {
		BirthPredict(1, 1, "ShockWave");
		BirthPredict(2, 1, "ShockWave");
		BirthPredict(1, 2, "ShockWave");
		BirthPredict(2, 2, "ShockWave");
	}
	else if (AttackCount == 1) {
		BirthPredict(1, 0, "ShockWave");
		BirthPredict(2, 0, "ShockWave");
		BirthPredict(0, 1, "ShockWave");
		BirthPredict(3, 1, "ShockWave");
		BirthPredict(0, 2, "ShockWave");
		BirthPredict(3, 2, "ShockWave");
		BirthPredict(1, 3, "ShockWave");
		BirthPredict(2, 3, "ShockWave");
	}
	else if (AttackCount == 2) {
		BirthPredict(0, 0, "ShockWave");
		BirthPredict(3, 0, "ShockWave");
		BirthPredict(0, 3, "ShockWave");
		BirthPredict(3, 3, "ShockWave");
	}
}

void BossEnemy2::ShockWaveAttackBirthArea(int AttackCount)
{
	if (AttackCount == 0) {
		BirthArea(1, 1, "ShockWave");
		BirthArea(2, 1, "ShockWave");
		BirthArea(1, 2, "ShockWave");
		BirthArea(2, 2, "ShockWave");
	}
	else if (AttackCount == 1) {
		BirthArea(1, 0, "ShockWave");
		BirthArea(2, 0, "ShockWave");
		BirthArea(0, 1, "ShockWave");
		BirthArea(3, 1, "ShockWave");
		BirthArea(0, 2, "ShockWave");
		BirthArea(3, 2, "ShockWave");
		BirthArea(1, 3, "ShockWave");
		BirthArea(2, 3, "ShockWave");
	}
	else if (AttackCount == 2) {
		BirthArea(0, 0, "ShockWave");
		BirthArea(3, 0, "ShockWave");
		BirthArea(0, 3, "ShockWave");
		BirthArea(3, 3, "ShockWave");
	}
}

//スキルのCSVを読み取る
void BossEnemy2::LoadCsvSkill(std::string& FileName, const int id) {

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

bool BossEnemy2::CreateSkill(int id) {

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
void BossEnemy2::PlayerCollide() {

}

//魔法陣生成
void BossEnemy2::BirthMagic() {
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

void BossEnemy2::WarpEnemy() {
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
void BossEnemy2::AttackMove() {
	if (!m_Rot) { return; }
	const float l_AddFrame = 1 / 20.0f;
	if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
		m_Rotation.y = 270.0f;
		m_Rot = false;
		m_AttackFrame = {};
	}

	m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 630.0f);
}