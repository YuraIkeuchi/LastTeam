#include "AttackArea.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <GameStateManager.h>
#include <Helper.h>
//読み込み
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
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (AttackArea::* AttackArea::stateTable[])() = {
	&AttackArea::SlashMove,//斬撃
	&AttackArea::StoneMove,//岩落とし
	&AttackArea::PoisonMove,//毒系
};
//初期化
bool AttackArea::Initialize() {
	return true;
}

//ステータス初期化
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
		m_Scale = { 0.2f,0.2f,0.2f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		m_Object->SetBillboard(true);
	}
	else {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.2f,0.2f,0.2f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		m_Object->SetBillboard(true);
	}
}

//更新
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
	//状態移行(charastateに合わせる)
	(this->*stateTable[_EffectState])();
	Obj_SetParam();
}
//描画
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
	ImGui::Text("Scale:%f", m_Scale.x);
	ImGui::End();
}
//パネルの位置に置く
XMFLOAT3 AttackArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
//斬撃系
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
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, {});
		}
	}

	//ある程度傾いたら攻撃判定
	if (m_Rotation.x <= -30.0f) {
		m_Attack = true;
	}
}
//岩落とし系
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
		m_AddPower -= m_Gravity;
		m_Scale = Helper::Float3AddFloat(m_Scale, 0.02f);
		Helper::CheckMax(m_Position.y, 1.0f, m_AddPower);
		if (Helper::CheckMin(m_AddDisolve, 2.5f, 0.1f)) {
			m_Alive = false;
		}
	}

	//ある程度の高さになったら攻撃判定
	if (m_Position.y <= 2.0f) {
		m_Attack = true;
	}
}
//毒系
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

	//ある程度の高さになったら攻撃判定
	if (m_Position.y <= 2.0f) {
		m_Attack = true;
	}
}
