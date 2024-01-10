#include "CanonEnemy.h"
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
CanonEnemy::CanonEnemy() {

	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::CANNON));

	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });

	bullets = make_unique<EnemyBullet>();
	bullets->Initialize();
	bullets->SetPlayer(player);
	bullets->SetTargetTimer(5);

	//shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	//shadow_tex->TextureCreate();
	//shadow_tex->Initialize();
	//shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });

}
//初期化
bool CanonEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,270.0f,0.0f };
	auto LimitSize = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/CanonEnemy.csv", "LIMIT_NUM")));

	m_Limit.resize(LimitSize);
	LoadCSV::LoadCsvParam_Int("Resources/csv/chara/enemy/CanonEnemy.csv", m_Limit, "Interval");

	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/CanonEnemy.csv", "hp")));
	m_BulletNum = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/CanonEnemy.csv", "BULLET_NUM")));

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
	enemywarp.Scale = 0.6f;
	m_BaseScale = 0.6f;
	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	m_AddDisolve = 2.0f;
	return true;
}

void (CanonEnemy::* CanonEnemy::stateTable[])() = {
	&CanonEnemy::Inter,//動きの合間
	&CanonEnemy::Attack,//動きの合間
	&CanonEnemy::Teleport,//瞬間移動
};

//行動
void CanonEnemy::Action() {
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

	bullets->Update();
	
	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	//shadow_tex->SetPosition(m_ShadowPos);
	//shadow_tex->SetScale(m_ShadowScale);
	//shadow_tex->Update();

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();
}

//描画
void CanonEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	magic.tex->Draw();
	BaseFrontDraw(dxCommon);
	IKETexture::PostDraw();

	if (bullets->GetAlive()) {
		bullets->Draw(dxCommon);
	}
	////敵の弾
	//for (unique_ptr<EnemyBullet>& newbullet : bullets) {
	//	if (newbullet != nullptr) {
	//		newbullet->Draw(dxCommon);
	//	}
	//}
	if (m_Color.w != 0.0f)
		Obj_Draw();
	BaseBackDraw(dxCommon);

}
//ImGui描画
void CanonEnemy::ImGui_Origin() {
	//敵の弾
	/*for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->ImGuiDraw();
		}
	}*/
	bullets->ImGuiDraw();
	ImGui::Begin("Canon");
	ImGui::Text("ShotDir:%d", m_ShotDir);
	ImGui::End();
}
//開放
void CanonEnemy::Finalize() {

}
//待機
void CanonEnemy::Inter() {
	const int l_RandTimer = Helper::GetRanNum(0, 40);
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_INTER];
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, l_TargetTimer + l_RandTimer);
	if (coolTimer == l_TargetTimer + l_RandTimer) {
		coolTimer = {};
		_charaState = STATE_ATTACK;
	}
}
//攻撃
void CanonEnemy::Attack() {
	int l_TargetTimer = {};
	l_TargetTimer = m_Limit[STATE_ATTACK];
	const float l_AddFrame = 1 / 30.0f;
	float l_AddScaleFrame = {};
	float l_AfterScale = {};
	//弾の生成関係
	if (_CanonType == CANON_SET) {
		l_AddScaleFrame = 1 / 20.0f;
		l_AfterScale = 0.9f;
		if (coolTimer == 101) {		//ここで撃つ方向を決める
			m_ShotDir = 2;
			//敵が端にいた場合反射によって回転が変に見えるから指定する
			if (m_NowHeight == 0 && m_ShotDir == 2) {
				m_ShotDir = 1;
			} else if (m_NowHeight == 3 && m_ShotDir == 1) {
				m_ShotDir = 2;
			}
			//弾を撃つ方向で向きが変わる
			if (m_ShotDir == 0) {
				m_AfterRotY = 270.0f;
			} else if (m_ShotDir == 1) {
				m_AfterRotY = 315.0f;
			} else {
				m_AfterRotY = 225.0f;
			}
		}
		if (Helper::CheckMin(coolTimer, l_TargetTimer, 1)) {
			if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
				if (Helper::FrameCheck(m_ScaleFrame, l_AddFrame)) {
					m_RotFrame = {};
					m_ScaleFrame = {};
					coolTimer = {};
					_CanonType = CANON_THROW;
					BirthBullet();
				}
			}

			m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
			m_Rotation.y = Ease(In, Cubic, m_RotFrame, m_Rotation.y, m_AfterRotY);
		}
	} else if (_CanonType == CANON_THROW) {
		l_AfterScale = 0.6f;
		l_AddScaleFrame = 1 / 10.0f;
		if (Helper::FrameCheck(m_ScaleFrame, l_AddScaleFrame)) {
			m_ScaleFrame = {};
			m_AttackCount++;
			if (m_AttackCount != m_BulletNum) {
				_CanonType = CANON_SET;
			}
			else {
				_CanonType = CANON_END;
			}
		}
		m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
	} else {
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
		coolTimer = {};
		_CanonType = CANON_SET;
		StagePanel::GetInstance()->EnemyHitReset();
	}

	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
}

