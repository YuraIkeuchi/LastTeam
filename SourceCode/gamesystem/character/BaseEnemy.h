#pragma once

#include <memory>

#include "GameObject/GameObject.h"
#include "ObjCommon.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DrawNumber.h"

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
	void Draw(DirectXCommon* dxCommon) override;

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

	// �����_���ȃp�l�����擾
	XMFLOAT3 RandPanelPos();
	
	//HP�̕\��
	unique_ptr<IKESprite> hptex_;
	static const int kNumberMax{ 3 };	// ����
	array<unique_ptr<DrawNumber>, kNumberMax> draw_number_;
	static const int kHpDrawNumMax{ 3 };
	array<int, kHpDrawNumMax> digit_number_;
	

	//����
	enum DightType {
		kFirstDight,
		kSecondDight,
		kThirdDight
	};
	virtual void CreateTree() {};
	struct Position
	{
		int width = -1;
		int height = -1;
	}panel_position_;

	float hitpoint_{};		// �̗�
	float max_hitpoint_{};	// �ő�̗�
	int inter_hitpoint_{};	// HP�i�����j
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

private:

	//HP��UI�Ɏg���ϐ�
	XMFLOAT2 m_HPPos = { 1000.0f,50.0f };
	XMFLOAT2 m_HPSize = { 100.0f,15.0f };
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};
	void WorldDivision();
	float HpPercent();
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