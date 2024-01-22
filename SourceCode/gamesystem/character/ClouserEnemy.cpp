#include "ClouserEnemy.h"
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
ClouserEnemy::ClouserEnemy() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::ROCKENEMY));

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
bool ClouserEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_Scale = { 0.6f,0.6f,0.6f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ClouserEnemy.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ClouserEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/ClouserEnemy.csv", m_Limit, "Interval");

	m_MaxHP = m_HP;
	m_CheckPanel = true;
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
//��ԑJ��
void (ClouserEnemy::* ClouserEnemy::stateTable[])() = {
	&ClouserEnemy::Inter,//�����̍���
	&ClouserEnemy::Attack,//�����̍���
	&ClouserEnemy::Teleport,//�u�Ԉړ�
};

//�s��
void ClouserEnemy::Action() {
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
void ClouserEnemy::Draw(DirectXCommon* dxCommon) {
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
	predictarea->Draw(dxCommon);
	if (m_Color.w != 0.0f)
		Obj_Draw();
	BaseBackDraw(dxCommon);
}
//ImGui�`��
void ClouserEnemy::ImGui_Origin() {
	/*ImGui::Begin("Area");
	ImGui::Text("Height:%d", m_NowWidth);
	ImGui::Text("Induction:%d", m_Induction);
	ImGui::Text("InductionFrame:%f", m_InductionFrame);
	ImGui::Text("InductionPos:%f", m_InductionPos);
	ImGui::End();
	predictarea->ImGuiDraw();*/
	for (auto i = 0; i < enerock.size(); i++) {
		if (enerock[i] == nullptr)continue;
		enerock[i]->ImGui_Origin();
	}
}
//�J��
void ClouserEnemy::Finalize() {

}
//�ҋ@
void ClouserEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
	}
}
//�U��
void ClouserEnemy::Attack() {
	//�v���C���[�̌��݃}�X
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];
	if (coolTimer == 0) {
		m_CanCounter = true;
		//�v���C���[����̋���(-1~1)
		int l_RandWigth = Helper::GetRanNum(-1, 1);
		int l_RandHeight = Helper::GetRanNum(-1, 1);
		m_RandWigth = l_PlayerWidth + l_RandWigth;
		m_RandHeight = l_PlayerHeight + l_RandHeight;
		Helper::Clamp(m_RandWigth, 0, 3);
		Helper::Clamp(m_RandHeight, 0, 3);
		BirthPredict(m_RandWigth, m_RandHeight);
		StagePanel::GetInstance()->SetRock(m_RandWigth, m_RandHeight, true);
	}
	else if (coolTimer == 30) {
		m_Jump = true;
		m_AddPower = 0.2f;
		m_Rot = true;
		BirthArea(m_RandWigth, m_RandHeight);
	}
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		m_CanCounter = false;
		predictarea->ResetPredict();
		coolTimer = {};
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
	}
	predictarea->SetTargetTimer(l_TargetTimer);
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//���[�v
void ClouserEnemy::Teleport() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];

	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
	}
}

//�U���G���A
void ClouserEnemy::BirthArea(const int Width, const int Height) {
	std::unique_ptr<EnemyRock> newarea = std::make_unique<EnemyRock>();
	newarea->Initialize();
	newarea->InitState(Width, Height,{m_Position.x,m_Position.y + 2.0f,m_Position.z});
	newarea->SetPlayer(player);
	enerock.emplace_back(std::move(newarea));
}
//�\���G���A
void ClouserEnemy::BirthPredict(const int Width, const int Height) {
	predictarea->SetPredict(Width, Height, true);
	predictarea->SetFlashStart(true);
}
//���@�w����
void ClouserEnemy::BirthMagic() {
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
void ClouserEnemy::WarpEnemy() {
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
			m_Rotation.y = 270.0f;
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
			m_RandTimer = Helper::GetRanNum(0, 40);
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}

	m_Scale = { enemywarp.Scale,enemywarp.Scale, enemywarp.Scale };
}
//�U�����̓���
void ClouserEnemy::AttackMove() {
	if (!m_Rot) { return; }
	const float l_AddFrame = 1 / 20.0f;
	if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
		m_Rotation.y = 270.0f;
		m_Rot = false;
		m_AttackFrame = {};
	}

	m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 630.0f);
}
//�N���A�V�[���̍X�V
void ClouserEnemy::ClearAction() {
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
//�Q�[���I�[�o�[�V�[���̍X�V
void ClouserEnemy::GameOverAction() {
	if (_GameOverState == OVER_STOP) {
		m_Position = { -4.5f,0.0f,2.5f };
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
			if (Helper::CheckMin(m_OverTimer, 10, 1)) {
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

		float RotPower = 15.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//�ŏ��̓C�[�W���O�ŉ�
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//�Ō�͓|���
				m_Rotation.z = 90.0f;
			}
		}
		else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 22.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}