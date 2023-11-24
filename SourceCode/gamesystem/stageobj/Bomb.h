#pragma once
#include"InterEnemy.h"

using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class Bomb :public InterEnemy {
public:
	Bomb();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(Bomb::* stateTable[])();

private:
	void Inter();//�ҋ@
	void Attack();//�U��
	void ShockWave();
private:
	unique_ptr<IKETexture> shockWaveTex;
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	bool _isPlayerDamage = false;
	bool _isEnemyDamage = false;
	XMFLOAT4 m_shockWaveColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT3 m_shockWaveScale = { 0.0f,0.0f,0.0f };
	int m_shockWaveTimer = 0;
};
