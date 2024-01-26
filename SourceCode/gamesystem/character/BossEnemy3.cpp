#include "BossEnemy3.h"
#include <LastBossState.h>
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
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::LAST_BOSS));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	LastBossState::GetInstance()->SetBossShield(false);
	//HPII
	for (int i = {}; i < shield.size(); i++) {
		shield[i].tex.reset(new IKETexture(ImageManager::SHIELD_TEX, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
		shield[i].tex->TextureCreate();
		shield[i].tex->Initialize();
		shield[i].tex->SetIsBillboard(true);
		shield[i].pos = { 0.0f,{},0.0f};
		shield[i].CircleScale = 1.0f;
		shield[i].color = { 1.0f,1.0f,1.0f,1.0f };
		shield[i].scale = 0.0f;
	}
	
	//予測
	predictarea.reset(new PredictArea("ENEMY"));
	predictarea->Initialize();

	onomatope = make_unique<Onomatope>();
}
//初期化
bool BossEnemy3::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,-90.0f,0.0f };
	//m_Color = { 1.f,0.f,1.f,1.0f };
	m_Scale = { 0.8f,0.8f,0.8f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy3.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy3.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/BossEnemy3.csv", m_Limit, "Interval");

	auto AttackLimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy3.csv", "ATTACK_LIMIT_NUM")));

	m_AttackLimit.resize(AttackLimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/BossEnemy3.csv", m_AttackLimit, "Attack_Interval");

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
	enemywarp.Scale = 0.8f;

	m_AddDisolve = 2.0f;


	shield[0].CircleSpeed = {};
	shield[1].CircleSpeed = 90.0f;
	shield[2].CircleSpeed = 180.0f;
	shield[3].CircleSpeed = 270.0f;
	for (int i = 0; i < PANEL_WIDTH / 2; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_SafeArea[i][j] = false;
		}
	}
	m_EnemyTag = "LASTBOSS";
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
	&BossEnemy3::RockAttack,//弾を打つ攻撃
	&BossEnemy3::RandomAttack,//ランダム
	&BossEnemy3::AroundAttack,//ランダム
};

