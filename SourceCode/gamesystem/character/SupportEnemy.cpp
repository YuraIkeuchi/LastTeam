#include "SupportEnemy.h"
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
SupportEnemy::SupportEnemy(const int Num) {
	m_SupportType = Num;
	if (m_SupportType == SUPPORT_RED) {
		BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::SECOND_BOSS));
	}
	else {
		BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::SUPPORT_ENEMY));
	}

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	/*shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });*/
	//�\��
	predictarea.reset(new PredictArea("ENEMY"));
	predictarea->Initialize();
}
//������
bool SupportEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,-90.0f,0.0f };
	//m_Color = { 1.f,0.f,1.f,1.0f };
	m_Scale = { 0.4f,0.4f,0.4f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/SupportEnemy.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/SupportEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/SupportEnemy.csv", m_Limit, "Interval");

	auto AttackLimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/SupportEnemy.csv", "ATTACK_LIMIT_NUM")));

	m_AttackLimit.resize(AttackLimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/SupportEnemy.csv", m_AttackLimit, "Attack_Interval");

	m_BulletNum = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/SupportEnemy.csv", "BULLET_NUM")));

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
	enemywarp.Scale = 0.8f;
	m_AddDisolve = 2.0f;
	return true;
}
//��ԑJ��
void (SupportEnemy::* SupportEnemy::stateTable[])() = {
	&SupportEnemy::Inter,//�����̍���
	&SupportEnemy::Attack,//�����̍���
	&SupportEnemy::Teleport,//�u�Ԉړ�
};

//�s��
void SupportEnemy::Action() {
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

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();
}

//�`��
void SupportEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();
	predictarea->Draw(dxCommon);
	if (m_Color.w != 0.0f)
		Obj_Draw();
	BaseBackDraw(dxCommon);
}
//ImGui�`��
void SupportEnemy::ImGui_Origin() {
	//ImGui::Begin("Boss");
	//ImGui::Text("PosX:%f", m_Position.x);
	//ImGui::Text("PosZ:%f", m_Position.z);
	//ImGui::End();
	//predictarea->ImGuiDraw();
}
//�J��
void SupportEnemy::Finalize() {

}
//�ҋ@
void SupportEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
	}
}
//�U��
void SupportEnemy::Attack() {
	//(this->*attackTable[_AttackState])();
	//PlayerCollide();
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//���[�v
void SupportEnemy::Teleport() {
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
//�U���G���A
void SupportEnemy::BirthArea(const int Width, const int Height, const string& name) {

}
//�\���G���A
void SupportEnemy::BirthPredict(const int Width, const int Height, const string& name) {
	
}

//���@�w����
void SupportEnemy::BirthMagic() {
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
void SupportEnemy::WarpEnemy() {
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
void SupportEnemy::AttackMove() {
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
void SupportEnemy::ClearAction() {
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
//�Q�[���I�[�o�[�V�[���̍X�V
void SupportEnemy::GameOverAction() {
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
