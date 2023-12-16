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
//モデル読み込み
PoisonEnemy::PoisonEnemy() {
	//共通の初期化処理
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::MISSILE));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	//shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	//shadow_tex->TextureCreate();
	//shadow_tex->Initialize();
	//shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//初期化
bool PoisonEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	m_BaseScale = 0.7f;
	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "hp")));
	m_MaxHP = m_HP;
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/PoisonEnemy.csv", m_Limit, "Interval");
	m_BulletNum = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "POISON_NUM")));

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

void (PoisonEnemy::* PoisonEnemy::stateTable[])() = {
	&PoisonEnemy::Inter,//動きの合間
	&PoisonEnemy::Attack,//動きの合間
	&PoisonEnemy::Teleport,//瞬間移動
};

//行動
void PoisonEnemy::Action() {
	(this->*stateTable[_charaState])();
	Obj_SetParam();
	//当たり判定
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定
	PoisonState();//毒
	BirthMagic();//魔法陣

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	//shadow_tex->SetPosition(m_ShadowPos);
	//shadow_tex->SetScale(m_ShadowScale);
	//shadow_tex->Update();

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();

	//どくの削除
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

//描画
void PoisonEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	if (m_SuperPoison) {poison_tex->Draw();}
	if (m_HealDamage) { healdamage_tex->Draw(); }
	IKETexture::PostDraw();

	//障害物の削除
	for (int i = 0; i < poisonarea.size(); i++) {
		if (poisonarea[i] == nullptr) {
			continue;
		}

		poisonarea[i]->Draw(dxCommon);
	}
	if (m_Color.w != 0.0f)
		Obj_Draw();
}
//ImGui描画
void PoisonEnemy::ImGui_Origin() {
	////どくの削除
	//for (int i = 0; i < poisonarea.size(); i++) {
	//	if (poisonarea[i] == nullptr) {
	//		continue;
	//	}

	//	poisonarea[i]->ImGuiDraw();
	//}
	ImGui::Begin("Poison");
	ImGui::Text("Frame:%f", m_ScaleFrame);
	ImGui::Text("Scale:%f", m_BaseScale);
	ImGui::End();
}
//開放
void PoisonEnemy::Finalize() {

}
//待機
void PoisonEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, l_TargetTimer);
	if (coolTimer == l_TargetTimer) {
		coolTimer = 100;
		_charaState = STATE_ATTACK;
	}
}
//攻撃
void PoisonEnemy::Attack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];
	float l_AfterScale = {};
	float l_AddFrame = {};
	if (_PoisonType == Poison_SET) {
		l_AfterScale = 0.3f;
		l_AddFrame = 1 / 30.0f;
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
			if (Helper::FrameCheck(m_ScaleFrame,l_AddFrame)) {
				coolTimer = {};
				_PoisonType = Poison_THROW;
				m_ScaleFrame = {};
			}

			m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
		}
	}
	else if (_PoisonType == Poison_THROW) {
		l_AfterScale = 0.5f;
		l_AddFrame = 1 / 20.0f;
		if (Helper::FrameCheck(m_ScaleFrame, l_AddFrame)) {
			m_AttackCount++;
			BirthPoison();
			if (m_AttackCount != m_BulletNum) {
				_PoisonType = Poison_SET;
			}
			else {
				_PoisonType = Poison_END;
			}
			m_ScaleFrame = {};
		}
		m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
	}
	else {
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
		coolTimer = {};
		_PoisonType = Poison_SET;
		StagePanel::GetInstance()->EnemyHitReset();
	}

	m_Scale = { m_Scale.x,m_BaseScale,m_Scale.z };
}

//ワープ
void PoisonEnemy::Teleport() {
	const int l_RandTimer = Helper::GetRanNum(0, 30);
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];

	if (Helper::CheckMin(coolTimer, l_RandTimer + l_RandTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
	}
}
//毒の生成
void PoisonEnemy::BirthPoison() {
	int l_RandWidth;
	int l_RandHeight;
	StagePanel::GetInstance()->PoisonSetPanel(l_RandWidth,l_RandHeight);
	std::unique_ptr<PoisonArea> newarea = std::make_unique<PoisonArea>();
	newarea->SetPosition({ m_Position.x,m_Position.y + 0.5f,m_Position.z });
	newarea->InitState(l_RandWidth, l_RandHeight);
	newarea->SetPlayer(player);
	poisonarea.push_back(std::move(newarea));
}
//魔法陣生成
void PoisonEnemy::BirthMagic() {
	if (!magic.Alive) { return; }
	static float addFrame = 1.f / 15.f;
	const int l_TargetTimer = 20;
	if (magic.State == MAGIC_BIRTH) {			//魔法陣を広げる
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
	else {			//魔法陣を縮める
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
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel(m_LastEnemy);
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//キャラが小さくなる
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
	else {			//キャラが大きくなっている
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
