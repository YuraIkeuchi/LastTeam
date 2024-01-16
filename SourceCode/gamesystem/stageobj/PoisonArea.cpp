#include "PoisonArea.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <ModelManager.h>
#include <GameStateManager.h>
#include <Helper.h>
#include <ParticleEmitter.h>
//�ǂݍ���
PoisonArea::PoisonArea() {
	panels.tex.reset(new IKETexture(ImageManager::POISONAREA, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	panels.tex->TextureCreate();
	panels.tex->Initialize();
	panels.tex->SetScale({});
	panels.tex->SetRotation({ 90.0f,0.0f,0.0f });

	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BULLET);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	Initialize();
}
//������
bool PoisonArea::Initialize() {
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "POISON_DAMAGE")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "POISON_TIMER")));
	return true;
}

//�X�e�[�^�X������
void PoisonArea::InitState(const int width, const int height) {
	//�p�l����
	panels.position = SetPanelPos(width, height);
	panels.Width = width, panels.Height = height;
	panels.position.y = 0.011f;
	panels.color = { 1.0f,1.0f,1.0f,1.0f };
	panels.Alive = true;
	panels.Timer = 40;
	//�e
	m_BulletAlive = true;
	float baseScale = PANEL_SIZE * 0.1f;
	m_Scale = { baseScale,baseScale,baseScale };
	m_Color = { 1.0f,0.5f,0.5f,1.0f };
}

//�X�V
void PoisonArea::Update() {
	const XMFLOAT4 s_color = { 1.f,0.0f,0.0f,1.0f };
	const XMFLOAT4 e_color = { 1.f,0.5f,0.0f,1.0f };
	const float s_scale = 0.5f;
	const float e_scale = 0.0f;
	ParticleEmitter::GetInstance()->PoisonEffect(
	50, { m_Position.x,m_Position.y + 1.0f,m_Position.z }, s_scale, e_scale, s_color, e_color, 0.1f, 3.0f);

	Obj_SetParam();
	Collide();
	Move();
	panels.tex->Update();
	panels.tex->SetPosition(panels.position);
	panels.tex->SetColor(panels.color);
	panels.tex->SetScale({ panels.scale,panels.scale,panels.scale });
}
//�`��
void PoisonArea::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	panels.tex->Draw();
	IKETexture::PostDraw();
	if(m_BulletAlive)
	Obj_Draw();
}
//ImGui
void PoisonArea::ImGuiDraw() {
	ImGui::Begin("Poison");
	ImGui::Text("Width:%d,Height:%d", panels.Width, panels.Height);
	ImGui::End();
}
//�p�l���̈ʒu�ɒu��
XMFLOAT3 PoisonArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
void PoisonArea::Collide() {
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	if (_PoisonState != POISON_WIDE) { return; }
	//�ł̃}�X�ƃv���C���[���ꏏ���ƃ_���[�W��H�炤
	if (panels.Width == l_PlayerWidth && panels.Height == l_PlayerHeight) {
		panels.DamageTimer++;
	}
	else {
		if (panels.Damage) {
			panels.DamageTimer = {};
		}
	}

	//
	if (panels.DamageTimer == 20) {
		player->RecvDamage(m_Damage,"POISON");
		panels.DamageTimer = {};
		panels.Damage = true;
	}
}
void PoisonArea::Move() {
	const float l_TargetPosY = 15.0f;
	const float l_ThrowSpeed = 0.25f;

	if (_PoisonState == POISON_THROW) {			//��ɏグ��
		if (Helper::CheckMin(m_Position.y, l_TargetPosY, l_ThrowSpeed)) {
			_PoisonState = POISON_DROP;
			m_Position = { panels.position.x,l_TargetPosY,panels.position.z };
		}
	}
	else if (_PoisonState == POISON_DROP) {		//�����Ă���
		if (Helper::CheckMax(m_Position.y, 0.0f, -l_ThrowSpeed)) {
			_PoisonState = POISON_WIDE;
			m_BulletAlive = false;
		}
	}
	else if (_PoisonState == POISON_WIDE) {		//�L����
		static float addFrame = 1.f / 15.f;
		if (Helper::FrameCheck(panels.frame, addFrame)) {		//�L���邽�߂̃C�[�W���O
			if (Helper::CheckMin(panels.Timer, m_TargetTimer, 1)) {		//�L����؂�����J�E���g�����Z����
				_PoisonState = POISON_END;
				panels.frame = {};
				panels.afterscale = {};
			}
		}
		panels.scale = Ease(In, Cubic, panels.frame, panels.scale, panels.afterscale);
	}
	else {		//�Ȃ��Ȃ�
		static float addFrame = 1.f / 15.f;
		if (Helper::FrameCheck(panels.frame, addFrame)) {
			panels.Alive = false;
		}
		panels.scale = Ease(In, Cubic, panels.frame, panels.scale, panels.afterscale);
	}

}
