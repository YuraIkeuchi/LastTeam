#include "character/BaseEnemy.h"

#include "Helper.h"
#include "StagePanel.h"
#include "CsvLoader.h"
#include "GameStateManager.h"


BaseEnemy::BaseEnemy():
	GameObject("Enemy")
{
	// �r�w�C�r�A�c���[����
	behavior_tree_ = std::make_unique<behaviorTree::SimpleBehaviorTree>(this);	
}

bool BaseEnemy::Initialize()
{
	// �G�l�~�[�̃|�C���^���Q�[���X�e�[�g�}�l�[�W���Ɋi�[
	GameStateManager::GetInstance()->AddEnemy(std::static_pointer_cast<BaseEnemy>(shared_from_this()));
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
	BaseEnemy::Initialize();
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