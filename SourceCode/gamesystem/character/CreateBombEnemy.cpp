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
#include "EnemyManager.h"

//モデル読み込み
CreateBombEnemy::CreateBombEnemy() {
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
//初期化
bool CreateBombEnemy::Initialize() {
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

void (CreateBombEnemy::* CreateBombEnemy::stateTable[])() = {
	&CreateBombEnemy::Inter,//動きの合間
	&CreateBombEnemy::Attack,//動きの合間
	&CreateBombEnemy::Teleport,//瞬間移動
};

//行動
void CreateBombEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//当たり判定
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定
	PoisonState();//毒
	BirthMagic();//魔法陣

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();

	//障害物の削除
	for (int i = 0; i < bomb.size(); i++) {
		if (bomb[i] == nullptr) {
			continue;
		}
		if (bomb[i]->GetHP() <= 0) {
			//敵全てにダメージ
			//interEnemy42のエネミータグを使う？(そうなるとここじゃない所で処理した方がよさそう
			//アタックエリアでbombを作ってばらまく？
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
	shadow_tex->Draw();
	magic.tex->Draw();
	IKETexture::PostDraw();

	//障害物の削除
	for (int i = 0; i < bomb.size(); i++) {
		if (bomb[i] == nullptr) {
			continue;
		}

		bomb[i]->Draw(dxCommon);
	}
	Obj_Draw();
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
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	if (coolTimer == kIntervalMax) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		//BirthBomb();
	}
}
//攻撃
void CreateBombEnemy::Attack() {
	const int l_TargetTimer = 200;

	if (_BombType == Bomb_SET) {
		if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
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
	const int l_TargetTimer = 200;

	if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
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
	newBomb->SetPosition(StagePanel::GetInstance()->EnemySetPanel());
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

		if (Helper::GetInstance()->FrameCheck(magic.Frame, addFrame)) {
			if (Helper::GetInstance()->CheckMin(magic.Timer, l_TargetTimer, 1)) {
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
		if (Helper::GetInstance()->FrameCheck(magic.Frame, addFrame)) {
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
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel();
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//キャラが小さくなる
		if (Helper::GetInstance()->FrameCheck(enemywarp.Frame, addFrame)) {
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
		if (Helper::GetInstance()->FrameCheck(enemywarp.Frame, addFrame)) {
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
