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
//���f���ǂݍ���
BossEnemy3::BossEnemy3() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::LAST_BOSS));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	//�\��
	predictarea.reset(new PredictArea("ENEMY"));
	predictarea->Initialize();

}
//������
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
	enemywarp.Scale = 0.8f;
	m_AddDisolve = 2.0f;

	for (int i = 0; i < PANEL_WIDTH / 2; i++) {
		for (int j = 0; j < PANEL_WIDTH; j++) {
			m_SafeArea[i][j] = false;
		}
	}
	return true;
}
//��ԑJ��
void (BossEnemy3::* BossEnemy3::stateTable[])() = {
	&BossEnemy3::Inter,//�����̍���
	&BossEnemy3::Attack,//�����̍���
	&BossEnemy3::Teleport,//�u�Ԉړ�
};
//�U���J��
void (BossEnemy3::* BossEnemy3::attackTable[])() = {
	&BossEnemy3::RockAttack,//�e��łU��
	&BossEnemy3::RandomAttack,//�����_��
};

//�s��
void BossEnemy3::Action() {
	if (!m_Induction) {
		(this->*stateTable[_charaState])();
	}
	else {
		InductionMove();
	}
	Obj_SetParam();
	//�����蔻��
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//�����蔻��
	PoisonState();//��
	BirthMagic();//���@�w
	AttackMove();//�U�����̓���
	//�U�����W�����v����
	if (m_Jump) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
			m_AddPower = {};
			m_Jump = false;
			m_Position.y = 0.1f;
		}
	}

	//�G�̊�
	//�U���G���A�̍X�V(���ۂ̓X�L���ɂȂ�Ǝv��)
	for (auto i = 0; i < enetornade.size(); i++) {
		if (enetornade[i] == nullptr)continue;
		enetornade[i]->Update();

		if (!enetornade[i]->GetAlive()) {
			enetornade.erase(cbegin(enetornade) + i);
		}
	}

	//��G���A�̍X�V
	for (auto i = 0; i < enerock.size(); i++) {
		if (enerock[i] == nullptr)continue;
		enerock[i]->Update();

		if (!enerock[i]->GetAlive()) {
			enerock.erase(cbegin(enerock) + i);
		}
	}

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();
}

//�`��
void BossEnemy3::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
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
}
//ImGui�`��
void BossEnemy3::ImGui_Origin() {
	//ImGui::Begin("Boss");
	//ImGui::Text("PosX:%f", m_Position.x);
	//ImGui::Text("PosZ:%f", m_Position.z);
	//ImGui::End();
	//predictarea->ImGuiDraw();
}
//�J��
void BossEnemy3::Finalize() {

}
//�ҋ@
void BossEnemy3::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		int l_RandState = 1;
		_AttackState = (AttackState)(l_RandState);
	}
}
//�U��
void BossEnemy3::Attack() {
	(this->*attackTable[_AttackState])();
	PlayerCollide();
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//���[�v
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
//�e�̐���
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
//�U���J��
//�e
void BossEnemy3::RockAttack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_ROCK];
	if (coolTimer == 0) {
		//�^��4�}�X
		BirthPredict(m_RandWigth, m_RandHeight,"Rock");
	}
	else if (coolTimer == l_TargetTimer - 60) {
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
//�����_���}�X�U��
void BossEnemy3::RandomAttack() {
	//�v���C���[�̌��݃}�X
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_RANDOM];

	if (coolTimer == 0) {
		//�v���C���[����̋���(-1~1)
		int l_RandWigth = Helper::GetRanNum(-1, 1);
		int l_RandHeight = Helper::GetRanNum(-1, 1);
		m_RandWigth = l_PlayerWidth + l_RandWigth;
		m_RandHeight = l_PlayerHeight + l_RandHeight;
		Helper::Clamp(m_RandWigth, 0, 3);
		Helper::Clamp(m_RandHeight, 0, 3);
		SelectSafeArea();
		BirthPredict(m_RandWigth, m_RandHeight, "Random");
	}
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		BirthArea(m_RandWigth, m_RandHeight, "Random");
		coolTimer = {};
		m_Jump = true;
		m_AddPower = 0.2f;
		m_Rot = true;
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
	}

	predictarea->SetTargetTimer(l_TargetTimer);
}
//�U���G���A
void BossEnemy3::BirthArea(const int Width, const int Height, const string& name) {
	for (int i = 0; i < (PANEL_WIDTH / 2) - 1; i++) {
		for (int j = 0; j < PANEL_WIDTH; j++) {
			if (!m_SafeArea[i][j]) {
				std::unique_ptr<EnemyTornade> newarea = std::make_unique<EnemyTornade>();
				newarea->Initialize();
				newarea->InitState(i, j);
				newarea->SetPlayer(player);
				newarea->SetSound(true);
				enetornade.emplace_back(std::move(newarea));
			}
		}
	}

	for (int i = 0; i < (PANEL_WIDTH / 2); i++) {
		for (int j = 0; j < PANEL_WIDTH; j++) {
			m_SafeArea[i][j] = false;
		}
	}
	predictarea->ResetPredict();
}

