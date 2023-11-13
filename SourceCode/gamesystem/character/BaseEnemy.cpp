#include "character/BaseEnemy.h"

#include "Helper.h"
#include "StagePanel.h"
#include "CsvLoader.h"



BaseEnemy::BaseEnemy():
	GameObject("Enemy")
{
	// �r�w�C�r�A�c���[����
	behavior_tree_ = std::make_unique<behaviorTree::SimpleBehaviorTree>(this);	
}

bool BaseEnemy::Initialize()
{
	return false;
}

void BaseEnemy::Update()
{
	// �c���[�X�V
	behavior_tree_->Update();
	Obj_SetParam();
	
}

void BaseEnemy::Draw()
{
	Obj_Draw();
}

void BaseEnemy::UIDraw()
{
}

void BaseEnemy::ImGuiDraw()
{
}

XMFLOAT3 BaseEnemy::RandPanelPos()
{
	panel_position_ =
	{
		Helper::GetInstance()->GetRanNum(4, 7),
		Helper::GetInstance()->GetRanNum(0, 3)
	};
	return StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);
}

TestEnemy::TestEnemy()
{
	// ���f���Z�b�g
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	
	// �c���[���s�J�n
	CreateTree();
	behavior_tree_->Start();


}

bool TestEnemy::Initialize()
{
	// ���W�Z�b�g
	m_Position = RandPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };

	// HP�Z�b�g
	hitpoint_ = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "hp")));
	max_hitpoint_ = hitpoint_;

	return false;
}

void TestEnemy::Draw()
{
	Obj_Draw();
}

void TestEnemy::UIDraw()
{
}


void TestEnemy::ImGuiDraw()
{
	ImGui::Begin("TestEnemy");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::Text("Width:%d", panel_position_.width);
	ImGui::Text("Height:%d", panel_position_.height);
	ImGui::End();
}

