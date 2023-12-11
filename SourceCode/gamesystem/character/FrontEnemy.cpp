#include "FrontEnemy.h"
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
FrontEnemy::FrontEnemy() {
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

	//�\��
	predictarea.reset(new PredictArea("ENEMY"));
	predictarea->Initialize();
}
//������
bool FrontEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_Color = { 0.0f,1.0f,0.5f,1.0f };
	m_Scale = { 0.4f,0.4f,0.4f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/FrontEnemy.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/FrontEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/FrontEnemy.csv", m_Limit, "Interval");

	auto AttackLimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/FrontEnemy.csv", "ATTACK_LIMIT_NUM")));

	m_AttackLimit.resize(AttackLimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/FrontEnemy.csv", m_AttackLimit, "Attack_Interval");

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
	enemywarp.Scale = 0.5f;
	m_AddDisolve = 2.0f;
	return true;
}
//��ԑJ��
void (FrontEnemy::* FrontEnemy::stateTable[])() = {
	&FrontEnemy::Inter,//�����̍���
	&FrontEnemy::Attack,//�U��
	&FrontEnemy::Teleport,//�u�Ԉړ�
};
//�U���J��
void (FrontEnemy::* FrontEnemy::attackTable[])() = {
	&FrontEnemy::FrontPlayerWarp,//�v���C���[�̑O�Ɉړ�
	&FrontEnemy::FrontAttack,//���ʂɍU��
};

//�s��
void FrontEnemy::Action() {
	(this->*stateTable[_charaState])();
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
void FrontEnemy::Draw(DirectXCommon* dxCommon) {
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
//ImGui�`��
void FrontEnemy::ImGui_Origin() {
	ImGui::Begin("Area");
	ImGui::Text("AttackCount:%d", m_AttackCount);
	ImGui::Text("cool:%d", coolTimer);
	ImGui::End();
	predictarea->ImGuiDraw();
}
//�J��
void FrontEnemy::Finalize() {

}
//�ҋ@
void FrontEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		int l_RandState = Helper::GetRanNum(0, 2);
		_AttackState = (AttackState)(l_RandState);
	}
}
//�U��
void FrontEnemy::Attack() {
	(this->*attackTable[_AttackState])();
	PlayerCollide();
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//���[�v
void FrontEnemy::Teleport() {
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
void FrontEnemy::BirthArea(const int Width, const int Height, const string& name) {
	if (name == "Row") {			//�����
		for (auto i = 0; i < m_Area.size(); i++) {
			if (m_Area[i][Height] == 1) {		//�}�b�v�`�b�v�ԍ��ƃ^�C���̍ő吔�A�ŏ����ɉ����ĕ`�悷��
				std::unique_ptr<EnemyThorn> newarea = std::make_unique<EnemyThorn>();
				newarea->Initialize();
				newarea->InitState(i, Height);
				newarea->SetPlayer(player);
				enethorn.emplace_back(std::move(newarea));
			}
		}
	}
	else {		//�����_��(�v���C���[����߂��܂�)
		std::unique_ptr<EnemyThorn> newarea = std::make_unique<EnemyThorn>();
		newarea->Initialize();
		newarea->InitState(Width, Height);
		newarea->SetPlayer(player);
		enethorn.emplace_back(std::move(newarea));
	}
	predictarea->ResetPredict();

}
//�\���G���A
void FrontEnemy::BirthPredict(const int Width, const int Height, const string& name) {
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
	predictarea->SetFlashStart(true);
}

//���@�w����
void FrontEnemy::BirthMagic() {
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
void FrontEnemy::WarpEnemy() {
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
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}

	m_Scale = { enemywarp.Scale,enemywarp.Scale, enemywarp.Scale };
}
//�U�����̓���
void FrontEnemy::AttackMove() {
	if (!m_Rot) { return; }
	const float l_AddFrame = 1 / 20.0f;
	if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
		m_Rotation.y = 270.0f;
		m_Rot = false;
		m_AttackFrame = {};
	}

	m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 630.0f);
}
//�v���C���[�̑O�Ɉړ�
void FrontEnemy::FrontPlayerWarp() {

}
//���ʂɍU��
void FrontEnemy::FrontAttack() {

}