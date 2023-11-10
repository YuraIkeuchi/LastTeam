#pragma once
#include "ObjCommon.h"
#include "AttackArea.h"
#include "DrawNumber.h"
#include <memory>
#include <IKESprite.h>
#include <array>
using namespace std;         //  ���O��Ԏw��


//�L�����̏��
enum CharaState {
	STATE_INTER,
	STATE_ATTACK,
	STATE_SPECIAL,
};
//�G���
class InterEnemy :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	XMFLOAT3 randPanelPos();
protected:
	//�O���\���܂�
	static const int NUMBER_MAX = 3;

protected:
	array<unique_ptr<DrawNumber>, NUMBER_MAX> _drawnumber;

	//����
	enum DightType {
		FIRST_DIGHT,
		SECOND_DIGHT,
		THIRD_DIGHT
	};
	//HP�̕\��
	unique_ptr<IKESprite> hptex;
	
	int _charaState = STATE_INTER;
	//�N�[���^�C��
	int coolTimer = 0;
	//�R�}���h�Ԋu
	int kIntervalMax = 120;
	//HP
	float m_HP = {};
	float m_MaxHP = {};
	//�G�̃_���[�W����̃C���^�[�o��
	int m_DamegeTimer = {};
	//�����蔻��̔��a
	float m_Radius = 0.5f;

	struct PanelProb {
		int AttackProb = 50;
		int SkillProb = 25;
		int GuardProb = 25;
	};

	//���݂̃}�X�ԍ�
	int m_NowWidth = {};
	int m_NowHeight = {};

	//HP��UI�Ɏg���ϐ�
	XMFLOAT2 m_HPPos = { 1000.0f,50.0f };
	XMFLOAT2 m_HPSize = {100.0f,15.0f};
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};

	//���l������HP�\���̂��߂̕ϐ�
	array<int, NUMBER_MAX> m_DigitNumber;
	int m_InterHP = {};//�����ɂ���HP

	bool m_Poison = false;
	bool m_PoisonLong = false;
	bool m_IsVenom = false;
	int m_PoisonTimer = {};
public://getter setter
	void SetState(int state) { _charaState = state; }
	int GetState() { return _charaState; };
	
	void SetPoizonLong(bool isPoison) { m_PoisonLong = isPoison; }
	void SetPoizonVenom(bool isPoison) { m_IsVenom = isPoison; }

	const float GetHP() { return m_HP; }
public:
	//virtual ~InterEnemy() = default;
	/// <summary>
	/// ������
	/// </summary>
	virtual bool Initialize()override;
	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();

	virtual void Action() = 0;//�G�̍s��

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon)override;

	void ImGuiDraw();

	virtual void ImGui_Origin() = 0;

	void UIDraw();
	XMFLOAT3 SetPannelPos(int width, int height);
private:
	void BirthParticle();
	//HP�̊��������߂�
	float HpPercent();
	//�X�v���C�g��G���W�ɏo��
	void WorldDivision();
	//UI�̂��߂�HP�̊Ǘ�
	void HPManage();
	void BirthPoisonParticle();
protected:
	void Collide(vector<AttackArea*>area);
	//�ł̏��
	void PoisonState();
};