void TestEnemy::CreateTree()
{
	// �r�w�C�r�A�c���[�ݒ�f��
	// �|�[��(�X�v���b�h�V�[�g4p)�z��

	// �R���f�B�V�����m�[�h
	// �V�[�P���T��Z���N�^�ŏ���������s�����߂̃m�[�h
	// �����_�����g�p�ł��邽�߁A���G�Ȕ�����\
	ConditionalNode *front_check = new ConditionalNode // �őO��ɂ��邩���肷��m�[�h
	(
		[&](GameObject *owner)
		{
			return panel_position_.width == 4 ?	// �G�w�n�̍őO��ɂ���ΐ���
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	front_check->Name = "�őO��ɂ��邩";


	// �A�N�V�����m�[�h
	// ���ۂɉ����̏������s���m�[�h
	// Ranning��Ԃ��ꍇ���s�r�����Ӗ����A�����𕪊�ł���
	ActionNode *pawn_attack = new ActionNode	// �΂߂ɍU��
	(
		[&](GameObject *owner)
		{
			// �A�j���[�V�����̑���̋^���J�E���^
			static int temp_counter = 0;

			static bool init_flag = false;
			// ��x�����A�j���[�V��������
			if (!init_flag)
			{
				// �U�����[�V�����̎��s��������

				// �U�����萶��������������

				init_flag = true;
				temp_counter = 0;
			}
			// �A�j���[�V�������I�������烉���_���ȍŌ��Ɉړ����ďI��
			if (temp_counter >= 60)
			{
				// �Ō���Ɉړ�
				panel_position_ =
				{
					7,
					Helper::GetInstance()->GetRanNum(0, 3)
				};
				m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);
				init_flag = false;
				return BehaviorStatus::Sucess;
			}

			temp_counter++;

			return BehaviorStatus::Runnning;
		}
	);
	pawn_attack->Name = "�O��1�}�X�΂߂ɍU��";

	// �A�N�V�����m�[�h
	// ���ۂɉ����̏������s���m�[�h
	// Ranning��Ԃ��ꍇ���s�r�����Ӗ����A�����𕪊�ł���
	ActionNode *move = new ActionNode	// �ړ�
	(
		[&](GameObject *owner)
		{
			// �^���J�E���^
			static int temp_diray_counter = 0;	// �ړ�����̑ҋ@�p�J�E���^ 
			static int temp_move_counter = 0;	// �ړ��Ԋu�p�J�E���^
			
			static bool init_flag = false;			// �^��������
			// ��x�����A�j���[�V��������
			if (!init_flag)
			{
				// �U�����[�V�����̎��s��������

				// �U�����萶��������������

				temp_diray_counter = 0;
				temp_move_counter = 0;
				init_flag = true;
			}
			// �f�B���C���I��������ړ��J�n
			// �i���ۂɂ̓f�B���C�p�Ƀm�[�h�𕪂��������������A�������񂱂̂܂܎����j
			if (temp_diray_counter >= 90)
			{
				if (temp_move_counter >= 30)
				{
					temp_move_counter = 0;

					// �Ō���Ɉړ�
					panel_position_ =
					{
						panel_position_.width - 1,
						panel_position_.height,
					};
					m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);

					if (panel_position_.width == 4)
					{
						init_flag = false;
						return BehaviorStatus::Sucess;
					}

				}
				else
				{
					temp_move_counter++;
				}
			}
			else
			{
				temp_diray_counter++;
			}

			return BehaviorStatus::Runnning;
		}
	);
	move->Name = "�ړ����s";


	// �V�[�P���T
	// ���s����܂Ŏ��s����m�[�h
	// �ǂꂩ��ł����s����܂Ŏq�̃m�[�h�����s
	Sequencer *is_attack = new Sequencer;	// �U����Ԃ����ʂ���ׂ̃m�[�h
	is_attack->Name = "�U��";
	is_attack->NeedsConditionalAbort = true;	// �Ē���
	is_attack->AddNode(front_check);	// �őO��`�F�b�N
	is_attack->AddNode(pawn_attack);	// �΂߂ɍU��


	// �R���f�B�V�����m�[�h
	// �V�[�P���T��Z���N�^�ŏ���������s�����߂̃m�[�h
	// �����_�����g�p�ł��邽�߁A���G�Ȕ�����\
	ConditionalNode *alive_check = new ConditionalNode // HP�`�F�b�J�[
	(
		[&](GameObject *owner)
		{
			return hitpoint_ > 0 ?	// HP��0���傫����ΐ���
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	alive_check->Name = "�����t���O�m�F";


	// �Z���N�^�[
	// ��������܂Ŏ��s����m�[�h
	// �ǂꂩ��ł���������܂Ŏq�̃m�[�h�����s
	Selector *state_check = new Selector;	// ��Ԋm�F�p�m�[�h
	state_check->Name = "��Ԋm�F";
	state_check->AddNode(is_attack);
	state_check->AddNode(move);

	// �V�[�P���T
	// ���s����܂Ŏ��s����m�[�h
	// �ǂꂩ��ł����s����܂Ŏq�̃m�[�h�����s
	Sequencer *is_alive = new Sequencer;
	is_alive->Name = "�����m�F";
	is_alive->NeedsConditionalAbort = true;	// �Ē���
	is_alive->AddNode(alive_check);	// �����t���O�m�F
	is_alive->AddNode(state_check);	// ��Ԋm�F

	// �Z���N�^�[
	// ��������܂Ŏ��s����m�[�h
	// �ǂꂩ��ł���������܂Ŏq�̃m�[�h�����s
	Selector *alive_state_checker = new Selector;	// �����m�F�p�m�[�h
	alive_state_checker->Name = "�����m�F";
	alive_state_checker->AddNode(is_alive);



	// ���s�[�^�[
	// �c���[�����[�v�ɑΉ������邽�߂̂���
	// �e�폈�������s�����ǂ������ʂ���
	Repeater *repeater = new Repeater();
	repeater->Name = "Repeater";
	repeater->AddNode(alive_state_checker);

	behavior_tree_->SetRootNode(repeater);	// ���s�[�^�[�����[�g�m�[�h�Ƃ��Đݒ�

}


RookEnemy::RookEnemy()
{
	// ���f���Z�b�g
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));

	// �c���[���s�J�n
	CreateTree();
	behavior_tree_->Start();


}

bool RookEnemy::Initialize()
{
	// ���W�Z�b�g
	m_Position = RandPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };

	// HP�Z�b�g
	hitpoint_ = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "hp")));
	max_hitpoint_ = hitpoint_;

	return false;
}

void RookEnemy::Draw()
{
	Obj_Draw();
}

void RookEnemy::UIDraw()
{
}


void RookEnemy::ImGuiDraw()
{
	ImGui::Begin("RookEnemy");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::Text("Width:%d", panel_position_.width);
	ImGui::Text("Height:%d", panel_position_.height);
	ImGui::End();
}

