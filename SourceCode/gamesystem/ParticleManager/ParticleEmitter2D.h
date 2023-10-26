#pragma once
#include "ParticleManager2D.h"

using namespace std;         //  ���O��Ԏw��
/// <summary>
/// �p�[�e�B�N���G�~�b�^�[
/// </summary>
class ParticleEmitter2D
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�V���O���g����
	//�R���X�g���N�^���B��
	ParticleEmitter2D() = default;
	//�f�X�g���N�^���B��
	~ParticleEmitter2D() = default;
public: //�����o�֐�
	static ParticleEmitter2D* GetInstance();

	//������
	void Initialize();

	//�X�V
	void Update();

	void IntroDraw();
	//�`��
	void FlontDrawAll();

	//�G�t�F�N�g
	void FireEffect(const int life,const XMFLOAT2& pos,const float startscale, const float endscale,const XMFLOAT4& startcolor,const XMFLOAT4& endcolor);
	//�_���[�W�G�t�F�N�g
	void Break(const int life, const XMFLOAT2& pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const float Gravity, float divi);
	void AllDelete();
private: //�����o�֐�
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture();

private: //�����o�ϐ�
	//�p�[�e�B�N���}�l�[�W���[(��)
	unique_ptr<ParticleManager2D> circleParticle;
private:
	int m_Timer = 0;
};
