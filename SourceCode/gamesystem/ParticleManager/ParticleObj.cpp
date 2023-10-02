#include "ParticleObj.h"
#include "ModelManager.h"
#include "imgui.h"
#include <random>
//読み込み
ParticleObj::ParticleObj() {
	model = ModelManager::GetInstance()->GetModel(ModelManager::Particle);
}
//初期化
void ParticleObj::Initialize() {
	IKEObject3d* object_[Particle_Max];
	for (int i = 0; i < object.size(); i++) {
		object_[i] = new IKEObject3d();
		object_[i] = IKEObject3d::Create();
		object_[i]->SetModel(model);
		object_[i]->SetColor(m_StartColor);
		object[i].reset(object_[i]);
		m_Alive[i] = false;
		m_scale[i] = { 0.0f,0.0f,0.0f };
	}
}
//更新
void ParticleObj::Update(const XMFLOAT3& StartPos, int Timer, int TargetTimer, int ParticleType) {
	//パーティクルのタイプを指定する
	if (ParticleType == Normal) {
		NormalParticle(StartPos, Timer, TargetTimer);
	}
	else if (ParticleType == Up) {
		UpParticle(StartPos, Timer, TargetTimer);
	}
	else if (ParticleType == End) {
		EndParticle(StartPos, Timer, TargetTimer);
	}

	for (int i = 0; i < m_NormalParticleCount; i++) {
		object[i]->SetPosition(m_pos[i]);
		object[i]->SetScale(m_scale[i]);
		object[i]->SetColor(m_StartColor);
		if (m_Alive[i]) {
			object[i]->Update();
		}
	}
}
//描画
void ParticleObj::Draw() {
	IKEObject3d::PreDraw();
	for (int i = 0; i < m_NormalParticleCount; i++) {
		if (m_Alive[i]) {
			object[i]->Draw();
		}
	}
}
//ImGuiの描画
void ParticleObj::ImGuiDraw() {
}

