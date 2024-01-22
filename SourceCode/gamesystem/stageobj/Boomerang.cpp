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

	dir_tex.reset(new IKETexture(ImageManager::BOOM_DIR, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	dir_tex->TextureCreate();
	dir_tex->Initialize();
	dir_tex->SetRotation(m_DirRot);
	dir_tex->SetScale({ 0.05f,0.05f,0.05f});
}
//èâä˙âª
bool Boomerang::Initialize() {
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ThrowEnemy.csv", "BULLET_DAMAGE")));
	m_Speed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/ThrowEnemy.csv", "SPEED")));
	return true;
}
void Boomerang::InitState(const XMFLOAT3& pos) {
	m_Position = pos;
	m_Rotation.y = 270.0f;
	m_Scale = { 0.0f,0.0f,0.0f };
	m_BaseScale = {};
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_AddSpeed = 1.0f;
	m_Alive = true;
	m_ThrowType = THROW_SET;
	m_AliveTimer = {};
	panels.position = {};
	panels.color = { 1.f,1.f,1.f,1.f };
	m_Hit = false;
	m_HitTimer = {};
	m_DirRot.y = 270.0f;
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

	dir_tex->SetPosition({ m_Position.x,m_Position.y + 0.5f,m_Position.z });
	dir_tex->SetRotation(m_DirRot);
	dir_tex->Update();

	m_Rotation.y += 20.0f;
}
//ï`âÊ
void Boomerang::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	panels.tex->Draw();
	dir_tex->Draw();
	IKETexture::PostDraw();
	Obj_Draw();
}
//ImGuiï`âÊ
void Boomerang::ImGuiDraw() {
}

//ìñÇΩÇËîªíË
bool Boomerang::Collide() {
	if (m_Hit) { return false; }
	if (!m_Alive) { return false; }
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 0.15f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius)) {
		player->RecvDamage(m_Damage, "NORMAL");
		m_Hit = true;
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
					if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
						_MoveDir = MOVE_UP;
						m_RotFrame = {};
					}
					else {
						m_DirRot.y = Ease(In, Cubic, m_RotFrame, m_DirRot.y, 360.0f);
					}
				}
				else {
					if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
						_MoveDir = MOVE_DOWN;
						m_RotFrame = {};
					}
					else {
						m_DirRot.y = Ease(In, Cubic, m_RotFrame, m_DirRot.y, 180.0f);
					}
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
				if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
					_MoveDir = MOVE_RETURN;
					m_RotFrame = {};
				}
				else {
					m_DirRot.y = Ease(In, Cubic, m_RotFrame, m_DirRot.y, 450.0f);
				}
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
				if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
					_MoveDir = MOVE_RETURN;
					m_RotFrame = {};
				}
				else {
					m_DirRot.y = Ease(In, Cubic, m_RotFrame, m_DirRot.y, 90.0f);
				}
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

	if (m_Hit) {
		if (Helper::CheckMin(m_HitTimer, 20, 1)) {
			m_Hit = false;
			m_HitTimer = {};
		}
	}
}