//行動
void BossEnemy3::Action() {
	if (m_LastEnemy && _charaState == STATE_INTER && !m_AngerFinish) {	//最後の一体になったとき怒り状態になる
		m_Anger = true;
	}
	if (!m_Induction) {
		if (!m_Anger) {
			(this->*stateTable[_charaState])();
		}
		else {	//怒りの動き
			AngerMove();
		}
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

	//敵の竜巻
	//攻撃エリアの更新(実際はスキルになると思う)
	for (auto i = 0; i < enetornade.size(); i++) {
		if (enetornade[i] == nullptr)continue;
		enetornade[i]->Update();

		if (!enetornade[i]->GetAlive()) {
			enetornade.erase(cbegin(enetornade) + i);
		}
	}

	//岩エリアの更新
	for (auto i = 0; i < enerock.size(); i++) {
		if (enerock[i] == nullptr)continue;
		enerock[i]->Update();

		if (!enerock[i]->GetAlive()) {
			enerock.erase(cbegin(enerock) + i);
		}
	}

	LastBossState::GetInstance()->SetBossSpace(m_NowWidth, m_NowHeight);
	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();
	onomatope->Update();

	//シールドの更新
	ShieldUpdate();
}

//描画
void BossEnemy3::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKESprite::PreDraw();
	onomatope->Draw();
	IKESprite::PostDraw();
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();
	for (auto i = 0; i < enerock.size(); i++) {
		if (enerock[i] == nullptr)continue;
		enerock[i]->Draw(dxCommon);
	}
	for (auto i = 0; i < enetornade.size(); i++) {
		if (enetornade[i] == nullptr)continue;
		enetornade[i]->Draw(dxCommon);
	}
	predictarea->Draw(dxCommon);
	if (m_Color.w != 0.0f)
		Obj_Draw();
	BaseBackDraw(dxCommon);
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	if (LastBossState::GetInstance()->GetBossShield()) {
		for (int i = 0; i < shield.size(); i++) {
			shield[i].tex->Draw();
		}
	}
	IKETexture::PostDraw();
}
//ImGui描画
void BossEnemy3::ImGui_Origin() {
	ImGui::Begin("Boss3");
	for (int i = 0; i < shield.size(); i++) {
		ImGui::Text("circle[%d].%f", i, shield[i].CircleSpeed);
	}
	ImGui::End();
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
		int l_RandState = 0;
		if (enerock.size() >= 3) {
			l_RandState = 2;
		}
		else if(enerock.size() < 3 && enerock.size() != 0) {
			l_RandState = 1;
		}
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
	m_CanCounter = false;
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
void BossEnemy3::BirthRock() {
	int l_width = {};
	int l_height = {};
	if (m_RockCount == 1) {
		l_width = 2;
		l_height = 1;
	}
	else if (m_RockCount == 2) {
		l_width = 1;
		l_height = 1;
	}
	else if (m_RockCount == 3) {
		l_width = 2;
		l_height = 2;
	}
	else {
		l_width = 1;
		l_height = 2;
	}
	std::unique_ptr<EnemyRock> newarea = std::make_unique<EnemyRock>();
	newarea->Initialize();
	newarea->InitState(l_width, l_height, { m_Position.x,m_Position.y + 2.0f,m_Position.z });
	newarea->SetPlayer(player);
	enerock.emplace_back(std::move(newarea));
	StagePanel::GetInstance()->SetRock(l_width, l_height, true);
}
//攻撃遷移
//弾
void BossEnemy3::RockAttack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_ROCK];
	if (coolTimer == 0) {
		//真ん中4マス
		BirthPredict(m_RandWidth, m_RandHeight,"Rock");
	}
	else if (coolTimer == l_TargetTimer - 60) {
		//カウンター判定
		m_CanCounter = true;
		if (m_RockCount != 4) {
			m_Jump = true;
			m_AddPower = 0.2f;
			m_Rot = true;
			m_RockCount++;
			coolTimer = l_TargetTimer - 90;
			BirthRock();
		}

		if (m_RockCount == 2) {
			predictarea->ResetPredict();
		}
	}
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		coolTimer = {};
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		m_RockCount = {};
		_charaState = STATE_SPECIAL;
	}
	predictarea->SetTargetTimer(l_TargetTimer);
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}
//ランダムマス攻撃
void BossEnemy3::RandomAttack() {
	//プレイヤーの現在マス
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_RANDOM];

	if (coolTimer == 0) {
		SelectSafeArea();
		BirthPredict(m_RandWidth, m_RandHeight, "Random");
	}
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		BirthArea(m_RandWidth, m_RandHeight, "Random");
		coolTimer = {};
		m_Jump = true;
		m_AddPower = 0.2f;
		m_Rot = true;
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
	}

	//カウンター判定
	if (coolTimer >= l_TargetTimer - 30) {
		m_CanCounter = true;
	}

	predictarea->SetTargetTimer(l_TargetTimer);
}
//周りを攻撃
void BossEnemy3::AroundAttack() {
	//プレイヤーの現在マス
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_AROUND];
	const int l_StartWidth = 3;
	const int l_StartHeight = {};
	if (m_AttackCount != 10) {
		if (coolTimer == 0) {
			//プレイヤーからの距離(-1~1)
			if (m_AttackCount == 0) {	//下から
				m_RandWidth = l_StartWidth;
				m_RandHeight = {};
			}
			else if (m_AttackCount <= 3) {		//徐々に左に
				m_RandWidth = l_StartWidth - m_AttackCount;
			}
			else if (m_AttackCount <= 6) {		//上に上がる
				m_RandWidth = {};
				m_RandHeight = l_StartHeight + (m_AttackCount - 3);
			}
			else {		//最後は戻る
				m_RandWidth++;
				m_RandHeight = 3;
			}

			BirthPredict(m_RandWidth, m_RandHeight, "Around");
		}
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 2)) {
			BirthArea(m_RandWidth, m_RandHeight, "Around");
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

	//カウンター判定
	if (coolTimer >= l_TargetTimer - 30) {
		m_CanCounter = true;
	}

	predictarea->SetTargetTimer(l_TargetTimer);
}
//攻撃エリア
void BossEnemy3::BirthArea(const int Width, const int Height, const string& name) {
	if (name == "Random") {
		int l_SoundCount = {};
		for (int i = 0; i < (PANEL_WIDTH / 2) - 1; i++) {
			for (int j = 0; j < PANEL_HEIGHT; j++) {
				if (!m_SafeArea[i][j]) {
					std::unique_ptr<EnemyTornade> newarea = std::make_unique<EnemyTornade>();
					newarea->Initialize();
					newarea->InitState(i, j);
					newarea->SetPlayer(player);
					if (l_SoundCount == 0) {
						newarea->SetSound(true);
					}
					l_SoundCount++;
					enetornade.emplace_back(std::move(newarea));
				}
			}
		}

		for (int i = 0; i < (PANEL_WIDTH / 2); i++) {
			for (int j = 0; j < PANEL_HEIGHT; j++) {
				m_SafeArea[i][j] = false;
			}
		}
	}
	else if (name == "Around") {
		std::unique_ptr<EnemyTornade> newarea = std::make_unique<EnemyTornade>();
		newarea->Initialize();
		newarea->InitState(Width, Height);
		newarea->SetPlayer(player);
		newarea->SetSound(true);
		enetornade.emplace_back(std::move(newarea));
	}
	predictarea->ResetPredict();
}

