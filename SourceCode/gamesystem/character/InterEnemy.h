#pragma once
#include "ObjCommon.h"


//�L�����̏��
enum CharaState {
	STATE_INTER,
	STATE_ATTACK,
	STATE_STANDBY,
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

	int _charaState = STATE_INTER;
	//�N�[���^�C��
	int coolTimer = 0;
	//�R�}���h�Ԋu
	int kIntervalMax = 120;
	//HP
	float m_HP = {};
	//�G�̃_���[�W����̃C���^�[�o��
	int m_DamegeTimer = {};
	//�����蔻��̔��a
	float m_Radius = 1.0f;

	struct PanelProb {
		int AttackProb = 50;
		int SkillProb = 25;
		int GuardProb = 25;

	};

public://getter setter
	void SetState(int state) { _charaState = state; }
	int GetState() { return _charaState; };
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

private:
	void BirthParticle();
protected:
	void Collide();
};