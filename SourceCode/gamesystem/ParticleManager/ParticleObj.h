#pragma once
#include "IKEModel.h"
#include "IKEObject3d.h"

#include <array>   
#include <memory>
using namespace std;         //  名前空間指定
//OBJのパーティクル
class ParticleObj {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	ParticleObj();
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(const XMFLOAT3& StartPos, int Timer, int TargetTimer, int ParticleType);

	//各パーティクルの動き
	void NormalParticle(const XMFLOAT3& StartPos, int Timer, int TargetTimer);

	void WideParticle(const XMFLOAT3& StartPos, int Timer, int TargetTimer);

	void UpParticle(const XMFLOAT3& StartPos, int Timer, int TargetTimer);

	void EndParticle(const XMFLOAT3& StartPos, int Timer, int TargetTimer);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void ImGuiDraw();
private:
	//定数
	static const int Particle_Max = 40;//パーティクルの数
public:
	void SetStartColor(const XMFLOAT4& m_color) { this->m_StartColor = m_color; }
	void SetParticleType(int m_ParticleType) { this->m_ParticleType = m_ParticleType; }
	void SetParticleBreak(bool m_ParticleBreak) { this->m_ParticleBreak = m_ParticleBreak; }
	void SetStartScale(float m_StartScale) { this->m_StartScale = m_StartScale; }
	void SetAddScale(float  m_AddScale) { this->m_AddScale = m_AddScale; }
	void SetEndCount(int m_EndCount) { this->m_EndCount = m_EndCount; }
private:
	//モデルやOBJ
	IKEModel* model = nullptr;
	array<unique_ptr <IKEObject3d>, Particle_Max> object;

	//変数
	array<XMFLOAT3, Particle_Max> m_pos;//座標
	array<float, Particle_Max> m_Angle;//角度
	array<XMFLOAT4, Particle_Max> m_color;//色
	array<XMFLOAT2, Particle_Max> m_speed;//移動速度
	array<XMFLOAT3, Particle_Max> m_scale;//大きさ
	array<XMFLOAT3, Particle_Max> m_AddPower;//加わる力
	array<float, Particle_Max> m_power;
	XMFLOAT4 m_StartColor = { 1.0f,1.0f,1.0f,1.0f };//始まりの色


	array<bool, Particle_Max> m_Alive;//生存しているか
	array<bool, Particle_Max> m_End;//生存しているか
	array<bool, Particle_Max> m_ScaleChange;//大きさの変更
	array<int, Particle_Max> m_Number;//パーティクルの動く方向
	array<int, Particle_Max> m_Timer;//パーティクルの時間
	XMFLOAT3 m_StartPos = { 0.0f,0.0f,0.0f };

	//各パーティクルの数
	//unique_ptr<GamePlayer> player;
	int m_NormalParticleCount = 60;
	int m_WideParticleCount = 10;
	int m_ParticleType = 0;
	int m_EndCount = 0;
	bool m_ParticleBreak = false;
	float m_StartScale = 0.1f;
	float m_AddScale = 0.02f;
	enum Type {
		Normal,
		Up,
		End,
	};
};