//予測エリア
void BossEnemy3::BirthPredict(const int Width, const int Height, const string& name) {
	if (name == "Random") {
		for (int i = 0; i < (PANEL_WIDTH / 2) - 1; i++) {
			for (int j = 0; j < PANEL_HEIGHT; j++) {
				if (!m_SafeArea[i][j]) {
					predictarea->SetPredict(i, j, true);
				}
			}
		}
	}
	else if (name == "Rock") {
		for (int i = 0; i < (PANEL_WIDTH / 2) - 1; i++) {
			for (int j = 0; j < PANEL_HEIGHT; j++) {
				if ((i == 1 || i == 2) && (j == 1 || j == 2)) {
					predictarea->SetPredict(i, j, true);
				}
			}
		}
	}
	else if (name == "Around") {
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
			enemywarp.AfterScale = 0.8f;
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
	const int l_TargetTimer = 200;
	const float l_AddFrame = 1 / 200.0f;
	m_Position.x = -0.75f;
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
		m_Position = { -1.0f,0.0f,5.5f };
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
			if (Helper::CheckMin(m_OverTimer, 45, 1)) {
				m_OverTimer = {};
				m_AddPower = 0.3f;
			}
		}
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
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 20.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}
void BossEnemy3::SelectSafeArea() {
	const int l_safeMax = 5;
	for (int i = 0; i < l_safeMax; i++) {
		bool isSet = false;
		//乱数の設定
		int width = Helper::GetRanNum(0, 3);
		int height = Helper::GetRanNum(0, 3);

		//パネル探索（開いてるのが3追加の場合書いてない）

		while (!isSet) {
			if (m_SafeArea[width][height]){
				width = Helper::GetRanNum(0, 3);
				height = Helper::GetRanNum(0, 3);
			}
			else {
				isSet = true;
			}
		}
		
		m_SafeArea[width][height] = true;
	}
}
//怒り状態になったときの動き
void BossEnemy3::AngerMove() {
	const int l_TargetTimer = 30;
	if (m_AngerTimer % 10 == 0) {
		XMFLOAT2 l_RandPos = {};
		l_RandPos.x = (float)Helper::GetRanNum(100, 1000);
		l_RandPos.y = (float)Helper::GetRanNum(100, 500);

		onomatope->AddOnomato(BossSpawn, l_RandPos);		//オノマトペが出る
	}
	if (Helper::CheckMin(m_AngerTimer, l_TargetTimer, 1)) {	
		m_AngerCount++;
		m_AddPower = 0.2f;
		m_Jump = true;
		m_AngerTimer = {};
		if (m_AngerCount == 3) {
			m_Rot = true;
			m_AngerFinish = true;
			m_Anger = false;
		}
	}
}
//シールドの更新
void BossEnemy3::ShieldUpdate() {
	if (!LastBossState::GetInstance()->GetBossShield()) { return; }
	const float l_AddFrame = 1 / 20.0f;
	const int l_TargetTimer = 200;

	if (_ShieldState == SHIELD_BIRTH) {		//シールドが上に上がる
		if (Helper::FrameCheck(m_ShieldFrame, l_AddFrame)) {
			for (int i = 0; i < shield.size(); i++) {
				shield[i].CircleSpeed += 2.0f;
				if (shield[i].CircleSpeed == 360.0f) {
					shield[i].CircleSpeed = {};
				}
				shield[i].pos = Helper::CircleMove(m_Position, shield[i].CircleScale, shield[i].CircleSpeed);
			}
			if (Helper::CheckMin(m_ShieldTimer, l_TargetTimer, 1)) {
				_ShieldState = SHIELD_DELETE;
				m_ShieldFrame = {};
			}
		}
		else {
			for (int i = 0; i < shield.size(); i++) {
				shield[i].scale = Ease(In, Cubic, m_ShieldFrame, shield[i].scale, 0.1f);
			}
		}
	}
	else {		//シールドが下がる
		if (Helper::FrameCheck(m_ShieldFrame, l_AddFrame)) {
			LastBossState::GetInstance()->SetBossShield(false);
			m_ShieldFrame = {};
			m_ShieldTimer = {};
			_ShieldState = SHIELD_BIRTH;
		}
		else {
			for (int i = 0; i < shield.size(); i++) {
				shield[i].scale = Ease(In, Cubic, m_ShieldFrame, shield[i].scale, 0.0f);
			}
		}
	}

	for (int i = 0; i < shield.size(); i++) {
		shield[i].pos.y = 1.0f;
		shield[i].tex->SetPosition(shield[i].pos);
		shield[i].tex->SetColor(shield[i].color);
		shield[i].tex->SetScale({ shield[i].scale,shield[i].scale,shield[i].scale });
		shield[i].tex->Update();
	}
}
void BossEnemy3::DeathSpecial() {

}