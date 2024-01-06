#include "ThrowEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
#include <StagePanel.h>

//ƒ‚ƒfƒ‹“Ç‚İ‚İ
ThrowEnemy::ThrowEnemy() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::THROW));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//‰Šú‰»
bool ThrowEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_Scale = { 0.6f,0.4f,0.6f };
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ThrowEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/ThrowEnemy.csv", m_Limit, "Interval");

	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ThrowEnemy.csv", "hp")));

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

void (ThrowEnemy::* ThrowEnemy::stateTable[])() = {
	&ThrowEnemy::Inter,//“®‚«‚Ì‡ŠÔ
	&ThrowEnemy::Attack,//“®‚«‚Ì‡ŠÔ
	&ThrowEnemy::Teleport,//uŠÔˆÚ“®
};

//s“®
void ThrowEnemy::Action() {
	if (!m_Induction) {
		(this->*stateTable[_charaState])();
	}
	else {
		InductionMove();
	}
	Obj_SetParam();
	//“–‚½‚è”»’è
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//“–‚½‚è”»’è
	PoisonState();//“Å
	BirthMagic();//–‚–@w
	//“G‚Ì’e
	for (unique_ptr<Boomerang>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Update();
		}
	}

	//áŠQ•¨‚Ìíœ
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i] == nullptr) {
			continue;
		}

		if (!bullets[i]->GetAlive()) {
			bullets.erase(cbegin(bullets) + i);
		}
	}

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();

	if (_charaState == STATE_ATTACK) {
		m_RotPower = Ease(In, Cubic, 0.5f, m_RotPower, 20.0f);
	}
	else {
		m_RotPower = Ease(In, Cubic, 0.5f, m_RotPower, 2.0f);
	}
	m_Rotation.y += m_RotPower;
}

//•`‰æ
void ThrowEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();
	//“G‚Ì’e
	for (unique_ptr<Boomerang>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Draw(dxCommon);
		}
	}
	if (m_Color.w != 0.0f) {
		Obj_Draw();
	}
	BaseBackDraw(dxCommon);
}
//ImGui•`‰æ
void ThrowEnemy::ImGui_Origin() {
	//“G‚Ì’e
	/*for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->ImGuiDraw();
		}
	}*/
	ImGui::Begin("Throw");
	ImGui::Text("Height:%d", m_NowHeight);
	ImGui::Text("ShotDir:%d", m_ShotDir);
	ImGui::End();
}
//ŠJ•ú
void ThrowEnemy::Finalize() {

}
//‘Ò‹@
void ThrowEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, l_TargetTimer);
	if (coolTimer == l_TargetTimer) {
		coolTimer = 100;
		_charaState = STATE_ATTACK;
	}
}
//UŒ‚
void ThrowEnemy::Attack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];

	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		BirthBullet();
		m_CheckPanel = true;
		m_AttackCount = {};
		coolTimer = {};
		StagePanel::GetInstance()->EnemyHitReset();
		_charaState = STATE_SPECIAL;
	}
}

//ƒ[ƒv
void ThrowEnemy::Teleport() {
	const float l_AddFrame = 1 / 30.0f;
	const int l_RandTimer = Helper::GetRanNum(0, 30);
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];

	if (Helper::CheckMin(coolTimer, l_TargetTimer + l_RandTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
	}
}
//’e‚Ì¶¬
void ThrowEnemy::BirthBullet() {
	//’e‚Ì”­¶
	unique_ptr<Boomerang> newbullet = make_unique<Boomerang>();
	newbullet->Initialize();
	newbullet->SetPlayer(player);
	newbullet->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
	bullets.emplace_back(std::move(newbullet));
}
//–‚–@w¶¬
void ThrowEnemy::BirthMagic() {
	if (!magic.Alive) { return; }
	static float addFrame = 1.f / 15.f;
	const int l_TargetTimer = 20;
	if (magic.State == MAGIC_BIRTH) {			//–‚–@w‚ğL‚°‚é
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
	else {			//–‚–@w‚ğk‚ß‚é
		if (Helper::FrameCheck(magic.Frame, addFrame)) {
			magic.Frame = {};
			magic.AfterScale = 0.2f;
			magic.Alive = false;
			magic.State = MAGIC_BIRTH;
		}
		magic.Scale = Ease(In, Cubic, magic.Frame, magic.Scale, magic.AfterScale);
	}
}
void ThrowEnemy::WarpEnemy() {
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel(m_LastEnemy);
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//ƒLƒƒƒ‰‚ª¬‚³‚­‚È‚é
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
	else {			//ƒLƒƒƒ‰‚ª‘å‚«‚­‚È‚Á‚Ä‚¢‚é
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
//ƒNƒŠƒAƒV[ƒ“‚ÌXV
void ThrowEnemy::ClearAction() {
	m_AddDisolve = {};
	Obj_SetParam();
}