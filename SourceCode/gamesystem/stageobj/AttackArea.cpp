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
		_EffectState = Poison;
	}
	else {
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

	if (_EffectState != Poison) {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::DOGO);
	}
	else {
		m_Model = ModelManager::GetInstance()->GetModel(ModelManager::THORN);
	}
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
	if (_EffectState != Poison) {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.3f,0.3f,0.3f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		m_Object->SetBillboard(true);
	}
	else {
		m_Scale = { 0.4f,0.25f,0.4f };
		m_Position = { panels.position.x,-0.5f,panels.position.z };
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
	ImGui::Text("Timer:%d", m_Timer);
	ImGui::End();
}
//パネルの位置に置く
XMFLOAT3 AttackArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
//斬撃系
void AttackArea::SlashMove() {
	if (m_Timer > m_BirthTimer) { return; }
	m_AddPower -= m_Gravity;
	if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
		//m_Alive = false;
	}
}
//岩落とし系
void AttackArea::StoneMove() {
	if (m_Timer > m_BirthTimer) { return; }
	m_AddPower -= m_Gravity;
	if (Helper::CheckMax(m_Position.y, 1.0f, m_AddPower)) {
		//m_Alive = false;
	}
}
//毒系
void AttackArea::PoisonMove() {
	static float addFrame = 1.f / 10.f;
	if (m_Timer > m_BirthTimer) { return; }
	if (_ThornState == THORN_UP) {
		if (Helper::FrameCheck(m_Frame, addFrame)) {
			_ThornState = THORN_END;
			m_Frame = {};
		}
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.4f);
	}
	else {
		if (Helper::FrameCheck(m_Frame, addFrame)) {
			//m_Alive = false;
		}
		m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);
	}
}
