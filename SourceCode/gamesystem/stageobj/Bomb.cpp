#include "Bomb.h"
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
Bomb::Bomb() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::BULLET));
	m_Object->SetLightEffect(false);

	shockWaveTex.reset(new IKETexture(ImageManager::SHOCKWAVE, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shockWaveTex->TextureCreate();
	shockWaveTex->Initialize();
	shockWaveTex->SetRotation({ 90.0f,0.0f,0.0f });

	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });

	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>(0.5f);
		_drawnumber[i]->Initialize();
	}

	shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
	kIntervalMax = 450;
}
//������
bool Bomb::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = 1.0f;//static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/Bomb.csv", "hp")));
	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_EnemyTag = "Bomb";
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	return true;
}

void (Bomb::* Bomb::stateTable[])() = {
	&Bomb::Inter,//�����̍���
	&Bomb::Attack,//�����̍���
	&Bomb::ShockWave,
};

//�s��
void Bomb::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//�����蔻��
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//�����蔻��

	if (m_HP <= 0.0f && _charaState != STATE_SPECIAL) {
		Attack();
	}

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();
}

//�`��
void Bomb::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	if (_charaState == STATE_SPECIAL) {
		//�Ռ��g�̕`��
		shockWaveTex->Draw();
	}
	IKETexture::PostDraw();
	UIDraw();
	Obj_Draw();
}
//ImGui�`��
void Bomb::ImGui_Origin() {

}
//�J��
void Bomb::Finalize() {

}
//�ҋ@
void Bomb::Inter() {
	//�Ռ��g�o���Ă�Ԃ͎��Ԑi�܂Ȃ��悤��

	//��������
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	//���Ԑ؂�
	if (coolTimer == kIntervalMax) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
	}
}
//�U��
void Bomb::Attack() {
	if (m_HP <= 0) {
		//�G�S�̂Ƀ_���[�W
		GameStateManager::GetInstance()->SetIsBombDamage(true);
		m_shockWaveColor = { 0.0f,0.0f,1.0f,1.0f };
	}
	else {
		//�v���C���[�Ƀ_���[�W
		player->RecvDamage(10, "NORMAL");
		m_shockWaveColor = { 1.0f,0.0f,0.0f,1.0f };
	}
	//setcolor
	_charaState = STATE_SPECIAL;
}

void Bomb::ShockWave()
{
	shockWaveTex->SetPosition(m_Position.x, m_Position.y + 0.2f, m_Position.z);
	shockWaveTex->SetScale(m_shockWaveScale);
	shockWaveTex->SetColor(m_shockWaveColor);
	shockWaveTex->Update();

	m_shockWaveTimer++;
	m_shockWaveTimer = clamp(m_shockWaveTimer, 0, 30);
	//�^�C�}�[�ɉ����ďՌ��g�̑傫���ύX����
	m_shockWaveScale.x += 0.15f;
	m_shockWaveScale.y += 0.15f;
	m_shockWaveScale.z += 0.15f;
	//���Ԑ؂�
	if (m_shockWaveTimer == 30) {
		m_Alive = false;
		m_shockWaveTimer = 0;
	}
	//�Ռ��g���o��
	//�o���I�������m_alive��false��
}
