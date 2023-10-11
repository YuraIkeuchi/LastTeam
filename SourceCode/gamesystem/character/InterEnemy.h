#pragma once
#include "ObjCommon.h"
#include <memory>
#include <IKESprite.h>

using namespace std;         //  ���O��Ԏw��
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
	unique_ptr<IKESprite> hptex;
	//�L�����̏��
	enum CharaState {
		STATE_INTER,
		STATE_ATTACK,
	};
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
	float m_Radius = 1.0f;

	//HP��UI�Ɏg���ϐ�
	XMFLOAT2 m_HPPos = { 1000.0f,50.0f };
	XMFLOAT2 m_HPSize = {100.0f,30.0f};
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};
public://getter setter
	void SetState(int state) {_charaState = state;}
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

	//�J�����̃f�[�^���Q�b�g����
	void GetData(const XMMATRIX& matView, const XMMATRIX& matProjection, const XMMATRIX& matPort);
private:
	void BirthParticle();
	//HP�̊��������߂�
	float HpPercent();
	//�X�v���C�g��G���W�ɏo��
	void WorldDivision();
protected:
	void Collide();
};