#include "SupportEnemy.h"
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
SupportEnemy::SupportEnemy() {
	m_BombCounter = true;
	m_EnemyTag = "SUPPORT";
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::SUPPORT_ENEMY2));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	/*shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });*/
	//予測
	predictarea.reset(new PredictArea("ENEMY"));
	predictarea->Initialize();
	lastbomb = make_unique<LastBomb>();
	lastbomb->Initialize();
	lastbomb->SetPlayer(player);
}
//初期化
bool SupportEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,90.0f,0.0f };
	//m_Color = { 1.f,0.f,1.f,1.0f };
	m_Scale = { 0.4f,0.4f,0.4f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/SupportEnemy.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/SupportEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/SupportEnemy.csv", m_Limit, "Interval");

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
	enemywarp.Scale = 0.4f;
	m_AddDisolve = 2.0f;
	m_RandTimer = Helper::GetRanNum(0,20);
	_BombAttackType = SET_BOMB;
	return true;
}
//状態遷移
void (SupportEnemy::* SupportEnemy::stateTable[])() = {
	&SupportEnemy::Inter,//動きの合間
	&SupportEnemy::Attack,//動きの合間
	&SupportEnemy::Teleport,//瞬間移動
};

//攻撃遷移
void (SupportEnemy::* SupportEnemy::attackTable[])() = {
	&SupportEnemy::CounterAttack,//カウンター
	&SupportEnemy::BombAttack,//ボム攻撃
};

//行動
void SupportEnemy::Action() {
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
	AttackMove();//攻撃時の動き


	//カウンターのボム
	if (m_BirthBomb) {
		if (_AttackState == ATTACK_COUNTER && _charaState == STATE_ATTACK) {
			BirthCounter();
		}
		m_BirthBomb = false;
	}

	//攻撃時ジャンプする
	if (m_Jump) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
			m_AddPower = {};
			m_Jump = false;
			m_Position.y = 0.1f;
		}
	}

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();

	//カウンターボムの削除
	for (int i = 0; i < counterbomb.size(); i++) {
		if (counterbomb[i] == nullptr) {
			continue;
		}

		counterbomb[i]->Update();
		if (!counterbomb[i]->GetAlive()) {
			counterbomb.erase(cbegin(counterbomb) + i);
		}
	}

	//吸収エフェクト
	for (int i = 0; i < abseffect.size(); i++) {
		if (abseffect[i] == nullptr) {
			continue;
		}

		abseffect[i]->Update();
		if (!abseffect[i]->GetAlive()) {
			abseffect.erase(cbegin(abseffect) + i);
		}
	}

	//吸収エフェクト
	for (int i = 0; i < abseffect.size(); i++) {
		if (abseffect[i] == nullptr) {
			continue;
		}

		if (!abseffect[i]->GetAlive()) {
			abseffect.erase(cbegin(abseffect) + i);
		}
	}
	lastbomb->Update();
}