//�\���G���A
void BossEnemy3::BirthPredict(const int Width, const int Height, const string& name) {
	if (name == "Random") {
		for (int i = 0; i < (PANEL_WIDTH / 2) - 1; i++) {
			for (int j = 0; j < PANEL_WIDTH; j++) {
				if (!m_SafeArea[i][j]) {
					predictarea->SetPredict(i, j, true);
				}
			}
		}
	}
	else if (name == "Rock") {
		for (int i = 0; i < (PANEL_WIDTH / 2) - 1; i++) {
			for (int j = 0; j < PANEL_WIDTH; j++) {
				if ((i == 1 || i == 2) && (j == 1 || j == 2)) {
					predictarea->SetPredict(i, j, true);
				}
			}
		}
	}

	predictarea->SetFlashStart(true);
}
//�X�L����CSV��ǂݎ��
void BossEnemy3::LoadCsvSkill(std::string& FileName, const int id) {

	std::ifstream file;
	std::stringstream popcom;

	file.open(FileName);
	popcom << file.rdbuf();
	file.close();

	std::string line;
	//�A�^�b�N�G���A�p
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
//�G���A�U���̔���
void BossEnemy3::PlayerCollide() {

}
//���@�w����
void BossEnemy3::BirthMagic() {
	if (!magic.Alive) { return; }
	static float addFrame = 1.f / 15.f;
	const int l_TargetTimer = 20;
	if (magic.State == MAGIC_BIRTH) {			//���@�w���L����
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
	else {			//���@�w���k�߂�
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
	if (enemywarp.State == WARP_START) {			//�L�������������Ȃ�
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
	else {			//�L�������傫���Ȃ��Ă���
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
//�U�����̓���
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
//�N���A�V�[���̍X�V
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
//�Q�[���I�[�o�[�V�[���̍X�V
void BossEnemy3::GameOverAction() {
	if (_GameOverState == OVER_STOP) {
		m_Position = { -1.0f,0.0f,4.5f };
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

		l_AddRotZ = float(Helper::GetRanNum(3, 10)) / 10;
		l_AddFrame2 = float(Helper::GetRanNum(1, 10)) / 100;
		float RotPower = 10.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//�ŏ��̓C�[�W���O�ŉ�
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//�Ō�͓|���
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
	const int l_safeMax = 3;
	for (int i = 0; i < l_safeMax; i++) {
		bool isSet = false;
		//�����̐ݒ�
		int width = Helper::GetRanNum(0, 3);
		int height = Helper::GetRanNum(0, 3);

		//�p�l���T���i�J���Ă�̂�3�ǉ��̏ꍇ�����ĂȂ��j

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