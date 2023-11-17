#include "PoisonEnemy.h"
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
PoisonEnemy::PoisonEnemy() {
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

	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>();
		_drawnumber[i]->Initialize();
	}

	shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//‰Šú‰»
bool PoisonEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "hp")));
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
	return true;
}

void (PoisonEnemy::* PoisonEnemy::stateTable[])() = {
	&PoisonEnemy::Inter,//“®‚«‚Ì‡ŠÔ
	&PoisonEnemy::Attack,//“®‚«‚Ì‡ŠÔ
	&PoisonEnemy::Teleport,//uŠÔˆÚ“®
};

//s“®
void PoisonEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//“–‚½‚è”»’è
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//“–‚½‚è”»’è
	PoisonState();//“Å
	BirthMagic();//–‚–@w

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();

	//áŠQ•¨‚Ìíœ
	for (int i = 0; i < poisonarea.size(); i++) {
		if (poisonarea[i] == nullptr) {
			continue;
		}

		poisonarea[i]->Update();
		if (!poisonarea[i]->GetAlive()) {
			poisonarea.erase(cbegin(poisonarea) + i);
		}
	}
}

//•`‰æ
void PoisonEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	magic.tex->Draw();
	IKETexture::PostDraw();

	//áŠQ•¨‚Ìíœ
	for (int i = 0; i < poisonarea.size(); i++) {
		if (poisonarea[i] == nullptr) {
			continue;
		}

		poisonarea[i]->Draw(dxCommon);
	}
	Obj_Draw();
}
//ImGui•`‰æ
void PoisonEnemy::ImGui_Origin() {
	//áŠQ•¨‚Ìíœ
	for (int i = 0; i < poisonarea.size(); i++) {
		if (poisonarea[i] == nullptr) {
			continue;
		}

		poisonarea[i]->ImGuiDraw();
	}
}
//ŠJ•ú
void PoisonEnemy::Finalize() {

}
//‘Ò‹@
void PoisonEnemy::Inter() {
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	if (coolTimer == kIntervalMax) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		BirthPoison();
	}
}
//UŒ‚
void PoisonEnemy::Attack() {
	const int l_TargetTimer = 200;

	if (_PoisonType == Poison_SET) {
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
			coolTimer = {};
			_PoisonType = Poison_THROW;
		}
	}
	else if (_PoisonType == Poison_THROW) {
		m_AttackCount++;
		BirthPoison();
		if (m_AttackCount != 2) {
			_PoisonType = Poison_SET;
		}
		else {
			_PoisonType = Poison_END;
		}
	}
	else {
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
		coolTimer = {};
		_PoisonType = Poison_SET;
		StagePanel::GetInstance()->EnemyHitReset();
	}
}

//ƒ[ƒv
void PoisonEnemy::Teleport() {
	const int l_TargetTimer = 200;

	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
	}
}
//“Å‚Ì¶¬
void PoisonEnemy::BirthPoison() {
	int l_RandWidth;
	int l_RandHeight;
	StagePanel::GetInstance()->PoisonSetPanel(l_RandWidth,l_RandHeight);
	std::unique_ptr<PoisonArea> newarea = std::make_unique<PoisonArea>();
	newarea->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
	newarea->InitState(l_RandWidth, l_RandHeight);
	newarea->SetPlayer(player);
	poisonarea.push_back(std::move(newarea));
}
//–‚–@w¶¬
void PoisonEnemy::BirthMagic() {
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
void PoisonEnemy::WarpEnemy() {
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel();
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//ƒLƒƒƒ‰‚ª¬‚³‚­‚È‚é
		if (Helper::FrameCheck(enemywarp.Frame, addFrame)) {
			enemywarp.Frame = {};
			enemywarp.AfterScale = 0.5f;
			enemywarp.State = WARP_END;
			coolTimer = {};
			m_Position = l_RandPos;
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
