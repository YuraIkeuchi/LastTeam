#pragma once
#include"InterEnemy.h"
#include<list>
#include<memory>
#include "CsvLoader.h"
#include "Player.h"
#include "LightGroup.h"
class EnemyManager {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	EnemyManager();
	~EnemyManager();

	void Initialize();
	void Update();
	void Draw(DirectXCommon* dxCommon);

	void SetCount();

	void ImGuiDraw();
	//UI�̕`��
	void UIDraw();

	//�G�̎��S����
	bool BossDestroy();
	/// <summary>
	/// �p�b�V�u������Γŉ���(�o�g���V�[��)
	/// </summary>
	void PoizonGauge();
	/// <summary>
	/// �p�b�V�u������Ζғ�(�o�g���V�[��)
	/// </summary>
	void PoizonVenom();
	/// <summary>
	/// �p�b�V�u������΃h���C���㏸(�o�g���V�[��)
	/// </summary>
	void DrainHealUp();
	/// <summary>
	/// 
	/// </summary>
	void ReLoadDamage();
private:
	/// <summary>
	/// csv�Ń}�b�v�ɓG���X�|�[��
	/// </summary>
	void Spawn2Map();
public:
	static void SetPlayer(Player* player) { EnemyManager::player = player; }

private:
	static Player* player;
	std::vector<unique_ptr<InterEnemy>> enemys;
	//�ۉe(�{�X)
	float BosscircleShadowDir[3] = { 0,-1,0 };
	float BosscircleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float BosscircleShadowFactorAngle[2] = { 0.0f, 0.3f };
};