//普通のパーティクル
void ParticleObj::NormalParticle(const XMFLOAT3& StartPos, int Timer, int TargetTimer) {
	m_StartPos = StartPos;
	//フレーム数が目標を超えたら出現する
	if (Timer >= TargetTimer) {
		for (int i = 0; i < m_NormalParticleCount; i++) {
			//飛ばす方向をランダムで決める
			if (!m_Alive[i]) {
				//乱数生成
				mt19937 mt{ std::random_device{}() };
				uniform_int_distribution<int> l_angledist(0, 360);
				uniform_int_distribution<int> l_Numberdist(0, 1);
				m_Angle[i] = float(l_angledist(mt));
				m_speed[i] = { 0.02f,0.02f };
				m_Number[i] = l_Numberdist(mt);
				m_pos[i].x = m_StartPos.x;
				m_pos[i].y = m_StartPos.y;
				m_pos[i].z = m_StartPos.z;
				m_AddPower[i].y = 0.0f;
				m_scale[i] = { m_StartScale,m_StartScale,m_StartScale };
				m_Alive[i] = true;
				m_ScaleChange[i] = false;
				if (m_ParticleBreak) {
					break;
				}
			}
		}
	}
	//実際に動く
	for (int i = 0; i < m_NormalParticleCount; i++) {
		if (m_Alive[i]) {
			m_AddScale = 0.02f;
			m_pos[i].x += (cos(m_Angle[i]) * m_speed[i].x);
			m_pos[i].y += (sin(m_Angle[i]) * m_speed[i].y);
			if (!m_ScaleChange[i]) {
				m_scale[i].x += m_AddScale;
				m_scale[i].y += m_AddScale;
				m_scale[i].z += m_AddScale;

				if (m_scale[i].x >= m_StartScale + 0.5f) {
					m_ScaleChange[i] = true;
				}
			}
			else {
				m_scale[i].x -= m_AddScale;
				m_scale[i].y -= m_AddScale;
				m_scale[i].z -= m_AddScale;

				if (m_scale[i].x <= 0.0f) {
					m_Alive[i] = false;
				}
			}
		}
	}
}
//広がるようなパーティくる
void ParticleObj::WideParticle(const XMFLOAT3& StartPos, int Timer, int TargetTimer) {
	m_StartPos = StartPos;
	//フレーム数が目標を超えたら出現する
	if (Timer >= TargetTimer) {
		for (int i = 0; i < m_WideParticleCount; i++) {
			//飛ばす方向をランダムで決める
			if (!m_Alive[i] && !m_End[i]) {
				//乱数生成
				mt19937 mt{ std::random_device{}() };
				uniform_int_distribution<int> l_angledist(0, 360);
				uniform_int_distribution<int> l_Numberdist(0, 1);
				m_Angle[i] = float(l_angledist(mt));
				m_speed[i] = { 0.02f,0.02f };
				m_Number[i] = l_Numberdist(mt);
				m_pos[i].x = m_StartPos.x;
				m_pos[i].y = m_StartPos.y;
				m_pos[i].z = m_StartPos.z;
				if (m_Number[i] == 0) {
					m_power[i] = 0.2f;
				}
				else {
					m_power[i] = -0.2f;
				}
				m_AddPower[i].y = 0.0f;
				m_scale[i] = { 0.1f,0.1f,0.1f };
				m_Alive[i] = true;
				m_ScaleChange[i] = false;
				break;
			}
		}
	}
	//実際に動く
	for (int i = 0; i < m_WideParticleCount; i++) {
		if (m_Alive[i]) {
			if (m_Number[i] == 0) {
				if (m_power[i] > 0.0f) {
					m_power[i] -= 0.025f;
				}
				else {
					m_power[i] = 0.0f;
				}
			}
			else {
				if (m_power[i] < 0.0f) {
					m_power[i] += 0.025f;
				}
				else {
					m_power[i] = 0.0f;
				}
			}
			m_pos[i].x += (cos(m_Angle[i]) * m_speed[i].x + m_power[i]);
			m_pos[i].y += (sin(m_Angle[i]) * m_speed[i].y);
			if (!m_ScaleChange[i]) {
				m_scale[i].x += 0.02f;
				m_scale[i].y += 0.02f;
				m_scale[i].z += 0.02f;

				if (m_scale[i].x >= 0.6f) {
					m_ScaleChange[i] = true;
				}
			}
			else {
				m_scale[i].x -= 0.02f;
				m_scale[i].y -= 0.02f;
				m_scale[i].z -= 0.02f;

				if (m_scale[i].x <= 0.0f) {
					m_Alive[i] = false;
					m_End[i] = true;
				}
			}
		}
	}

	//すべてのパーティクルが消えたあとまた全てをセットする
	if (m_EndCount == 1) {
		m_EndCount = 0;
		for (int i = 0; i < m_WideParticleCount; i++) {
			m_End[i] = false;
		
		}
	}
}
//上に上がるパーティクル(主に上方向
void ParticleObj::UpParticle(const XMFLOAT3& StartPos, int Timer, int TargetTimer) {
	m_StartPos = StartPos;
	//フレーム数が目標を超えたら出現する
	if (Timer >= TargetTimer) {
		for (int i = 0; i < m_NormalParticleCount; i++) {
			//飛ばす方向をランダムで決める
			if (!m_Alive[i]) {
				//乱数生成
				mt19937 mt{ std::random_device{}() };
				uniform_int_distribution<int> l_Numberdist(0, 1);
				m_speed[i] = { 0.1f,0.02f };
				m_Number[i] = l_Numberdist(mt);
				m_pos[i].x = m_StartPos.x;
				m_pos[i].y = m_StartPos.y;
				m_pos[i].z = m_StartPos.z;
				m_AddPower[i].y = 0.0f;
				m_scale[i] = { m_StartScale,m_StartScale,m_StartScale };
				m_Alive[i] = true;
				m_ScaleChange[i] = false;
				if (m_ParticleBreak) {
					break;
				}
			}
		}
	}

	for (int i = 0; i < m_NormalParticleCount; i++) {
		//実際に動く
		if (m_Alive[i]) {
			m_Timer[i]++;
			//乱数生成
			mt19937 mt{ std::random_device{}() };
			uniform_int_distribution<int> l_Powerdist(1, 2);
			if (m_Timer[i] % 30 == 0) {
				uniform_int_distribution<int> l_angledist(0, 360);
				m_Angle[i] = float(l_angledist(mt));
			}
			m_pos[i].x += (cos(m_Angle[i]) * m_speed[i].x);
			m_AddPower[i].y = float(l_Powerdist(mt));
			m_AddPower[i].y = m_AddPower[i].y / 10;
			m_pos[i].y += m_AddPower[i].y;
			m_AddScale = 0.005f;
			if (!m_ScaleChange[i]) {
				m_scale[i].x += m_AddScale;
				m_scale[i].y += m_AddScale;
				m_scale[i].z += m_AddScale;

				if (m_scale[i].x >= m_StartScale + 1.5f) {
					m_ScaleChange[i] = true;
				}
			}
			else {
				m_scale[i].x -= m_AddScale;
				m_scale[i].y -= m_AddScale;
				m_scale[i].z -= m_AddScale;

				if (m_scale[i].x <= 0.0f) {
					m_Alive[i] = false;
				}
			}
		}
	}

}
//敵をたおした時のパーティクル
void ParticleObj::EndParticle(const XMFLOAT3& StartPos, int Timer, int TargetTimer) {
	m_StartPos = StartPos;
	if (Timer >= TargetTimer) {
		for (int i = 0; i < m_NormalParticleCount; i++) {
			if (!m_Alive[i]) {
				//乱数生成
				mt19937 mt{ std::random_device{}() };
				uniform_int_distribution<int> l_angledist(0, 360);
				uniform_int_distribution<int> l_Numberdist(0, 1);
				m_Angle[i] = float(l_angledist(mt));
				m_speed[i] = { 0.3f,0.3f };
				m_Number[i] = l_Numberdist(mt);
				m_pos[i].x = m_StartPos.x;
				m_pos[i].y = m_StartPos.y;
				m_pos[i].z = m_StartPos.z;
				m_AddPower[i].y = 0.0f;
				m_scale[i] = { 0.5f,0.5f,0.5f };
				m_Alive[i] = true;
				m_ScaleChange[i] = false;
				if (m_ParticleBreak) {
					break;
				}
			}
		}
	}

	for (int i = 0; i < m_NormalParticleCount; i++) {
		if (m_Alive[i]) {
			m_AddScale = 0.03f;
			m_pos[i].x += (cos(m_Angle[i]) * m_speed[i].x);
			m_pos[i].y += (sin(m_Angle[i]) * m_speed[i].y);

			if (m_speed[i].x > 0.0f) {
				m_speed[i].x -= 0.01f;
				m_speed[i].y -= 0.01f;
			}
			else {
				m_speed[i].x = 0.0f;
				m_speed[i].y = 0.0f;
			}
			if (!m_ScaleChange[i]) {
				m_scale[i].x += m_AddScale;
				m_scale[i].y += m_AddScale;
				m_scale[i].z += m_AddScale;

				if (m_scale[i].x >= m_StartScale + 1.0f) {
					m_ScaleChange[i] = true;
				}
			}
			else {
				m_scale[i].x -= m_AddScale;
				m_scale[i].y -= m_AddScale;
				m_scale[i].z -= m_AddScale;

				if (m_scale[i].x <= 0.0f) {
					m_Alive[i] = false;
				}
			}
		}
	}
}