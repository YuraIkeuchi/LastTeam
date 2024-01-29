#include "AttackArea.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <GameStateManager.h>
#include <Audio.h>
#include <Helper.h>
//読み込み
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
	&AttackArea::SpearMove,//やり系
	&AttackArea::HatenaMove,//はてな
	&AttackArea::HealMove,//回復の動き
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
	if (_EffectState == Poison) {
		m_Rotation.y = 270.0f;
		m_Scale = { 0.2f,0.2f,0.2f };
		m_Position = { panels.position.x,3.0f,panels.position.z };
		m_Object->SetBillboard(true);
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
	ImGui::Text("Sound:%d", m_Timer);
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
		m_Hit = true;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, {});
		}
	}

	//ある程度傾いたら攻撃判定
	if (m_Rotation.x <= -20.0f) {
		m_Attack = true;
		/// <summary>
		///	音入れ(斬撃音希望)
		/// </summary>
		if (m_Sound) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/slash.wav", 0.02f);
			m_Sound = false;
		}
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
		m_Hit = true;
		m_AddPower -= m_Gravity;
		m_Scale = Helper::Float3AddFloat(m_Scale, 0.02f);
		Helper::CheckMax(m_Position.y, 1.0f, m_AddPower);
		if (Helper::CheckMin(m_AddDisolve, 2.5f, 0.1f)) {
			m_Alive = false;
		}
	}

	/// <summary>
	///	音入れ
	/// </summary>
	if (m_Sound && m_Rotation.x <= -10.0f) {
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Damage.wav", 0.02f);
	}

	//ある程度の高さになったら攻撃判定
	if (m_Position.y <= 2.5f) {
		m_Attack = true;
		/// <summary>
		///	音入(岩が落ちる又は欠ける音希望)
		/// </summary>
		if (m_Sound) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/heavyRockCollapse.wav", 0.02f);
			m_Sound = false;
		}
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

	//ある程度の高さになったら攻撃判定
	if (m_Position.y <= 2.5f) {
		m_Attack = true;
		/// <summary>
		///	音入(ドロドロしたものが地面に落ちる音希望(ベチャッみたいなやつ)
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

	//ある程度の高さになったら攻撃判定
	if (m_Position.y <= 2.5f) {
		m_Attack = true;
		/// <summary>
		///	音入(岩が落ちる又は欠ける音希望)
		/// </summary>
		if (m_Sound) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/heavyRockCollapse.wav", 0.02f);
			m_Sound = false;
		}
	}
}
void AttackArea::HatenaMove() {
	const XMFLOAT3 l_AfterScale = { 0.0f,0.0f,0.0f };
	const float l_AddFrame = 1 / 30.0f;
	if (m_Timer > m_BirthTimer) { return; }
	if (_StoneType == STONE_FALL) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			_StoneType = STONE_BOUND;
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 1.0f);
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
		}
	}

	m_Rotation.y += 10.0f;

	//ある程度の高さになったら攻撃判定
	if (m_Position.y >= 0.0f) {
		m_Attack = true;
		/// <summary>
		///	音入(ドロドロしたものが地面に落ちる音希望(ベチャッみたいなやつ)
		/// </summary>
		if (m_Sound) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/Poison.wav", 0.02f);
			m_Sound = false;
		}
	}
}
//回復の動き
void AttackArea::HealMove() {
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const XMFLOAT3 l_AfterScale = { 0.0f,0.0f,0.0f };
	const float l_AddFrame = 1 / 30.0f;
	if (m_Timer > m_BirthTimer) { return; }
	if (_StoneType == STONE_FALL) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			_StoneType = STONE_BOUND;
		}
		else {
			m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 1.0f);
		}
	}
	else {
		
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
		}
	}

	//ある程度の高さになったら攻撃判定
	if (m_Position.y >= 0.0f && m_Position.y < 0.7f) {
		m_Attack = true;
		/// <summary>
		///	音入(ドロドロしたものが地面に落ちる音希望(ベチャッみたいなやつ)
		/// </summary>
		if (m_Sound) {
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/Poison.wav", 0.02f);
			m_Sound = false;
		}
	}
}