//描画
void SupportEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();
	//吸収エフェクト
	for (int i = 0; i < abseffect.size(); i++) {
		if (abseffect[i] == nullptr) {
			continue;
		}

		abseffect[i]->Draw(dxCommon);
	}
	//カウンターボムの描画
	for (int i = 0; i < counterbomb.size(); i++) {
		if (counterbomb[i] == nullptr) {
			continue;
		}

		counterbomb[i]->Draw(dxCommon);
	}
	predictarea->Draw(dxCommon);
	if (m_Color.w != 0.0f)
		Obj_Draw();
	if(lastbomb->GetAlive())
	   lastbomb->Draw(dxCommon);
	BaseBackDraw(dxCommon);
}
//ImGui描画
void SupportEnemy::ImGui_Origin() {
}
//開放
void SupportEnemy::Finalize() {

}
//待機
void SupportEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		m_RandTimer = Helper::GetRanNum(0, 20);
		int l_RandState = Helper::GetRanNum(0,1);
		_AttackState = (AttackState)l_RandState;
	}
}
//攻撃
void SupportEnemy::Attack() {
	(this->*attackTable[_AttackState])();
}
//ワープ
void SupportEnemy::Teleport() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];

	if (Helper::CheckMin(coolTimer, l_TargetTimer + m_RandTimer, 1)) {
		magic.Alive = true;
	}

	if (m_Warp) {
		WarpEnemy();
	}
}
//カウンター
void SupportEnemy::CounterAttack() {
	int l_CounterTimer = 200;

	if (Helper::CheckMin(coolTimer, l_CounterTimer, 1)) {
		_charaState = STATE_SPECIAL;
		m_RandTimer = Helper::GetRanNum(0, 20);
		coolTimer = 0;
	}

	if (coolTimer % 5 == 0) {
		BirthCharge();
	}
}
//爆弾攻撃
void SupportEnemy::BombAttack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];

	const int l_JumpTimer = 80;
	if (coolTimer == 10) {
		m_CanCounter = true;
		//弾の発生
		lastbomb->InitState({ m_Position.x,m_Position.y + 1.5f,m_Position.z }, m_NowWidth - 1, m_NowHeight);
		BirthPredict(3 - m_AttackCount, m_NowHeight, "Bomb");
	}
	else if (coolTimer == l_JumpTimer) {
		_BombAttackType = JUMP_MOVE;
	}
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
		m_AttackCount++;
		coolTimer = 0;
		if (m_AttackCount == 4) {
			_charaState = STATE_SPECIAL;
			m_RandTimer = Helper::GetRanNum(0, 20);
			m_AttackCount = {};
		}
	}

	if (_BombAttackType == JUMP_MOVE) {
		if (Helper::CheckMin(m_Position.y, 10.0f, 0.3f)) {
			m_Position.x = lastbomb->GetPosition().x;
			_BombAttackType = FALL_MOVE;
		}
	}
	else if (_BombAttackType == FALL_MOVE) {
		if (Helper::CheckMax(m_Position.y, 0.1f, -0.3f)) {
			m_CanCounter = false;
			predictarea->ResetPredict();
			_BombAttackType = SET_BOMB;
			lastbomb->BirthExplosion();
		}
	}
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
	predictarea->SetTargetTimer(l_TargetTimer);
}
//攻撃エリア
void SupportEnemy::BirthArea(const int Width, const int Height, const string& name) {

}
//予測エリア
void SupportEnemy::BirthPredict(const int Width, const int Height, const string& name) {
	for (int i = 0; i < (PANEL_WIDTH / 2); i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (Width == i || Height == j) {
				predictarea->SetPredict(i, j, true);
			}
		}
	}
	predictarea->SetFlashStart(true);
}

//魔法陣生成
void SupportEnemy::BirthMagic() {
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
void SupportEnemy::WarpEnemy() {
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel(m_LastEnemy,m_EnemyTag);
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//キャラが小さくなる
		if (Helper::FrameCheck(enemywarp.Frame, addFrame)) {
			enemywarp.Frame = {};
			enemywarp.AfterScale = 0.4f;
			enemywarp.State = WARP_END;
			coolTimer = {};
			m_Position = l_RandPos;
			m_RotFrame = {};
			m_Rotation.y = 90.0f;
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
			m_RandTimer = Helper::GetRanNum(0, 20);
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}

	m_Scale = { enemywarp.Scale,enemywarp.Scale, enemywarp.Scale };
}
//攻撃時の動き
void SupportEnemy::AttackMove() {
	if (!m_Rot) { return; }
	const float l_AddFrame = 1 / 20.0f;
	if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
		m_Rotation.y = -90.0f;
		m_Rot = false;
		m_AttackFrame = {};
	}

	m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 630.0f);
}
//クリアシーンの更新
void SupportEnemy::ClearAction() {
	const int l_TargetTimer = 160;
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
void SupportEnemy::GameOverAction() {
	if (_GameOverState == OVER_STOP) {
		m_Position = { -6.5f,0.0f,5.5f };
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
			if (Helper::CheckMin(m_OverTimer, 35, 1)) {
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
		float RotPower = 10.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//最初はイージングで回す
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//最後は倒れる
				m_Rotation.z = 90.0f;
			}
		}
		else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 20.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}

	Obj_SetParam();
}

void SupportEnemy::BirthCounter() {
	int l_PlayerWidth = {};
	int l_PlayerHeight = {};
	l_PlayerWidth = player->GetNowWidth();
	l_PlayerHeight = player->GetNowHeight();
	std::unique_ptr<CounterBomb> newbomb = std::make_unique<CounterBomb>();
	newbomb->SetPosition({ m_Position.x,m_Position.y + 0.5f,m_Position.z });
	newbomb->InitState(l_PlayerWidth, l_PlayerHeight);
	newbomb->SetPlayer(player);
	counterbomb.push_back(std::move(newbomb));
}
//チャージのときのエフェクト
void SupportEnemy::BirthCharge() {
	//エフェクト発生
	std::unique_ptr<AbsorptionEffect> neweffect = std::make_unique<AbsorptionEffect>();
	neweffect->Initialize();
	neweffect->SetBasePos(m_Position);
	neweffect->SetColor({ 0.7f,0.2f,0.2f,1.0f });
	neweffect->SetAddFrame(0.01f);
	abseffect.push_back(std::move(neweffect));
}
void SupportEnemy::DeathSpecial() {
}