void RookEnemy::CreateTree()
{
	// �r�w�C�r�A�c���[�ݒ�f��
	// �|�[��(�X�v���b�h�V�[�g4p)�z��

	// �R���f�B�V�����m�[�h
	// �V�[�P���T��Z���N�^�ŏ���������s�����߂̃m�[�h
	// �����_�����g�p�ł��邽�߁A���G�Ȕ�����\

#pragma region �ːi�񐔃`�F�b�N(�R���f�B�V����)
	//�ːi�񐔂�3�񒴂������̃`�F�b�N
	ConditionalNode* tacklecount_check = new ConditionalNode
	(
		[&](GameObject* owner)
		{
			return tacklecount == 3 ?	// �^�b�N���񐔂�3�����Ă����OK
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	tacklecount_check->Name = "3��ːi�����ォ";
#pragma endregion

#pragma region �ǔ��ːi���s(�A�N�V�����m�[�h)
	// �A�N�V�����m�[�h
	// ���ۂɉ����̏������s���m�[�h
	// Ranning��Ԃ��ꍇ���s�r�����Ӗ����A�����𕪊�ł���
	ActionNode* tracking_tackle = new ActionNode	// �܂������ːi
	(
		[&](GameObject* owner)
		{
			// �A�j���[�V�����̑���̋^���J�E���^
			static int temp_counter = 0;

			static bool init_flag = false;
			// ��x�����A�j���[�V��������
			if (!init_flag)
			{
				// �U�����[�V�����̎��s��������

				// �U�����萶��������������

				init_flag = true;
				temp_counter = 0;
			}
			// �A�j���[�V�������I�������烉���_���ȍŌ��Ɉړ����ďI��
			if (temp_counter >= 60)
			{
				// �Ō���Ɉړ�
				panel_position_ =
				{
					7,
					Helper::GetInstance()->GetRanNum(0, 3)
				};
				m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);
				init_flag = false;
				return BehaviorStatus::Sucess;
			}

			temp_counter++;

			return BehaviorStatus::Runnning;
		}
	);
	tracking_tackle->Name = "�ǔ��ːi";

#pragma endregion

#pragma region �ǔ��ːi(�V�[�P���T)
	// �V�[�P���T
	// ���s����܂Ŏ��s����m�[�h
	// �ǂꂩ��ł����s����܂Ŏq�̃m�[�h�����s
	Sequencer* is_tracking_tackle = new Sequencer;	// �U����Ԃ����ʂ���ׂ̃m�[�h
	is_tracking_tackle->Name = "�ǔ��ːi";
	is_tracking_tackle->NeedsConditionalAbort = true;	// �Ē���
	is_tracking_tackle->AddNode(tacklecount_check);	//3��ːi�������ǂ���
	is_tracking_tackle->AddNode(tracking_tackle);	// �ǔ��ːi
#pragma endregion

#pragma region �ʏ�ːi���s(�A�N�V�����m�[�h)
	// �A�N�V�����m�[�h
	// ���ۂɉ����̏������s���m�[�h
	// Ranning��Ԃ��ꍇ���s�r�����Ӗ����A�����𕪊�ł���
	ActionNode* normal_tackle = new ActionNode	// �܂������ːi
	(
		[&](GameObject* owner)
		{
			// �A�j���[�V�����̑���̋^���J�E���^
			static int temp_counter = 0;

			static bool init_flag = false;
			// ��x�����A�j���[�V��������
			if (!init_flag)
			{
				// �U�����[�V�����̎��s��������

				// �U�����萶��������������

				init_flag = true;
				temp_counter = 0;
			}
			// �A�j���[�V�������I�������烉���_���ȍŌ��Ɉړ����ďI��
			if (temp_counter >= 60)
			{
				// �Ō���Ɉړ�
				panel_position_ =
				{
					7,
					Helper::GetInstance()->GetRanNum(0, 3)
				};
				m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);
				init_flag = false;
				return BehaviorStatus::Sucess;
			}

			temp_counter++;

			return BehaviorStatus::Runnning;
		}
	);
	normal_tackle->Name = "�ʏ�ːi";

#pragma endregion

#pragma region �ʏ�ːi(�V�[�P���T)
	// �V�[�P���T
	// ���s����܂Ŏ��s����m�[�h
	// �ǂꂩ��ł����s����܂Ŏq�̃m�[�h�����s
	Sequencer* is_normal_tackle = new Sequencer;	// �U����Ԃ����ʂ���ׂ̃m�[�h
	is_normal_tackle->Name = "�ʏ�ːi";
	is_normal_tackle->NeedsConditionalAbort = true;	// �Ē���
	//is_normal_tackle->AddNode(waitingcount_check);	//�U���\���ǂ���
	is_normal_tackle->AddNode(normal_tackle);	// �ʏ�ːi
#pragma endregion

#pragma region �U����Ԋm�F(�R���f�B�V����)
	//�U���\���ǂ���
	ConditionalNode* waitingcount_check = new ConditionalNode
	(
		[&](GameObject* owner)
		{
			return waitingcount == 60 ?	//�҂����Ԃ�1�b�����Ă����ok
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	waitingcount_check->Name = "�U���\���ǂ���";
#pragma endregion

#pragma region �U����ފm�F(�Z���N�^�[)
	// �Z���N�^�[
	// ��������܂Ŏ��s����m�[�h
	// �ǂꂩ��ł���������܂Ŏq�̃m�[�h�����s
	Selector* attacktype_check = new Selector;	// �U����ފm�F�p�m�[�h
	attacktype_check->Name = "�U����ފm�F";
	attacktype_check->AddNode(is_tracking_tackle);
	attacktype_check->AddNode(is_normal_tackle);
#pragma endregion

#pragma region �U��(�V�[�P���T)
	// �V�[�P���T
	// ���s����܂Ŏ��s����m�[�h
	// �ǂꂩ��ł����s����܂Ŏq�̃m�[�h�����s
	Sequencer* is_attack = new Sequencer;	// �U����Ԃ����ʂ���ׂ̃m�[�h
	is_attack->Name = "�U��";
	is_attack->NeedsConditionalAbort = true;	// �Ē���
	is_attack->AddNode(waitingcount_check);	//�U���\���ǂ���
	is_attack->AddNode(attacktype_check);	//�U����ފm�F
#pragma endregion

#pragma region �ړ����s(�A�N�V�����m�[�h)
	// �A�N�V�����m�[�h
	// ���ۂɉ����̏������s���m�[�h
	// Ranning��Ԃ��ꍇ���s�r�����Ӗ����A�����𕪊�ł���
	ActionNode* move = new ActionNode	// �ړ�
	(
		[&](GameObject* owner)
		{
			// �^���J�E���^
			static int temp_diray_counter = 0;	// �ړ�����̑ҋ@�p�J�E���^ 
			static int temp_move_counter = 0;	// �ړ��Ԋu�p�J�E���^

			static bool init_flag = false;			// �^��������
			// ��x�����A�j���[�V��������
			if (!init_flag)
			{
				// �U�����[�V�����̎��s��������

				// �U�����萶��������������

				temp_diray_counter = 0;
				temp_move_counter = 0;
				init_flag = true;
			}
			// �f�B���C���I��������ړ��J�n
			// �i���ۂɂ̓f�B���C�p�Ƀm�[�h�𕪂��������������A�������񂱂̂܂܎����j
			if (temp_diray_counter >= 90)
			{
				if (temp_move_counter >= 30)
				{
					temp_move_counter = 0;

					// �Ō���Ɉړ�
					panel_position_ =
					{
						panel_position_.width - 1,
						panel_position_.height,
					};
					m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);

					if (panel_position_.width == 4)
					{
						init_flag = false;
						return BehaviorStatus::Sucess;
					}

				}
				else
				{
					temp_move_counter++;
				}
			}
			else
			{
				temp_diray_counter++;
			}

			return BehaviorStatus::Runnning;
		}
	);
	move->Name = "�ړ����s";

#pragma endregion

#pragma region �����t���O�m�F(�R���f�B�V����)
	// �R���f�B�V�����m�[�h
	// �V�[�P���T��Z���N�^�ŏ���������s�����߂̃m�[�h
	// �����_�����g�p�ł��邽�߁A���G�Ȕ�����\
	ConditionalNode* alive_check = new ConditionalNode // HP�`�F�b�J�[
	(
		[&](GameObject* owner)
		{
			return hitpoint_ > 0 ?	// HP��0���傫����ΐ���
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	alive_check->Name = "�����t���O�m�F";
#pragma endregion

#pragma region ��Ԋm�F(�Z���N�^�[)
	// �Z���N�^�[
	// ��������܂Ŏ��s����m�[�h
	// �ǂꂩ��ł���������܂Ŏq�̃m�[�h�����s
	Selector* state_check = new Selector;	// ��Ԋm�F�p�m�[�h
	state_check->Name = "��Ԋm�F";
	state_check->AddNode(is_attack);
	state_check->AddNode(move);
#pragma endregion

#pragma region ����(�V�[�P���T)
	// �V�[�P���T
	// ���s����܂Ŏ��s����m�[�h
	// �ǂꂩ��ł����s����܂Ŏq�̃m�[�h�����s
	Sequencer* is_alive = new Sequencer;
	is_alive->Name = "����";
	is_alive->NeedsConditionalAbort = true;	// �Ē���
	is_alive->AddNode(alive_check);	// �����t���O�m�F
	is_alive->AddNode(state_check);	// ��Ԋm�F
#pragma endregion

#pragma region �����m�F(�Z���N�^)
	// �Z���N�^�[
	// ��������܂Ŏ��s����m�[�h
	// �ǂꂩ��ł���������܂Ŏq�̃m�[�h�����s
	Selector* alive_state_checker = new Selector;	// �����m�F�p�m�[�h
	alive_state_checker->Name = "�����m�F";
	alive_state_checker->AddNode(is_alive);
#pragma endregion


	// ���s�[�^�[
	// �c���[�����[�v�ɑΉ������邽�߂̂���
	// �e�폈�������s�����ǂ������ʂ���
	Repeater* repeater = new Repeater();
	repeater->Name = "Repeater";
	repeater->AddNode(alive_state_checker);

	behavior_tree_->SetRootNode(repeater);	// ���s�[�^�[�����[�g�m�[�h�Ƃ��Đݒ�

}