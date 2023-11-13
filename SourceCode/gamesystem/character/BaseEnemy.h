#pragma once

#include <memory>

#include "GameObject/GameObject.h"
#include "ObjCommon.h"
#include "BehaviorTree/BehaviorTree.h"


using namespace behaviorTree;



class BaseEnemy:
	public ObjCommon,
	public GameObject
{
public:
	BaseEnemy();
	//������
	bool Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw(DirectXCommon* dxCommon) override;

	//UI�p
	void UIDraw() override;
	//ImGui
	void ImGuiDraw() override;

protected:

	XMFLOAT3 RandPanelPos();
	
	virtual void CreateTree() {};
	struct Position
	{
		int width = -1;
		int height = -1;
	}panel_position_;

	float hitpoint_{};		// �̗�
	float max_hitpoint_{};	// �ő�̗�

	// �r�w�C�r�A�c���[
	std::unique_ptr<SimpleBehaviorTree> behavior_tree_;


};

// �f�o�b�N�p�G�l�~�[�N���X�ł�
class TestEnemy :
	public BaseEnemy
{
public:
	// �R���X�g���N�^
	TestEnemy();

	//������
	bool Initialize();
	//UI�p
	void UIDraw() override;
	//ImGui
	void ImGuiDraw() override;

protected:
	void CreateTree()override;
};