#include "FrontEnemy.h"
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
FrontEnemy::FrontEnemy() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::FRONT));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	//予測
	predictarea.reset(new PredictArea("ENEMY"));
	predictarea->Initialize();
}
//初期化
bool FrontEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	//m_Color = { 0.0f,1.0f,0.5f,1.0f };
	m_Scale = { 0.4f,0.4f,0.4f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/FrontEnemy.csv", "hp")));
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/FrontEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/FrontEnemy.csv", m_Limit, "Interval");

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
	_AttackState = ATTACK_WARP;
	return true;
}
//状態遷移
void (FrontEnemy::* FrontEnemy::stateTable[])() = {
	&FrontEnemy::Inter,//動きの合間
	&FrontEnemy::Attack,//攻撃
	&FrontEnemy::Teleport,//瞬間移動
};
//攻撃遷移
void (FrontEnemy::* FrontEnemy::attackTable[])() = {
	&FrontEnemy::FrontPlayerWarp,//プレイヤーの前に移動
	&FrontEnemy::FrontAttack,//正面に攻撃
};

//行動
void FrontEnemy::Action() {
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
	//攻撃時ジャンプする
	if (m_Jump) {
		m_AddPower -= m_Gravity;
		if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
			m_AddPower = {};
			m_Jump = false;
			m_Position.y = 0.1f;
		}
	}

	//鎌クラスの更新とか
	for (auto i = 0; i < sickle.size(); i++) {
		if (sickle[i] == nullptr)continue;
		sickle[i]->Update();

		if (!sickle[i]->GetAlive()) {
			sickle.erase(cbegin(sickle) + i);
		}
	}

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();
}

//描画
void FrontEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();

	for (auto i = 0; i < sickle.size(); i++) {
		if (sickle[i] == nullptr)continue;
		sickle[i]->Draw(dxCommon);
	}
	predictarea->Draw(dxCommon);
	if (m_Color.w != 0.0f)
		Obj_Draw();
	BaseBackDraw(dxCommon);

}
//ImGui描画
void FrontEnemy::ImGui_Origin() {
	ImGui::Begin("Area");
	ImGui::Text("AttackCount:%d", m_AttackCount);
	ImGui::Text("State:%d", (int)_charaState);
	ImGui::Text("cool:%d", coolTimer);
	ImGui::End();
	predictarea->ImGuiDraw();
}
//開放
void FrontEnemy::Finalize() {

}
//待機
void FrontEnemy::Inter() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	if (Helper::CheckMin(coolTimer, l_TargetTimer, 1) && (StagePanel::GetInstance()->GetPanelType(player->GetNowWidth() + 1, player->GetNowHeight()) == 0)
		&& (!StagePanel::GetInstance()->GetisEnemyHit(player->GetNowWidth() + 1, player->GetNowHeight()))) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		_AttackState = ATTACK_WARP;
	}
}
//攻撃
void FrontEnemy::Attack() {
	(this->*attackTable[_AttackState])();
	predictarea->Update();
	predictarea->SetTimer(coolTimer);
}

//ワープ
void FrontEnemy::Teleport() {
	const int l_RandTimer = Helper::GetRanNum(0, 30);
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_SPECIAL];

	if (Helper::CheckMin(coolTimer, l_TargetTimer,1)) {
		magic.Alive = true;
		if (m_Warp) {
			WarpEnemy(false);
		}
	}

	
}
//攻撃エリア
void FrontEnemy::BirthArea(const int Width, const int Height) {
	std::unique_ptr<Sickle> newarea = std::make_unique<Sickle>();
	newarea->Initialize();
	newarea->InitState(Width, Height,{m_Position.x - 1.5f,m_Position.y,m_Position.z});
	newarea->SetPlayer(player);
	sickle.emplace_back(std::move(newarea));
	predictarea->ResetPredict();

}
//予測エリア
void FrontEnemy::BirthPredict(const int Width, const int Height) {
	predictarea->SetPredict(Width, Height, true);
	predictarea->SetFlashStart(true);
}

//魔法陣生成
void FrontEnemy::BirthMagic() {
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
void FrontEnemy::WarpEnemy(bool Attack) {
	XMFLOAT3 l_RandPos = {};
	if (!Attack) {
		l_RandPos = StagePanel::GetInstance()->EnemySetPanel(m_LastEnemy);
	}
	else {
		l_RandPos = StagePanel::GetInstance()->FrontPlayerSetPanel();
	}
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//キャラが小さくなる
		if (Helper::FrameCheck(enemywarp.Frame, addFrame)) {
			enemywarp.Frame = {};
			enemywarp.AfterScale = 0.5f;
			enemywarp.State = WARP_END;
			m_Position = l_RandPos;
			if (Attack) {
				m_AttackWidth = player->GetNowWidth();
				m_AttackHeight = player->GetNowHeight();
			}
			m_RotFrame = {};
			m_Rotation.y = 270.0f;
			StagePanel::GetInstance()->EnemyHitReset();
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}
	else {			//キャラが大きくなっている
		if (Helper::FrameCheck(enemywarp.Frame, addFrame)) {
			enemywarp.Frame = {};
			enemywarp.AfterScale = 0.0f;
			m_Warp = false;
			if (Attack) {
				_AttackState = ATTACK_FRONT;
			}
			else {

				_charaState = STATE_INTER;
			}
			enemywarp.State = WARP_START;
			coolTimer = {};
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	}

	m_Scale = { enemywarp.Scale,enemywarp.Scale, enemywarp.Scale };
}
//攻撃時の動き
void FrontEnemy::AttackMove() {
	if (!m_Rot) { return; }
	const float l_AddFrame = 1 / 20.0f;
	if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
		m_Rotation.y = 270.0f;
		m_Rot = false;
		m_AttackFrame = {};
	}

	m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 630.0f);
}
//プレイヤーの前に移動
void FrontEnemy::FrontPlayerWarp() {
	magic.Alive = true;
	if (m_Warp) {
		WarpEnemy(true);
	}
}
//正面に攻撃
void FrontEnemy::FrontAttack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];
	if (m_AttackCount != 1) {
		if (coolTimer == 0) {		//予測エリア
			BirthPredict(m_AttackWidth,m_AttackHeight);
		}
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {		//実際の攻撃
			m_Jump = true;
			m_AddPower = 0.2f;
			m_Rot = true;
			BirthArea(m_AttackWidth, m_AttackHeight);
			coolTimer = {};
			StagePanel::GetInstance()->EnemyHitReset();
			m_CheckPanel = true;
			m_AttackCount = {};
			_charaState = STATE_SPECIAL;
		}
		predictarea->SetTargetTimer(l_TargetTimer);
	}
}
//クリアシーンの更新
void FrontEnemy::ClearAction() {
	const int l_TargetTimer = 40;
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
void FrontEnemy::GameOverAction() {
	if (_GameOverState == OVER_STOP) {
		m_Position = { -2.0f,0.0f,2.5f };
		m_Rotation = { 0.0f,180.0f,0.0f };
		m_AddDisolve = 0.0f;
	}

	Obj_SetParam();
}