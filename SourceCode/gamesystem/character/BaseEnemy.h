#pragma once

#include <memory>

#include "GameObject/GameObject.h"
#include "ObjCommon.h"
#include "BehaviorTree/BehaviorTree.h"


using namespace behaviorTree;

enum class Debuff
{
	kNone = 0,
	kPoison = 1 << 1,
};

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
	void Draw() override;
	void Draw(DirectXCommon *dxCommon) override {};
	//UI�p
	void UIDraw() override;
	//ImGui
	void ImGuiDraw() override;

	XMFLOAT3 SetPannelPos(int width, int height);

	const float GetHP() { return hitpoint_; }

	bool GetAlive() { return is_alive_; }


	void SetPoizonLong(bool isPoison) { m_PoisonLong = isPoison; }
	void SetPoizonVenom(bool isPoison) { m_IsVenom = isPoison; }
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
	bool is_alive_{ false };	// �����m�F

	// �r�w�C�r�A�c���[
	std::unique_ptr<SimpleBehaviorTree> behavior_tree_;

	bool m_PoisonLong = false;
	bool m_IsVenom = false;
	int m_PoisonTimer = {};

	// �f�o�t
	int debuff_ { static_cast<int>(Debuff::kNone)};


	// �|�C�Y���p�[�e�B�N��
	void BirthPoisonParticle();
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
	//�`��
	void Draw() override;
	void Draw(DirectXCommon *dxCommon) override {};
	//UI�p
	void UIDraw() override;
	//ImGui
	void ImGuiDraw() override;

protected:
	void CreateTree()override;
};