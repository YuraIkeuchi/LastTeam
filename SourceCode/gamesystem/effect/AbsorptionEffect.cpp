#include "AbsorptionEffect.h"
#include "ImageManager.h"
#include "Helper.h"
#include "Easing.h"
#include <random>
#define WindowW 1920
#define WindowH 1020
//読み込み
AbsorptionEffect::AbsorptionEffect() {
	tex.reset(IKETexture::Create(ImageManager::ABSEFFECT, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	tex->TextureCreate();
	tex->SetRotation({ 0,0,0 });
	tex->SetScale(m_Scale);
	tex->SetIsBillboard(true);
}
//初期化
void AbsorptionEffect::Initialize() {

	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandScale(1, 2);
	uniform_int_distribution<int> l_RandSpeed(100, 360);

	m_CircleSpeed = float(l_RandSpeed(mt));
	m_CircleScale = float(l_RandScale(mt));
	m_Position = Helper::CircleMove({ m_BasePos.x,m_BasePos.y + 1.0f,m_BasePos.z }, m_CircleScale, m_CircleSpeed);

	m_Alive = true;
}
//更新
void AbsorptionEffect::Update() {
	//パーティクル動く
	MoveEffect();
}
//描画
void AbsorptionEffect::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AddBlendType);
	if (m_Alive) {
		tex->Draw();
	}
	IKETexture::PostDraw();
}
//ImGui
void AbsorptionEffect::ImGuiDraw() {
	
}

//吸収時エフェクト
void AbsorptionEffect::MoveEffect() {
	//円運動で消える
	if (m_Alive) {
		m_CircleSpeed += 10.0f;

		if (m_Frame < m_FrameMax) {
			m_Frame += m_AddFrame;
		}
		else {
			m_Frame = {};
			m_Alive = false;
		}

		m_CircleScale = Ease(In, Cubic, m_Frame, m_CircleScale, 0.0f);

		m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,0.0f),
			Ease(In,Cubic,m_Frame,m_Scale.y,0.0f),
			Ease(In,Cubic,m_Frame,m_Scale.z,0.0f),
		};
	}
	m_Position = Helper::CircleMove({ m_BasePos.x,m_BasePos.y + 1.0f,m_BasePos.z }, m_CircleScale, m_CircleSpeed);
	tex->SetPosition(m_Position);
	tex->SetScale(m_Scale);
	tex->SetColor(m_Color);
	tex->Update();
}