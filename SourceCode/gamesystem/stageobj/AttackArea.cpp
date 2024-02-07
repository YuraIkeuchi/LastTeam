#include "AttackArea.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <GameStateManager.h>
#include <Audio.h>
#include <Helper.h>
//�ǂݍ���
AttackArea::AttackArea(string& userName, string& stateName) {
	this->m_Name = userName;
	this->StateName = stateName;

	if (StateName == "POISON" || StateName == "VENOM" || StateName == "PASSIVEPOISON") {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DORO);
		_EffectState = Poison;
	}
	else if(StateName == "SLASH") {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::ZASHU);
		_EffectState = Slash;
	}
	else if (StateName == "SPEAR" || StateName == "ARROW") {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::GUSA);
		_EffectState = Spear;
	}
	else if (StateName == "SHUFFLE") {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::HATENA);
		_EffectState = Hatena;
	}
	else if (StateName == "DRAIN" || StateName == "PASSIVEDRAIN") {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::KIRA);
		_EffectState = Heal;
	}
	else if (StateName == "FAR" || StateName == "NEAR") {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BYUNN);
		_EffectState = Aero;
	}
	else {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DOGO);
		_EffectState = Stone;
	}
	UINT texNum = ImageManager::AREA;
	if (this->m_Name =="Player") {
		texNum = ImageManager::PLAYERAREA;
	}
	panels.tex = std::make_unique<IKETexture>(texNum, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,1.f,1.f,1.f });
	panels.tex->TextureCreate();
	panels.tex->Initialize();
	float baseScale = PANEL_SIZE * 0.1f;
	panels.tex->SetScale({ baseScale,baseScale,baseScale });
	panels.tex->SetRotation({ 90.0f,0.0f,0.0f });

	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	Initialize();
}
//��ԑJ��
/*CharaState��State���я��ɍ��킹��*/
void (AttackArea::* AttackArea::stateTable[])() = {
	&AttackArea::SlashMove,//�a��
	&AttackArea::StoneMove,//�◎�Ƃ�
	&AttackArea::PoisonMove,//�Ōn
	&AttackArea::SpearMove,//���n
	&AttackArea::HatenaMove,//�͂Ă�
	&AttackArea::HealMove,//�񕜂̓���
	&AttackArea::AeroMove,//�����񂹂Ƃ�
};
//������
bool AttackArea::Initialize() {
	return true;
}
//�X�e�[�^�X������
void AttackArea::InitState(const int width, const int height) {
	panels.position = SetPanelPos(width, height);
	panels.color = { 1.f,1.f,1.f,1.f };
	m_NowWidth = width, m_NowHeight = height;
	panels.position.y = 0.03f;
	m_Alive = true;
	m_Hit = false;
	m_Timer = false;
	m_Rotation = { 0.0f,0.0f,0.0f };
	if (_EffectState == Poison) {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.2f,0.2f,0.2f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		//m_Object->SetBillboard(true);
	}
	else if (_EffectState == Slash) {
		m_Rotation = { 45.0f,270.0f,0.0f };
		m_Scale = { 0.3f,0.3f,0.3f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		m_Object->SetBillboard(true);
	}
	else if (_EffectState == Spear) {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.7f,0.7f,0.7f };
		m_Position = { panels.position.x,5.0f,panels.position.z };
		//m_Object->SetBillboard(true);
	}
	else if (_EffectState == Hatena) {
		m_Rotation.y = 270.0f;
		m_Scale = { 1.5f,1.5f,1.5f };
		m_Position = { panels.position.x,-1.0f,panels.position.z };
		m_Color = { 0.8f,0.3f,0.2f,1.0f };
		//m_Object->SetBillboard(true);
	}
	else if (_EffectState == Heal) {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.75f,0.75f,0.75f };
		m_Position = { panels.position.x,-1.0f,panels.position.z };
		m_Color = { 0.6f,0.9f,0.2f,1.0f };
	}
	else if (_EffectState == Aero) {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.7f,0.7f,0.7f };
		if (StateName == "FAR") {
			m_Position = { panels.position.x - 0.7f,-1.0f,panels.position.z };
		}
		else {
			m_Position = { panels.position.x + 0.7f,-1.0f,panels.position.z };
		}
		m_Object->SetBillboard(true);
	}
	else {
		m_AddDisolve = 0.5f;
		m_Rotation.y = 270.0f;
		m_Scale = { 0.2f,0.2f,0.2f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		m_Object->SetBillboard(true);
	}
}
//�X�V
void AttackArea::Update() {
	const int l_TargetTimer = 10;
	panels.color = { 1.0f,0.3f,0.0f,1.0f };
	if (this->m_Name == "Player") {
		panels.color = { 1.0f,1.0f,1.0f,1.0f };
	}
	panels.tex->Update();
	panels.tex->SetPosition(panels.position);
	panels.tex->SetColor(panels.color);
	m_BirthTimer++;
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_EffectState])();
	Obj_SetParam();
}
//�`��
void AttackArea::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	panels.tex->Draw();
	IKETexture::PostDraw();
	if (m_Timer <= m_BirthTimer && m_Color.w >= 0.1f) {
		Obj_Draw();
	}
}
//ImGui
void AttackArea::ImGuiDraw() {
	ImGui::Begin("Attack");
	ImGui::Text("Sound:%d", m_Timer);
	ImGui::End();
}
//�p�l���̈ʒu�ɒu��
XMFLOAT3 AttackArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
//�a���n
void AttackArea::SlashMove() {
	if (m_Timer > m_BirthTimer) { return; }
	float l_AddFrame = {};
	if (_EffectType == EFFECT_FALL) {
		l_AddFrame = 1 / 20.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			if (!m_Hit) {
				_EffectType = EFFECT_BOUND;
			}
			else {
				_EffectType = EFFECT_HIT;
			}
			m_Frame = {};
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.5f);
			m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, -90.0f);
		}
		//������x�X������U������
		if (m_Rotation.x <= -20.0f) {
			m_Attack = true;
			/// <summary>
			///	������(�a������])
			/// </summary>
			if (m_Sound) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/slash.wav", 0.02f);
				m_Sound = false;
			}
		}
	}
	else if(_EffectType == EFFECT_BOUND) {
		l_AddFrame = 1 / 20.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,0.0f),
			Ease(In,Cubic,m_Frame,m_Scale.y,0.0f),
			Ease(In,Cubic,m_Frame,m_Scale.z,0.0f), };
		}
	}
	else {
		l_AddFrame = 1 / 40.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 3.0f);
			m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, 0.0f);
			//m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);

			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,0.5f),
			Ease(In,Cubic,m_Frame,m_Scale.y,0.5f),
			Ease(In,Cubic,m_Frame,m_Scale.z,0.5f), };
		}

		if (m_Frame > 0.5f) {
			if (Helper::CheckMax(m_Color.w, 0.0f, -0.1f)) {
				m_Alive = false;
				m_Frame = {};
			}
		}
	}
}
//�◎�Ƃ��n
void AttackArea::StoneMove() {
	if (m_Timer > m_BirthTimer) { return; }
	float l_AddFrame = {};
	if (_EffectType == EFFECT_FALL) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 1.0f, m_AddPower)) {
			m_AddPower = 0.2f;
			if (!m_Hit) {
				_EffectType = EFFECT_BOUND;
			}
			else {
				_EffectType = EFFECT_HIT;
			}
		}
		//������x�̍����ɂȂ�����U������
		if (m_Position.y <= 2.5f) {
			m_Attack = true;
			/// <summary>
			///	����(�₪�����閔�͌����鉹��])
			/// </summary>
			if (m_Sound) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/heavyRockCollapse.wav", 0.02f);
				m_Sound = false;
			}
		}
	}
	else if(_EffectType == EFFECT_BOUND) {
		m_Attack = false;
		m_AddPower -= m_Gravity;
		m_Scale = Helper::Float3AddFloat(m_Scale, 0.02f);
		Helper::CheckMax(m_Position.y, 1.0f, m_AddPower);
		if (Helper::CheckMin(m_AddDisolve, 2.5f, 0.1f)) {
			m_Alive = false;
		}
	}
	else {
		l_AddFrame = 1 / 60.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_AddPower = {};
			_EffectType = EFFECT_BOUND;
			m_Frame = {};
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 3.0f);
			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,0.4f),
			Ease(In,Cubic,m_Frame,m_Scale.y,0.4f),
			Ease(In,Cubic,m_Frame,m_Scale.z,0.4f), };
		}
	}
}
//�Ōn
void AttackArea::PoisonMove() {
	const XMFLOAT3 l_AfterScale = { 1.35f,0.05f,0.35f };
	float l_AddFrame = {};
	if (m_Timer > m_BirthTimer) { return; }
	if (_EffectType == EFFECT_FALL) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 1.0f, m_AddPower)) {
			if (!m_Hit) {
				_EffectType = EFFECT_BOUND;
			}
			else {
				_EffectType = EFFECT_HIT;
			}
			m_AddPower = 0.2f;
		}
		//������x�̍����ɂȂ�����U������
		if (m_Position.y <= 2.5f) {
			m_Attack = true;
			/// <summary>
			///	����(�h���h���������̂��n�ʂɗ����鉹��](�x�`���b�݂����Ȃ��)
			/// </summary>
			if (m_Sound) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Poison.wav", 0.02f);
				m_Sound = false;
			}
		}
	}
	else if(_EffectType == EFFECT_BOUND) {
		l_AddFrame = 1 / 30.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,l_AfterScale.x),
			Ease(In,Cubic,m_Frame,m_Scale.y,l_AfterScale.y),
			Ease(In,Cubic,m_Frame,m_Scale.z,l_AfterScale.z), };

			m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, {});
		}
	}
	else {
		l_AddFrame = 1 / 60.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
			m_Frame = {};
		}
		else {
			m_Rotation.z = Ease(In, Cubic, m_Frame, m_Rotation.z, -45.0f);
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 3.0f);
			m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);

			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,0.5f),
			Ease(In,Cubic,m_Frame,m_Scale.y,0.5f),
			Ease(In,Cubic,m_Frame,m_Scale.z,0.5f), };
		}
	}
}
//�h����n
void AttackArea::SpearMove() {
	float l_AddFrame = {};
	if (m_Timer > m_BirthTimer) { return; }
	if (_EffectType == EFFECT_FALL) {
		l_AddFrame = 1 / 20.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			if (!m_Hit) {
				_EffectType = EFFECT_BOUND;
			}
			else {
				_EffectType = EFFECT_HIT;
			}
			m_Frame = {};
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.5f);
			m_Rotation.y = Ease(In, Cubic, m_Frame, m_Rotation.y, 990.0f);
		}
		//������x�̍����ɂȂ�����U������
		if (m_Position.y <= 2.5f) {
			m_Attack = true;
			/// <summary>
			///	����(�h����)
			/// </summary>
			if (m_Sound) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/gusa.wav", 0.02f);
				m_Sound = false;
			}
		}
	}
	else if(_EffectType == EFFECT_BOUND) {
		l_AddFrame = 1 / 30.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,0.0f),
		Ease(In,Cubic,m_Frame,m_Scale.y,0.0f),
		Ease(In,Cubic,m_Frame,m_Scale.z,0.0f), };
		}
	}
	else {
		l_AddFrame = 1 / 60.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
			m_Frame = {};
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 1.5f);
			m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);
			m_Rotation.z = Ease(In, Cubic, m_Frame, m_Rotation.z, -45.0f);

			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,1.5f),
			Ease(In,Cubic,m_Frame,m_Scale.y,0.9f),
			Ease(In,Cubic,m_Frame,m_Scale.z,1.5f), };
		}
	}
}
//???�n
void AttackArea::HatenaMove() {
	const XMFLOAT3 l_AfterScale = { 0.0f,0.0f,0.0f };
	float l_AddFrame = {};
	if (m_Timer > m_BirthTimer) { return; }
	if (_EffectType == EFFECT_FALL) {
		l_AddFrame = 1 / 30.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			if (!m_Hit) {
				_EffectType = EFFECT_BOUND;
			}
			else {
				_EffectType = EFFECT_HIT;
			}
			m_Frame = {};
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 1.0f);
		}

		//������x�̍����ɂȂ�����U������
		if (m_Position.y >= 0.0f) {
			m_Attack = true;
			/// <summary>
			///	����(�h���h���������̂��n�ʂɗ����鉹��](�x�`���b�݂����Ȃ��)
			/// </summary>
			if (m_Sound) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Poison.wav", 0.02f);
				m_Sound = false;
			}
		}
	}
	else if(_EffectType == EFFECT_BOUND) {
		l_AddFrame = 1 / 30.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,l_AfterScale.x),
			Ease(In,Cubic,m_Frame,m_Scale.y,l_AfterScale.y),
			Ease(In,Cubic,m_Frame,m_Scale.z,l_AfterScale.z), };
		}
	}
	else {
		l_AddFrame = 1 / 60.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
			m_Frame = {};
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 3.0f);
			m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);
			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,1.7f),
			Ease(In,Cubic,m_Frame,m_Scale.y,1.7f),
			Ease(In,Cubic,m_Frame,m_Scale.z,1.7f), };
		}
	}

	m_Rotation.y += 10.0f;
}
//�񕜂̓���
void AttackArea::HealMove() {
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const XMFLOAT3 l_AfterScale = { 0.1f,0.1f,0.1f };
	const float l_AddFrame = 1 / 30.0f;
	if (m_Timer > m_BirthTimer) { return; }
	if (_EffectType == EFFECT_FALL) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			_EffectType = EFFECT_BOUND;
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 1.0f);
		}

		//������x�̍����ɂȂ�����U������
		if (m_Position.y >= 0.0f && m_Position.y < 0.7f) {
			m_Attack = true;
			/// <summary>
			///	����(�h���h���������̂��n�ʂɗ����鉹��](�x�`���b�݂����Ȃ��)
			/// </summary>
			if (m_Sound) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/HealAttack.wav", 0.02f);
				m_Sound = false;
			}
		}
	}
	else {
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
			if (m_Hit) {
				player->DrainHeal();
			}
		}
		else {
			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,l_AfterScale.x),
			Ease(In,Cubic,m_Frame,m_Scale.y,l_AfterScale.y),
			Ease(In,Cubic,m_Frame,m_Scale.z,l_AfterScale.z), };
			if (m_Hit) {
				m_Position = { Ease(In,Cubic,m_Frame,m_Position.x,l_PlayerPos.x),
				Ease(In,Cubic,m_Frame,m_Position.y,l_PlayerPos.y),
				Ease(In,Cubic,m_Frame,m_Position.z,l_PlayerPos.z), };
			}
			m_Rotation.z = Ease(In, Cubic, m_Frame, m_Rotation.z, -45.0f);
		}
	}
}
//�����񂹐�����΂��U��
void AttackArea::AeroMove() {
	float l_AddFrame = {};
	if (m_Timer > m_BirthTimer) { return; }
	if (_EffectType == EFFECT_FALL) {
		l_AddFrame = 1 / 20.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			if (!m_Hit) {
				_EffectType = EFFECT_BOUND;
			}
			else {
				_EffectType = EFFECT_HIT;
			}
			m_Frame = {};
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 1.0f);
			if (StateName == "FAR") {
				m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, 45.0f);
				m_Position.x = Ease(In, Cubic, m_Frame, m_Position.x, panels.position.x + 0.7f);
			}
			else {
				m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, -45.0f);
				m_Position.x = Ease(In, Cubic, m_Frame, m_Position.x, panels.position.x - 0.7f);
			}
		}
		//������x�̍����ɂȂ�����U������
		if (m_Position.y <= 1.5f && m_Position.y >= 0.5f) {
			m_Attack = true;
			/// <summary>
			///	����(�h����)
			/// </summary>
			if (m_Sound) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/gusa.wav", 0.02f);
				m_Sound = false;
			}
		}
	}
	else if (_EffectType == EFFECT_BOUND) {
		l_AddFrame = 1 / 30.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,0.0f),
		Ease(In,Cubic,m_Frame,m_Scale.y,0.0f),
		Ease(In,Cubic,m_Frame,m_Scale.z,0.0f), };
		}
	}
	else {
		l_AddFrame = 1 / 60.0f;
		m_Attack = false;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
			m_Frame = {};
		}
		else {
			m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, 0.0f);
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 3.0f);
			m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);

			m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,1.5f),
			Ease(In,Cubic,m_Frame,m_Scale.y,0.9f),
			Ease(In,Cubic,m_Frame,m_Scale.z,1.5f), };
		}
	}
}