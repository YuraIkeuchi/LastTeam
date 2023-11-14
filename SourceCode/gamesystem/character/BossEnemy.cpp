#include "BossEnemy.h"
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
BossEnemy::BossEnemy() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	m_Object->SetLightEffect(false);
	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });

	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>();
		_drawnumber[i]->Initialize();
	}

	shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
	//�\��
	predictarea.reset(new PredictArea());
	predictarea->Initialize();
}
//������
bool BossEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 0.0f,1.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy.csv", "hp")));
	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	CreateSkill(1);
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
	&BossEnemy::BulletAttack,//�e��łU��
	&BossEnemy::RowAttack,//��U��
	&BossEnemy::RandomAttack,//�����_��
};

//�s��
void BossEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//�����蔻��
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//�����蔻��
	PoisonState();//��

	//�G�̒e
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Update();
		}
	}

	//��Q���̍폜
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i] == nullptr) {
			continue;
		}

		if (!bullets[i]->GetAlive()) {
			bullets.erase(cbegin(bullets) + i);
		}
	}

	//�U���G���A�̍X�V(���ۂ̓X�L���ɂȂ�Ǝv��)
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Update();

		if (!attackarea[i]->GetAlive()) {
			attackarea.erase(cbegin(attackarea) + i);
		}
	}
	

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();
}

//�`��
void BossEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	IKETexture::PostDraw();
	//�G�̒e
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Draw(dxCommon);
		}
	}
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Draw(dxCommon);
	}
	predictarea->Draw(dxCommon);
	Obj_Draw();
}
//ImGui�`��
void BossEnemy::ImGui_Origin() {
	ImGui::Begin("Area");
	ImGui::Text("AttackCount:%d", m_AttackCount);
	ImGui::Text("cool:%d", coolTimer);
	ImGui::End();
	predictarea->ImGuiDraw();
}
//�J��
void BossEnemy::Finalize() {

}
//�ҋ@
void BossEnemy::Inter() {
	const int l_TargetTimer = 120;
	if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		int l_RandState = Helper::GetInstance()->GetRanNum(0, 2);
		_AttackState = (AttackState)(l_RandState);
	}
}
//�U��
void BossEnemy::Attack() {
	(this->*attackTable[_AttackState])();
	PlayerCollide();
	predictarea->Update();
}

//���[�v
void BossEnemy::Teleport() {
	const int l_TargetTimer = 120;
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel();
	if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
		//m_Position = randPanelPos();
		_charaState = STATE_INTER;
		coolTimer = {};
		m_Position = l_RandPos;
		StagePanel::GetInstance()->EnemyHitReset();
	}
}
//�e�̐���
void BossEnemy::BirthBullet() {
	//��Q���̔���
	EnemyBullet* newbullet;
	newbullet = new EnemyBullet();
	newbullet->Initialize();

	newbullet->SetPolterType(TYPE_FOLLOW);
	newbullet->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
	bullets.emplace_back(newbullet);
}
//�U���J��
//�e
void BossEnemy::BulletAttack() {
	const int l_TargetTimer = 70;

	if (_BossType == Boss_SET) {
		if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
			coolTimer = {};
			_BossType = Boss_THROW;
		}
	}
	else if (_BossType == Boss_THROW) {
		m_AttackCount++;
		BirthBullet();
		if (m_AttackCount != 5) {
			_BossType = Boss_SET;
		}
		else {
			_BossType = Boss_END;
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
//�����
void BossEnemy::RowAttack() {
	const int l_TargetTimer = 60;
	if (m_AttackCount != 4) {
		if (coolTimer == 0) {		//�\���G���A
			BirthPredict({}, m_AttackCount,"Row");
		}
		if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {		//���ۂ̍U��
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
}
//�����_���}�X�U��
void BossEnemy::RandomAttack() {
	auto player_data = GameStateManager::GetInstance()->GetPlayer().lock();
	//�v���C���[�̌��݃}�X
	int l_PlayerWidth = player_data->GetNowWidth();
	int l_PlayerHeight = player_data->GetNowHeight();
	const int l_TargetTimer = 60;
	if (m_AttackCount != 8) {
		if (coolTimer == 0) {
			//�v���C���[����̋���(-1~1)
			int l_RandWigth = Helper::GetInstance()->GetRanNum(-1, 1);
			int l_RandHeight = Helper::GetInstance()->GetRanNum(-1, 1);
			m_RandWigth = l_PlayerWidth + l_RandWigth;
			m_RandHeight = l_PlayerHeight + l_RandHeight;
			Helper::GetInstance()->Clamp(m_RandWigth, 0, 3);
			Helper::GetInstance()->Clamp(m_RandHeight, 0, 3);
			BirthPredict(m_RandWigth, m_RandHeight, "Random");
		}
		if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
			BirthArea(m_RandWigth, m_RandHeight, "Random");
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
}
//�U���G���A
void BossEnemy::BirthArea(const int Width, const int Height, const string& name) {
	if (name == "Row") {			//�����
		for (auto i = 0; i < m_Area.size(); i++) {
			if (m_Area[i][Height] == 1) {		//�}�b�v�`�b�v�ԍ��ƃ^�C���̍ő吔�A�ŏ����ɉ����ĕ`�悷��
				std::unique_ptr<AttackArea> newarea = std::make_unique<AttackArea>();
				newarea->Initialize();
				newarea->InitState(i, Height);
				newarea->SetDamage(20.0f);
				newarea->SetName("Enemy");
				attackarea.emplace_back(std::move(newarea));
			}
		}
	}
	else {		//�����_��(�v���C���[����߂��܂�)
		std::unique_ptr<AttackArea> newarea = std::make_unique<AttackArea>();
		newarea->Initialize();
		newarea->InitState(Width, Height);
		newarea->SetDamage(20.0f);
		newarea->SetName("Enemy");
		attackarea.emplace_back(std::move(newarea));
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
	}
	else {//�����_��(�v���C���[����߂��܂�)
		predictarea->SetPredict(Width, Height, true);
	}
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

bool BossEnemy::CreateSkill(int id) {

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
void BossEnemy::PlayerCollide() {
	auto player_data = GameStateManager::GetInstance()->GetPlayer().lock();
	int l_PlayerWidth = player_data->GetNowWidth();
	int l_PlayerHeight = player_data->GetNowHeight();
	for (unique_ptr<AttackArea>& newarea : attackarea) {
		if (newarea != nullptr) {
			if ((newarea->GetNowHeight() == l_PlayerHeight && newarea->GetNowWidth() == l_PlayerWidth) &&
				!newarea->GetHit() && (newarea->GetName() == "Enemy")) {
				player_data->RecvDamage(20.0f);
				newarea->SetHit(true);
				break;
			}
		}
	}
}