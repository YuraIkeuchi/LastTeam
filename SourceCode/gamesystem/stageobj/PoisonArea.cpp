#include "PoisonArea.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <ModelManager.h>
#include <GameStateManager.h>
#include <Helper.h>
//読み込み
PoisonArea::PoisonArea() {
	panels.tex.reset(new IKETexture(ImageManager::POISONAREA, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	panels.tex->TextureCreate();
	panels.tex->Initialize();
	panels.tex->SetScale({});
	panels.tex->SetRotation({ 90.0f,0.0f,0.0f });

	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BULLET);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	//予測
	predict.area.reset(new PredictArea("ENEMY"));
	predict.area->Initialize();
	Initialize();
}
//初期化
bool PoisonArea::Initialize() {
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "POISON_DAMAGE")));
	m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/PoisonEnemy.csv", "POISON_TIMER")));
	return true;
}

//ステータス初期化
void PoisonArea::InitState(const int width, const int height) {
	//パネル文
	panels.position = SetPanelPos(width, height);
	panels.Width = width, panels.Height = height;
	panels.position.y = 0.011f;
	panels.color = { 1.0f,1.0f,1.0f,1.0f };
	panels.Alive = true;
	panels.Timer = 40;
	//弾
	m_BulletAlive = true;
	float baseScale = PANEL_SIZE * 0.1f;
	m_Scale = { baseScale,baseScale,baseScale };
	m_Color = { 0.5f,1.0f,0.5f,1.0f };
}

//更新
void PoisonArea::Update() {
	
	Obj_SetParam();
	Collide();
	Move();
	panels.tex->Update();
	panels.tex->SetPosition(panels.position);
	panels.tex->SetColor(panels.color);
	panels.tex->SetScale({ panels.scale,panels.scale,panels.scale });
}
//描画
void PoisonArea::Draw(DirectXCommon* dxCommon) {
	predict.area->Draw(dxCommon);
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	panels.tex->Draw();
	IKETexture::PostDraw();
	if(m_BulletAlive)
	Obj_Draw();
}
//ImGui
void PoisonArea::ImGuiDraw() {
	ImGui::Begin("Poison");
	ImGui::Text("PosX:%f,PosZ:%f", panels.position.x, panels.position.z);
	ImGui::Text("Timer:%d", panels.DamageTimer);
	ImGui::End();
}
//パネルの位置に置く
XMFLOAT3 PoisonArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
void PoisonArea::Collide() {
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	if (_PoisonState != POISON_WIDE) { return; }
	//毒のマスとプレイヤーが一緒だとダメージを食らう
	if (panels.Width == l_PlayerWidth && panels.Height == l_PlayerHeight) {
		panels.DamageTimer++;
	}
	else {
		if (panels.Damage) {
			panels.DamageTimer = {};
		}
	}

	//
	if (panels.DamageTimer == 50) {
		player->RecvDamage(m_Damage,"POISON");
		panels.DamageTimer = {};
		panels.Damage = true;
	}
}
void PoisonArea::Move() {
	const float l_TargetPosY = 15.0f;
	const float l_ThrowSpeed = 0.25f;

	if (_PoisonState == POISON_THROW) {			//上に上げる
		BirthPredict(panels.Width, panels.Height);
		if (Helper::CheckMin(m_Position.y, l_TargetPosY, l_ThrowSpeed)) {
			_PoisonState = POISON_DROP;
			m_Position = { panels.position.x,l_TargetPosY,panels.position.z };
		}
	}
	else if (_PoisonState == POISON_DROP) {		//落ちてくる
		if (Helper::CheckMax(m_Position.y, 0.0f, -l_ThrowSpeed)) {
			_PoisonState = POISON_WIDE;
			m_BulletAlive = false;
		}
	}
	else if (_PoisonState == POISON_WIDE) {		//広がる
		static float addFrame = 1.f / 15.f;
		if (Helper::FrameCheck(panels.frame, addFrame)) {		//広がるためのイージング
			if (Helper::CheckMin(panels.Timer, m_TargetTimer, 1)) {		//広がり切ったらカウントを加算する
				_PoisonState = POISON_END;
				panels.frame = {};
				panels.afterscale = {};
			}
		}
		predict.area->ResetPredict();
		panels.scale = Ease(In, Cubic, panels.frame, panels.scale, panels.afterscale);
	}
	else {		//なくなる
		static float addFrame = 1.f / 15.f;
		if (Helper::FrameCheck(panels.frame, addFrame)) {
			panels.Alive = false;
		}
		panels.scale = Ease(In, Cubic, panels.frame, panels.scale, panels.afterscale);
	}

	if (_PoisonState == POISON_THROW || _PoisonState == POISON_DROP) {
		predict.Timer++;
		predict.area->SetTimer(predict.Timer);
		predict.area->SetTargetTimer(predict.Target);
		predict.area->Update();
	}
	//predictarea->SetTimer(coolTimer);
}
//予測エリア
void PoisonArea::BirthPredict(const int Width, const int Height) {
	predict.area->SetPredict(Width, Height, true);
	predict.area->SetFlashStart(true);
}