#include "BossEnemy.h"
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
#include <StagePanel.h>
//���f���ǂݍ���
BossEnemy::BossEnemy() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::FIRST_BOSS));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	/*shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });*/
	//�\��
	predictarea.reset(new PredictArea("LASTENEMY"));
	predictarea->Initialize();

}
//������
bool BossEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,-90.0f,0.0f };
	m_Color = { 1.f,1.f,1.f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/BossEnemy.csv", m_Limit, "Interval");

	auto AttackLimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy.csv", "ATTACK_LIMIT_NUM")));

	m_AttackLimit.resize(AttackLimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/BossEnemy.csv", m_AttackLimit, "Attack_Interval");

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
//��ԑJ��
void (BossEnemy::* BossEnemy::stateTable[])() = {
	&BossEnemy::Inter,//�����̍���
	&BossEnemy::Attack,//�����̍���
	&BossEnemy::Teleport,//�u�Ԉړ�
};
//�U���J��
void (BossEnemy::* BossEnemy::attackTable[])() = {
	&BossEnemy::RowAttack,//��U��
	&BossEnemy::RandomAttack,//�����_��
};

//�s��
void BossEnemy::Action() {
	if (!m_Induction) {
		(this->*stateTable[_charaState])();
	} else {
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

	//�U���G���A�̍X�V(���ۂ̓X�L���ɂȂ�Ǝv��)
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

//�`��
void BossEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();
	for (auto i = 0; i < enethorn.size(); i++) {
		if (enethorn[i] == nullptr)continue;
		enethorn[i]->Draw(dxCommon);
	}
	predictarea->Draw(dxCommon);
	if (m_Color.w != 0.0f)
		Obj_Draw();
	BaseBackDraw(dxCommon);
}
//ImGui�`��
void BossEnemy::ImGui_Origin() {
}
//�J��
void BossEnemy::Finalize() {

}
//�ҋ@
void BossEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		int l_RandState = Helper::GetRanNum(0, 100);	//�ǂ̍s������邩
		if (l_RandState <= 50) {
			_AttackState = ATTACK_RANDOM;
		}
		else{
			_AttackState = ATTACK_ROW;
		}
	}
}
//�U��
void BossEnemy::Attack() {
	(this->*attackTable[_AttackState])();
	PlayerCollide();
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//���[�v
void BossEnemy::Teleport() {
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
//�U���J��
//�����
void BossEnemy::RowAttack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_ROW];
	if (m_AttackCount != 4) {
		if (coolTimer == 0) {		//�\���G���A
			m_CanCounter = true;
			BirthPredict({}, m_AttackCount, "Row");
		}
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {		//���ۂ̍U��
			m_Jump = true;
			m_AddPower = 0.2f;
			m_Rot = true;
			BirthArea({}, m_AttackCount, "Row");
			coolTimer = {};
			m_AttackCount++;
			m_CanCounter = false;
		}
	} else {
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
	}

	predictarea->SetTargetTimer(l_TargetTimer);
}
//�����_���}�X�U��
void BossEnemy::RandomAttack() {
	//�v���C���[�̌��݃}�X
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	int l_TargetTimer = {};
	l_TargetTimer = m_AttackLimit[ATTACK_RANDOM];
	if (m_AttackCount != 8) {
		if (coolTimer == 0) {
			m_CanCounter = true;
			//�v���C���[����̋���(-1~1)
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
			m_CanCounter = false;
		}
	} else {
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
	}

	predictarea->SetTargetTimer(l_TargetTimer);
}
//�U���G���A
void BossEnemy::BirthArea(const int Width, const int Height, const string& name) {
	if (name == "Row") {			//�����
		int l_SoundTimer = {};
		for (auto i = 0; i < m_Area.size(); i++) {
			if (m_Area[i][Height] == 1) {		//�}�b�v�`�b�v�ԍ��ƃ^�C���̍ő吔�A�ŏ����ɉ����ĕ`�悷��
				std::unique_ptr<EnemyThorn> newarea = std::make_unique<EnemyThorn>("ARM");
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
	} else {		//�����_��(�v���C���[����߂��܂�)
		std::unique_ptr<EnemyThorn> newarea = std::make_unique<EnemyThorn>("ARM");
		newarea->Initialize();
		newarea->InitState(Width, Height);
		newarea->SetPlayer(player);
		newarea->SetSound(true);
		enethorn.emplace_back(std::move(newarea));
	}
	predictarea->ResetPredict();

}
//�\���G���A
void BossEnemy::BirthPredict(const int Width, const int Height, const string& name) {
	if (name == "Row") {			//�����
		for (auto i = 0; i < m_Area.size(); i++) {
			if (m_Area[i][Height] == 1) {		//�}�b�v�`�b�v�ԍ��ƃ^�C���̍ő吔�A�ŏ����ɉ����ĕ`�悷��
				predictarea->SetPredict(i, Height, true);
			}
		}
	} else {//�����_��(�v���C���[����߂��܂�)
		predictarea->SetPredict(Width, Height, true);
	}
	predictarea->SetFlashStart(true);
}
//�X�L����CSV��ǂݎ��
void BossEnemy::LoadCsvSkill(std::string& FileName, const int id) {

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
		} else if (word.find("AttackAreA") == 0) {
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

bool BossEnemy::CreateSkill(int id) {

	std::string directory = "Resources/csv/chara/enemy/AttackArea/Boss1/AttackArea";

	std::stringstream ss;
	if (id >= 10) {
		ss << directory << id << ".csv";

	} else {
		ss << directory << "0" << id << ".csv";
	}
	std::string csv_ = ss.str();

	LoadCsvSkill(csv_, id);

	//Player::GetInstance()->SetDelayTimer(m_Delay);
	return true;
}
//�G���A�U���̔���
void BossEnemy::PlayerCollide() {

}
//���@�w����
void BossEnemy::BirthMagic() {
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
	} else {			//���@�w���k�߂�
		if (Helper::FrameCheck(magic.Frame, addFrame)) {
			magic.Frame = {};
			magic.AfterScale = 0.2f;
			magic.Alive = false;
			magic.State = MAGIC_BIRTH;
		}
		magic.Scale = Ease(In, Cubic, magic.Frame, magic.Scale, magic.AfterScale);
	}
}
void BossEnemy::WarpEnemy() {
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel(m_LastEnemy);
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//�L�������������Ȃ�
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
	} else {			//�L�������傫���Ȃ��Ă���
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
void BossEnemy::AttackMove() {
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
void BossEnemy::ClearAction() {
	const int l_TargetTimer = 100;
	const float l_AddFrame = 1 / 200.0f;
	if (m_ClearTimer == 0) {
		m_Position.y = 10.0f;
	}

	if (Helper::CheckMin(m_ClearTimer, l_TargetTimer, 1)) {
		if (Helper::FrameCheck(m_ClearFrame, l_AddFrame)) {
			m_ClearFrame = 1.0f;
		} else {
			m_Position.y = Ease(In, Cubic, m_ClearFrame, m_Position.y, 0.1f);
		}
	}
	m_AddDisolve = {};
	Obj_SetParam();
}
//�Q�[���I�[�o�[�V�[���̍X�V
void BossEnemy::GameOverAction() {
	if (_GameOverState == OVER_STOP) {
		m_Position = { -3.0f,0.0f,1.5f };
		m_Rotation = { 0.0f,180.0f,0.0f };
		m_AddDisolve = 0.0f;
		if (player->GetOverType() == 3) {
			if (player->GetSelectType() == 1) {
				_GameOverState = OVER_YES;
				m_AddPower = 0.3f;
			}
			else if (player->GetSelectType() == 2) {
				_GameOverState = OVER_NO;
			}
		}
	} else if (_GameOverState == OVER_YES) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
			m_Position.y = 0.1f;
			m_AddPower = {};
			if (Helper::CheckMin(m_OverTimer, 25, 1)) {
				m_OverTimer = {};
				m_AddPower = 0.3f;
			}
		}
	} else {
		float l_AddRotZ = {};
		float l_AddFrame2 = {};

		l_AddRotZ = float(Helper::GetRanNum(30, 100)) / 100;
		l_AddFrame2 = float(Helper::GetRanNum(1, 10)) / 500;
		
		float RotPower = 10.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//�ŏ��̓C�[�W���O�ŉ�
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//�Ō�͓|���
				m_Rotation.z = 90.0f;
			}
		} else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 20.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}
void BossEnemy::DeathSpecial() {

}