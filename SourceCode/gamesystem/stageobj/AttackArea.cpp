#include "AttackArea.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <GameStateManager.h>
#include <Audio.h>
#include <Helper.h>
//“Ç‚İ‚İ
AttackArea::AttackArea(string& userName, string& stateName) {
	this->m_Name = userName;
	this->StateName = stateName;
	if (StateName == "POISON" || StateName == "VENOM") {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DORO);
		_EffectState = Poison;
	}
	else if(StateName == "SLASH") {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::ZASHU);
		_EffectState = Slash;
	}
	else if (StateName == "SPEAR") {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::GUSA);
		_EffectState = Spear;
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
//ó‘Ô‘JˆÚ
/*CharaState‚ÌState•À‚Ñ‡‚É‡‚í‚¹‚é*/
void (AttackArea::* AttackArea::stateTable[])() = {
	&AttackArea::SlashMove,//aŒ‚
	&AttackArea::StoneMove,//Šâ—‚Æ‚µ
	&AttackArea::PoisonMove,//“ÅŒn
	&AttackArea::SpearMove,//‚â‚èŒn
};
//‰Šú‰»
bool AttackArea::Initialize() {
	return true;
}

//ƒXƒe[ƒ^ƒX‰Šú‰»
void AttackArea::InitState(const int width, const int height) {
	panels.position = SetPanelPos(width, height);
	panels.color = { 1.f,1.f,1.f,1.f };
	m_NowWidth = width, m_NowHeight = height;
	panels.position.y = 0.03f;
	m_Alive = true;
	m_Hit = false;
	m_Timer = false;
	m_Rotation = { 0.0f,0.0f,0.0f };
	if (StateName == "POISON" || StateName == "VENOM") {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.2f,0.2f,0.2f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		m_Object->SetBillboard(true);
	}
	else if (StateName == "SLASH") {
		m_Rotation = { 45.0f,270.0f,0.0f };
		m_Scale = { 0.3f,0.3f,0.3f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		m_Object->SetBillboard(true);
	}
	else if (StateName == "SPEAR") {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.4f,0.4f,0.4f };
		m_Position = { panels.position.x,5.0f,panels.position.z };
		m_Object->SetBillboard(true);
	}
	else {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.2f,0.2f,0.2f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		m_Object->SetBillboard(true);
	}
}

//XV
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
	//ó‘ÔˆÚs(charastate‚É‡‚í‚¹‚é)
	(this->*stateTable[_EffectState])();
	Obj_SetParam();
}
//•`‰æ
void AttackArea::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	panels.tex->Draw();
	IKETexture::PostDraw();
	if (m_Timer <= m_BirthTimer) {
		Obj_Draw();
	}
}
//ImGui
void AttackArea::ImGuiDraw() {
	ImGui::Begin("Attack");
	ImGui::Text("Sound:%d", m_Timer);
	ImGui::End();
}
//ƒpƒlƒ‹‚ÌˆÊ’u‚É’u‚­
XMFLOAT3 AttackArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
//aŒ‚Œn
void AttackArea::SlashMove() {
	if (m_Timer > m_BirthTimer) { return; }
	const float l_AddFrame = 1 / 20.0f;
	if (_StoneType == STONE_FALL) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			_StoneType = STONE_BOUND;
			m_Frame = {};
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.5f);
			m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, -90.0f);
		}
	}
	else {
		m_Hit = true;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, {});
		}
	}

	//‚ ‚é’ö“xŒX‚¢‚½‚çUŒ‚”»’è
	if (m_Rotation.x <= -20.0f) {
		m_Attack = true;
		/// <summary>
		///	‰¹“ü‚ê(aŒ‚‰¹Šó–])
		/// </summary>
		if (m_Sound) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/slash.wav", 0.02f);
			m_Sound = false;
		}
	}
}
//Šâ—‚Æ‚µŒn
void AttackArea::StoneMove() {
	if (m_Timer > m_BirthTimer) { return; }
	if (_StoneType == STONE_FALL) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 1.0f, m_AddPower)) {
			m_AddPower = 0.2f;
			_StoneType = STONE_BOUND;
		}
	}
	else {
		m_Hit = true;
		m_AddPower -= m_Gravity;
		m_Scale = Helper::Float3AddFloat(m_Scale, 0.02f);
		Helper::CheckMax(m_Position.y, 1.0f, m_AddPower);
		if (Helper::CheckMin(m_AddDisolve, 2.5f, 0.1f)) {
			m_Alive = false;
		}
	}

	/// <summary>
	///	‰¹“ü‚ê
	/// </summary>
	if (m_Sound && m_Rotation.x <= -10.0f) {
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Damage.wav", 0.02f);
	}

	//‚ ‚é’ö“x‚Ì‚‚³‚É‚È‚Á‚½‚çUŒ‚”»’è
	if (m_Position.y <= 2.5f) {
		m_Attack = true;
		/// <summary>
		///	‰¹“ü(Šâ‚ª—‚¿‚é–”‚ÍŒ‡‚¯‚é‰¹Šó–])
		/// </summary>
		if (m_Sound) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/heavyRockCollapse.wav", 0.02f);
			m_Sound = false;
		}
	}
}
//“ÅŒn
void AttackArea::PoisonMove() {
	const XMFLOAT3 l_AfterScale = { 1.35f,0.05f,0.35f };
	const float l_AddFrame = 1 / 30.0f;
	if (m_Timer > m_BirthTimer) { return; }
	if (_StoneType == STONE_FALL) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 1.0f, m_AddPower)) {
			m_AddPower = 0.2f;
			_StoneType = STONE_BOUND;
		}
	}
	else {
		m_Hit = true;
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

	//‚ ‚é’ö“x‚Ì‚‚³‚É‚È‚Á‚½‚çUŒ‚”»’è
	if (m_Position.y <= 2.5f) {
		m_Attack = true;
		/// <summary>
		///	‰¹“ü(ƒhƒƒhƒ‚µ‚½‚à‚Ì‚ª’n–Ê‚É—‚¿‚é‰¹Šó–](ƒxƒ`ƒƒƒb‚İ‚½‚¢‚È‚â‚Â)
		/// </summary>
		if (m_Sound) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/Poison.wav", 0.02f);
			m_Sound = false;
		}
	}
}
void AttackArea::SpearMove() {
	float l_AddFrame = {};
	if (m_Timer > m_BirthTimer) { return; }
	if (_StoneType == STONE_FALL) {
		l_AddFrame = 1 / 20.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			_StoneType = STONE_BOUND;
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.5f);
			m_Rotation.y = Ease(In, Cubic, m_Frame, m_Rotation.y, 990.0f);
		}
	}
	else {
		l_AddFrame = 1 / 30.0f;
		m_Hit = true;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, {});
		}
	}

	//‚ ‚é’ö“x‚Ì‚‚³‚É‚È‚Á‚½‚çUŒ‚”»’è
	if (m_Position.y <= 2.5f) {
		m_Attack = true;
		/// <summary>
		///	‰¹“ü(Šâ‚ª—‚¿‚é–”‚ÍŒ‡‚¯‚é‰¹Šó–])
		/// </summary>
		if (m_Sound) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/heavyRockCollapse.wav", 0.02f);
			m_Sound = false;
		}
	}
}
