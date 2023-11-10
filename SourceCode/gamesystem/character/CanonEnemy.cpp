#include "CanonEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
//���f���ǂݍ���
CanonEnemy::CanonEnemy() {
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
}
//������
bool CanonEnemy::Initialize() {

	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "hp")));
	m_MaxHP = m_HP;
	return true;
}

void (CanonEnemy::* CanonEnemy::stateTable[])() = {
	&CanonEnemy::Inter,//�����̍���
	&CanonEnemy::Attack,//�����̍���
	&CanonEnemy::Teleport,//�u�Ԉړ�
};

//�s��
void CanonEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//�����蔻��
	vector<AttackArea*> _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
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
}
//�`��
void CanonEnemy::Draw(DirectXCommon* dxCommon) {
	//�G�̒e
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Draw(dxCommon);
		}
	}
	Obj_Draw();
}
//ImGui�`��
void CanonEnemy::ImGui_Origin() {
	ImGui::Begin("Canon");
	ImGui::Text("Height:%d,Width:%d", m_NowHeight, m_NowWidth);
	ImGui::Text("Timer:%d", m_PoisonTimer);
	ImGui::End();
}
//�J��
void CanonEnemy::Finalize() {

}
void CanonEnemy::Inter() {
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	if (coolTimer == kIntervalMax) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		BirthBullet();
	}
}

void CanonEnemy::Attack() {
	const int l_TargetTimer = 240;
	if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
		_charaState = STATE_SPECIAL;
		coolTimer = {};
	}
}

void CanonEnemy::Teleport() {
	if (Helper::GetInstance()->CheckMin(coolTimer, kIntervalMax, 1)) {
		//m_Position = randPanelPos();
		_charaState = STATE_INTER;
		coolTimer = {};
	}
}

void CanonEnemy::BirthBullet() {
	const int l_LimitTimer = 80;//��Q���������܂ł̎���
	for (int i = 0; i < BULLET_NUM; i++) {
		//��Q���̔���
		EnemyBullet* newbullet;
		newbullet = new EnemyBullet();
		newbullet->Initialize();

		newbullet->SetPolterType(TYPE_FOLLOW);
		newbullet->SetTargetTimer(i * l_LimitTimer);
		newbullet->SetBasePos(m_Position);
		newbullet->SetPosition({ m_Position.x,m_Position.y - 10.0f,m_Position.z });
		newbullet->SetCircleSpeed(i * 120.0f);
		bullets.emplace_back(newbullet);
	}
}