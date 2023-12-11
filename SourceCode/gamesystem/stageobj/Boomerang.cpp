#include "Boomerang.h"
#include "Collision.h"
#include "Player.h"
#include "Helper.h"
#include <StagePanel.h>
#include <Easing.h>
#include <ImageManager.h>
#include "GameStateManager.h"

Boomerang::Boomerang() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BOOMERAN);
	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	panels.tex.reset(new IKETexture(ImageManager::AREA, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	panels.tex->TextureCreate();
	panels.tex->Initialize();
	float baseScale = PANEL_SIZE * 0.1f;
	panels.tex->SetScale({ baseScale,baseScale,baseScale });
	panels.tex->SetRotation({ 90.0f,0.0f,0.0f });

	shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//èâä˙âª
bool Boomerang::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation.y = 270.0f;
	m_Scale = { 0.0f,0.0f,0.0f };
	m_BaseScale = {};
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_AddSpeed = 1.0f;
	m_Alive = true;
	m_ThrowType = THROW_SET;
	m_AliveTimer = {};
	panels.position = {};
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ThrowEnemy.csv", "BULLET_DAMAGE")));
	m_Speed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ThrowEnemy.csv", "SPEED")));
	panels.color = { 1.f,1.f,1.f,1.f };
	return true;
}

//çXêV
void Boomerang::Update() {
	Throw();
	//É^ÉCÉvÇ…ÇÊÇ¡ÇƒêFÇàÍíUïœÇ¶ÇƒÇÈ
	Obj_SetParam();

	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	Collide();		//ìñÇΩÇËîªíË

	panels.position = { (PANEL_SIZE * m_NowWidth) - (PANEL_HEIGHT * PANEL_SIZE) ,0.011f,(PANEL_SIZE * m_NowHeight) };
	panels.tex->SetPosition(panels.position);
	panels.tex->SetColor({ 1.0f,0.3f,0.0f,1.0f });
	panels.tex->Update();

	m_Rotation.y += 20.0f;
}
//ï`âÊ
void Boomerang::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	panels.tex->Draw();
	shadow_tex->Draw();
	IKETexture::PostDraw();
	Obj_Draw();
}
//ImGuiï`âÊ
void Boomerang::ImGuiDraw() {
}

//ìñÇΩÇËîªíË
bool Boomerang::Collide() {
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 0.15f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (m_Alive)) {
		player->RecvDamage(m_Damage, "NORMAL");
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}

	return false;
}
//í«è]
void Boomerang::Throw() {
	const float l_AddFrame = 0.1f;
	const int l_BaseTimer = 40;
	const float l_AddCircle = 2.0f;
	//íeÇÃÉ}ÉXÇéÊìæÇ∑ÇÈ
	StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
	//íeÇÃÉZÉbÉg(ÇæÇÒÇæÇÒïÇÇ©Ç—àßÇ”Ç™ÇÈÇÊÇ§Ç»ä¥Ç∂)
	if (m_ThrowType == THROW_SET) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_ThrowType = THROW_PLAY;
			_MoveDir = MOVE_STRAIGHT;
			m_TargetPos = { -6.0f,m_Position.y,m_Position.z };
		}

		m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, 0.3f);
	}
	//é¿ç€Ç…ë_Ç¡ÇøÇ·Ç§
	else {
		if (_MoveDir == MOVE_STRAIGHT) {
			//ìÒì_ä‘ÇÃãóó£åvéZ
			m_Length = Helper::ChechLength(m_Position,m_TargetPos);
			//éüÇÃçUåÇêÊÇë_Ç§
			if (m_Length > 0.1f) {
				Helper::FollowMove(m_Position, m_TargetPos, m_Speed);
			}
			else {
				//ç≈èâÇÃç¿ïWÇ…ÇÊÇ¡Çƒè„çsÇ≠Ç©â∫çsÇ≠Ç©åàÇ‹ÇÈ
				if (m_Position.z < 2.0f) {
					_MoveDir = MOVE_UP;
				}
				else {
					_MoveDir = MOVE_DOWN;
				}
			}
		}
		else if (_MoveDir == MOVE_UP) {
			m_TargetPos = { m_Position.x,m_Position.y,4.0f };
			//ìÒì_ä‘ÇÃãóó£åvéZ
			m_Length = Helper::ChechLength(m_Position, m_TargetPos);
			//éüÇÃçUåÇêÊÇë_Ç§
			if (m_Length > 0.1f) {
				Helper::FollowMove(m_Position, m_TargetPos, m_Speed);
			}
			else {
				_MoveDir = MOVE_RETURN;
			}
		}
		else if (_MoveDir == MOVE_DOWN) {
			m_TargetPos = { m_Position.x,m_Position.y,0.0f };
			//ìÒì_ä‘ÇÃãóó£åvéZ
			m_Length = Helper::ChechLength(m_Position, m_TargetPos);
			//éüÇÃçUåÇêÊÇë_Ç§
			if (m_Length > 0.1f) {
				Helper::FollowMove(m_Position, m_TargetPos, m_Speed);
			}
			else {
				_MoveDir = MOVE_RETURN;
			}
		}
		else {
			m_TargetPos = { 5.0f,m_Position.y,m_Position.z };
			Helper::FollowMove(m_Position, m_TargetPos, m_Speed);
			//ìÒì_ä‘ÇÃãóó£åvéZ
			m_Length = Helper::ChechLength(m_Position, m_TargetPos);
			if (m_Length < 0.1f) {
				m_Alive = false;
			}
		}
	}
}