//ワープ
void CanonEnemy::Teleport() {
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
//弾の生成
void CanonEnemy::BirthBullet() {
	/// <summary>
	///	音入れ(弾を打つ音希望(ポンッみたいなやつ))
	/// </summary>
	Audio::GetInstance()->PlayWave("Resources/Sound/SE/Damage.wav", 0.02f);
	//弾の発生
	bullets->InitState({ m_Position.x,m_Position.y + 0.5f,m_Position.z }, m_ShotDir);
}
//魔法陣生成
void CanonEnemy::BirthMagic() {
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
	} else {			//魔法陣を縮める
		if (Helper::FrameCheck(magic.Frame, addFrame)) {
			magic.Frame = {};
			magic.AfterScale = 0.2f;
			magic.Alive = false;
			magic.State = MAGIC_BIRTH;
		}
		magic.Scale = Ease(In, Cubic, magic.Frame, magic.Scale, magic.AfterScale);
	}
}
void CanonEnemy::WarpEnemy() {
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel(m_LastEnemy);
	static float addFrame = 1.f / 15.f;
	if (enemywarp.State == WARP_START) {			//キャラが小さくなる
		if (Helper::FrameCheck(enemywarp.Frame, addFrame)) {
			enemywarp.Frame = {};
			enemywarp.AfterScale = 0.6f;
			enemywarp.State = WARP_END;
			coolTimer = {};
			m_Position = l_RandPos;
			m_RotFrame = {};
			m_Rotation.y = 270.0f;
			StagePanel::GetInstance()->EnemyHitReset();
		}
		enemywarp.Scale = Ease(In, Cubic, enemywarp.Frame, enemywarp.Scale, enemywarp.AfterScale);
	} else {			//キャラが大きくなっている
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
//クリアシーンの更新
void CanonEnemy::ClearAction() {
	const int l_TargetTimer = 70;
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
//ゲームーオーバーシーンの更新
void CanonEnemy::GameOverAction() {
	float l_AddScaleFrame = {};
	float l_AfterScale = {};

	if (_GameOverState == OVER_STOP) {
		m_Position = { 1.0f,0.0f,1.5f };
		m_Rotation = { 0.0f,180.0f,0.0f };
		m_AddDisolve = 0.0f;
		if (player->GetSelectType() == 1) {
			_GameOverState = OVER_YES;
			m_AddPower = 0.3f;
			_CanonType = CANON_SET;
		}
		else if (player->GetSelectType() == 2) {
			_GameOverState = OVER_NO;
		}
	}
	else if (_GameOverState == OVER_YES) {
		if (_CanonType == CANON_SET) {
			l_AddScaleFrame = 1 / 30.0f;
			l_AfterScale = 0.9f;
			if (Helper::FrameCheck(m_ScaleFrame, l_AddScaleFrame)) {
				if (Helper::CheckMin(m_OverTimer, 10, 1)) {
					_CanonType = CANON_THROW;
					m_OverTimer = {};
					m_ScaleFrame = {};
				}
			}
			m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
		}
		else if (_CanonType == CANON_THROW) {
			l_AfterScale = 0.6f;
			l_AddScaleFrame = 1 / 10.0f;
			if (Helper::FrameCheck(m_ScaleFrame, l_AddScaleFrame)) {
				if (Helper::CheckMin(m_OverTimer, 40, 1)) {
					_CanonType = CANON_SET;
					m_OverTimer = {};
					m_ScaleFrame = {};
				}
			}
			m_BaseScale = Ease(In, Cubic, m_ScaleFrame, m_BaseScale, l_AfterScale);
		}
		
		m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	}
	else {
		const float l_AddRotZ = 0.5f;
		const float l_AddFrame2 = 0.01f;
		float RotPower = 12.0f;
		if (Helper::FrameCheck(m_RotFrame, l_AddFrame2)) {		//最初はイージングで回す
			m_RotFrame = 1.0f;
			if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//最後は倒れる
				m_Rotation.z = 90.0f;
			}
		}
		else {
			RotPower = Ease(In, Cubic, m_RotFrame, RotPower, 25.0f);
			m_Rotation.z = Ease(In, Cubic, m_RotFrame, m_Rotation.z, 45.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_RotFrame, m_Position.y, 0.5f);
		}
	}


	Obj_SetParam();
}