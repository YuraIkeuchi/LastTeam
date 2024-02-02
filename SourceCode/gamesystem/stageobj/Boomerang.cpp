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

	dir_tex.reset(new IKETexture(ImageManager::BOOM_DIR, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	dir_tex->TextureCreate();
	dir_tex->Initialize();
	dir_tex->SetRotation(m_DirRot);
	dir_tex->SetScale({ 0.05f,0.05f,0.05f});
	//�\��
	predictArea = std::make_unique<PredictArea>("ENEMY");
	predictArea->Initialize();
}
//������
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
	m_Hit = false;
	m_HitTimer = {};
	m_DirRot.y = 270.0f;
	//�e�̃}�X���擾����
	StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
}

//�X�V
void Boomerang::Update() {
	Throw();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();

	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	Collide();		//�����蔻��

	//dir_tex->SetPosition({ m_Position.x,m_Position.y + 0.5f,m_Position.z });
	//dir_tex->SetRotation(m_DirRot);
	//dir_tex->Update();
	predictArea->Update();
	m_Rotation.y += 20.0f;
}
//�`��
void Boomerang::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//dir_tex->Draw();
	IKETexture::PostDraw();
	predictArea->Draw(dxCommon);
	Obj_Draw();
}
//ImGui�`��
void Boomerang::ImGuiDraw() {
}

