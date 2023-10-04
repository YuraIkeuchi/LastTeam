#include "NormalEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
//ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
NormalEnemy::NormalEnemy() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
}
//‰Šú‰»
bool NormalEnemy::Initialize() {
	m_Position = { 4.5f, 1.0f, 4.5f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	return true;
}

void (NormalEnemy::* NormalEnemy::stateTable[])() = {
	&NormalEnemy::Inter,//“®‚«‚Ì‡ŠÔ
	&NormalEnemy::Follow,//’Ç]
	&NormalEnemy::Circle,//‰~‰^“®
	&NormalEnemy::MoveSin,//Sin”g
};

//s“®
void NormalEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
}
//•`‰æ
void NormalEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui•`‰æ
void NormalEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	if (ImGui::RadioButton("FOLLOW", &_charaState)) {
		_charaState = STATE_FOLLOW;
	}
	else if (ImGui::RadioButton("CIRCLE", &_charaState)) {
		m_StartPos = m_Position;
		m_CircleScale = 0.0f;
		m_CircleSpeed = 0.0f;
		_charaState = STATE_CIRCLE;
	}
	else if (ImGui::RadioButton("SIN", &_charaState)) {
		_charaState = STATE_SIN;
	}
	else if (ImGui::RadioButton("INTER", &_charaState)) {
		_charaState = STATE_INTER;
	}
	ImGui::End();
}
//ŠJ•ú
void NormalEnemy::Finalize() {

}
//’Ç]
void NormalEnemy::Follow() {
	Helper::GetInstance()->FollowMove(m_Position, Player::GetInstance()->GetPosition(), 0.1f);
}
//‰~‰^“®
void NormalEnemy::Circle() {

	m_CircleSpeed += 0.5f;
	if (m_CircleScale < 7.0f) {
		m_CircleScale += 0.3f;
	}

	m_AfterPos = Helper::GetInstance()->CircleMove(m_StartPos, m_CircleScale, m_CircleSpeed);

	m_Position = {
		Ease(In,Cubic,0.2f,m_Position.x,m_AfterPos.x),
		m_Position.y,
		Ease(In,Cubic,0.2f,m_Position.z,m_AfterPos.z),
	};
}
//
void NormalEnemy::MoveSin() {

}
void NormalEnemy::Inter() {

}