#include "CreateBombEnemy.h"
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
CreateBombEnemy::CreateBombEnemy() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::BOMBER));
	
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
bool CreateBombEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/CreateBombEnemy.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/CreateBombEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/CreateBombEnemy.csv", m_Limit, "Interval");
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
	m_RandTimer = Helper::GetRanNum(0, 40);
	return true;
}

void (CreateBombEnemy::* CreateBombEnemy::stateTable[])() = {
	&CreateBombEnemy::Inter,//動きの合間
	&CreateBombEnemy::Attack,//動きの合間
	&CreateBombEnemy::Teleport,//瞬間移動
};

//行動
void CreateBombEnemy::Action() {
	if (!m_Induction) {
		(this->*stateTable[_charaState])();
	}
	else {
		InductionMove();
	}
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

	//障害物の削除
	for (int i = 0; i < bomb.size(); i++) {
		if (bomb[i] == nullptr) {
			continue;
		}
		bomb[i]->Update();
		if (!bomb[i]->GetAlive()) {
			bomb.erase(cbegin(bomb) + i);
		}
	}
}

//描画
void CreateBombEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();

	//障害物の削除
	for (int i = 0; i < bomb.size(); i++) {
		if (bomb[i] == nullptr) {
			continue;
		}

		bomb[i]->Draw(dxCommon);
	}
	if (m_Color.w != 0.0f) {
		Obj_Draw();
	}
	BaseBackDraw(dxCommon);
}
//ImGui描画
void CreateBombEnemy::ImGui_Origin() {
	//障害物の削除
	for (int i = 0; i < bomb.size(); i++) {
		if (bomb[i] == nullptr) {
			continue;
		}

		bomb[i]->ImGuiDraw();
	}
}
//開放
void CreateBombEnemy::Finalize() {

}
//待機
void CreateBombEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];

	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
	}
}
//攻撃
void CreateBombEnemy::Attack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];
	
	if (_BombType == Bomb_SET) {
		m_CanCounter = true;
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
			coolTimer = {};
			_BombType = Bomb_THROW;
		}
	}
	else if (_BombType == Bomb_THROW) {
		if (!isBombThrow) {
			BirthBomb();
			isBombThrow = true;
			_BombType = Bomb_SET;
		}
		else {
			isBombThrow = false;
			_BombType = Bomb_END;
		}
	}
	else {
		m_CanCounter = false;
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
		coolTimer = {};
		_BombType = Bomb_SET;
		StagePanel::GetInstance()->EnemyHitReset();
	}
}
//ワープ
void CreateBombEnemy::Teleport() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];
	
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
	}
}
//爆弾の生成
void CreateBombEnemy::BirthBomb() {
	int l_RandWidth;
	int l_RandHeight;
	StagePanel::GetInstance()->PoisonSetPanel(l_RandWidth,l_RandHeight);
	//ボム生成
	std::unique_ptr<Bomb> newBomb = std::make_unique<Bomb>();
	newBomb->Initialize();
	newBomb->SetPosition({ m_Position.x,m_Position.y + 1.2f,m_Position.z });
	newBomb->SetTargetPos(StagePanel::GetInstance()->EnemySetPanel(true));
	newBomb->SetPlayer(player);
	bomb.push_back(std::move(newBomb));
}
//魔法陣生成
void CreateBombEnemy::BirthMagic() {
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
void CreateBombEnemy::WarpEnemy() {
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
			m_RandTimer = Helper::GetRanNum(0, 40);
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}

	m_Scale = { enemywarp.Scale,enemywarp.Scale, enemywarp.Scale };
}

//クリアシーンの更新
void CreateBombEnemy::ClearAction() {
	const int l_TargetTimer = 100;
	const float l_AddFrame = 1 / 200.0f;
	if (m_ClearTimer == 0) {
		m_Position.y = 10.0f;
	}

	if (Helper::CheckMin(m_ClearTimer, l_TargetTimer, 1)) {
		if (Helper::FrameCheck(m_ClearFrame, l_AddFrame)) {
			m_ClearFrame = 1.0f;
		}
		else {
			m_Position.y = Ease(In, Cubic, m_ClearFrame, m_Position.y, 0.1f);
		}
	}
	m_AddDisolve = {};
	Obj_SetParam();
}
//ゲームオーバーシーンの更新
void CreateBombEnemy::GameOverAction() {
	if (_GameOverState == OVER_STOP) {
		m_Position = { 2.5f,0.0f,2.5f };
		m_Rotation = { 0.0f,180.0f,0.0f };
		m_AddDisolve = 0.0f;
		if (player->GetSelectType() == 1) {
			_GameOverState = OVER_YES;
			m_AddPower = 0.3f;
		}
		else if (player->GetSelectType() == 2) {
			_GameOverState = OVER_NO;
		}
	}
	else if (_GameOverState == OVER_YES) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
			m_Position.y = 0.1f;
			m_AddPower = {};
			if (Helper::CheckMin(m_OverTimer, 40, 1)) {
				m_OverTimer = {};
				m_AddPower = 0.3f;
			}
		}
	}
	else {
		float l_AddRotZ = {};
		float l_AddFrame2 = {};

		l_AddRotZ = float(Helper::GetRanNum(30, 100)) / 100;
		l_AddFrame2 = float(Helper::GetRanNum(1, 10)) / 500;
		float RotPower = 8.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//最初はイージングで回す
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//最後は倒れる
				m_Rotation.z = 90.0f;
			}
		}
		else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 18.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}