//�����蔻��
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
//�Ǐ]
void Boomerang::Throw() {
	if (!m_Alive) { return; }
	const float l_AddFrame = 0.1f;
	const int l_BaseTimer = 40;
	const float l_AddCircle = 2.0f;
	//�e�̃}�X���擾����
	StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
	//�e�̃Z�b�g(���񂾂񕂂��ш��ӂ���悤�Ȋ���)
	if (m_ThrowType == THROW_SET) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_TargetPos = { -6.0f,m_Position.y,m_Position.z };
			int nextWidthPanel = m_NowWidth - (nextPredict + 1);
			if (nextWidthPanel < 0) {
				/// <summary>
				///	����(�Ԃ�񂷉���](�u�[������������Ă鉹�݂����Ȃ�A�����Ȃ�U��񂵂Ă���))
				/// </summary>
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/boomerang.wav", 0.05f);
				m_ThrowType = THROW_PLAY;
				_MoveDir = MOVE_STRAIGHT;
				nextPredict = 0;
				predictFrame = 0.f;
				m_Frame = {};
				return;
			}

			if (Helper::FrameCheck(predictFrame, 1.f / 5.0f)) {
				predictArea->VersePredict(nextWidthPanel, m_NowHeight);
				nextPredict++;
				predictFrame = 0.f;
			}
		}
		else {
			m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, 0.3f);
		}
	}
	//���ۂɑ_�����Ⴄ
	else {
		if (_MoveDir == MOVE_STRAIGHT) {
			predictArea->VanishPredict(m_NowWidth, m_NowHeight);
			//��_�Ԃ̋����v�Z
			m_Length = Helper::ChechLength(m_Position,m_TargetPos);
			//���̍U�����_��
			if (m_Length > 0.1f) {
				Helper::FollowMove(m_Position, m_TargetPos, m_Speed);
			}
			else {
				//�ŏ��̍��W�ɂ���ď�s�������s�������܂�
				if (m_Position.z < 2.0f) {
					if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
						int nextHeightPanel = m_NowHeight + (nextPredict + 1);
						if (nextHeightPanel > 3) {
							_MoveDir = MOVE_UP;
							m_RotFrame = {};
							m_ThrowType = THROW_PLAY;
							nextPredict = 0;
							predictFrame = 0.f;
							/// <summary>
							///	����(�Ԃ�񂷉���](�u�[������������Ă鉹�݂����Ȃ�A�����Ȃ�U��񂵂Ă���))
							/// </summary>
							Audio::GetInstance()->PlayWave("Resources/Sound/SE/boomerang.wav", 0.05f);
							return;
						}

						if (Helper::FrameCheck(predictFrame, 1.f / 5.0f)) {
							predictArea->VersePredict(m_NowWidth, nextHeightPanel);
							nextPredict++;
							predictFrame = 0.f;
						}
					
					}
					else {
						m_DirRot.y = Ease(In, Cubic, m_RotFrame, m_DirRot.y, 360.0f);
					}
				}
				else {
					if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
						int nextHeightPanel = m_NowHeight - (nextPredict + 1);
						if (nextHeightPanel < 0) {
							_MoveDir = MOVE_DOWN;
							m_RotFrame = {};
							m_ThrowType = THROW_PLAY;
							nextPredict = 0;
							predictFrame = 0.f;
							/// <summary>
							///	����(�Ԃ�񂷉���](�u�[������������Ă鉹�݂����Ȃ�A�����Ȃ�U��񂵂Ă���))
							/// </summary>
							Audio::GetInstance()->PlayWave("Resources/Sound/SE/boomerang.wav", 0.05f);
							return;
						}

						if (Helper::FrameCheck(predictFrame, 1.f / 5.0f)) {
							predictArea->VersePredict(m_NowWidth, nextHeightPanel);
							nextPredict++;
							predictFrame = 0.f;
						}
					}
					else {
						m_DirRot.y = Ease(In, Cubic, m_RotFrame, m_DirRot.y, 180.0f);
					}
				}
			}
		}
		else if (_MoveDir == MOVE_UP) {
			predictArea->VanishPredict(m_NowWidth, m_NowHeight);
			m_TargetPos = { m_Position.x,m_Position.y,4.0f };
			//��_�Ԃ̋����v�Z
			m_Length = Helper::ChechLength(m_Position, m_TargetPos);
			//���̍U�����_��
			if (m_Length > 0.1f) {
				Helper::FollowMove(m_Position, m_TargetPos, m_Speed);
			}
			else {
				if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
					int nextWidthPanel = m_NowWidth + (nextPredict + 1);
					if (nextWidthPanel > 7) {
						_MoveDir = MOVE_RETURN;
						m_RotFrame = {};
						nextPredict = 0;
						predictFrame = 0.f;
						/// <summary>
						///	����(�Ԃ�񂷉���](�u�[������������Ă鉹�݂����Ȃ�A�����Ȃ�U��񂵂Ă���))
						/// </summary>
						Audio::GetInstance()->PlayWave("Resources/Sound/SE/boomerang.wav", 0.05f);
						return;
					}

					if (Helper::FrameCheck(predictFrame, 1.f / 5.0f)) {
						predictArea->VersePredict(nextWidthPanel, m_NowHeight);
						nextPredict++;
						predictFrame = 0.f;
					}
				}
				else {
					m_DirRot.y = Ease(In, Cubic, m_RotFrame, m_DirRot.y, 450.0f);
				}
			}
		}
		else if (_MoveDir == MOVE_DOWN) {
			predictArea->VanishPredict(m_NowWidth, m_NowHeight);
			m_TargetPos = { m_Position.x,m_Position.y,0.0f };
			//��_�Ԃ̋����v�Z
			m_Length = Helper::ChechLength(m_Position, m_TargetPos);
			//���̍U�����_��
			if (m_Length > 0.1f) {
				Helper::FollowMove(m_Position, m_TargetPos, m_Speed);
			}
			else {
				if (Helper::FrameCheck(m_RotFrame, l_AddFrame)) {
					int nextWidthPanel = m_NowWidth + (nextPredict + 1);
					if (nextWidthPanel > 7) {
						_MoveDir = MOVE_RETURN;
						m_RotFrame = {};
						nextPredict = 0;
						predictFrame = 0.f;
						/// <summary>
						///	����(�Ԃ�񂷉���](�u�[������������Ă鉹�݂����Ȃ�A�����Ȃ�U��񂵂Ă���))
						/// </summary>
						Audio::GetInstance()->PlayWave("Resources/Sound/SE/boomerang.wav", 0.05f);
						return;
					}

					if (Helper::FrameCheck(predictFrame, 1.f / 5.0f)) {
						predictArea->VersePredict(nextWidthPanel, m_NowHeight);
						nextPredict++;
						predictFrame = 0.f;
					}
				}
				else {
					m_DirRot.y = Ease(In, Cubic, m_RotFrame, m_DirRot.y, 90.0f);
				}
			}
		}
		else {
		predictArea->VanishPredict(m_NowWidth, m_NowHeight);
			m_TargetPos = { 5.0f,m_Position.y,m_Position.z };
			Helper::FollowMove(m_Position, m_TargetPos, m_Speed);
			//��_�Ԃ̋����v�Z
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
