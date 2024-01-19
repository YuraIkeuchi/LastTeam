#pragma once
#include"DirectXCommon.h"
#include <DirectXMath.h>
#include "Player.h"

using namespace std;         //  名前空間指定

//トゲクラス
class Explosion{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Explosion();
public:
	//初期化
	bool Initialize();
	//ステータス初期化
	void InitState(const int width, const int height);
	//更新
	void Update();
	void ImGuiDraw();
private:
	XMFLOAT3 SetPanelPos(const int width, const int height);
	void Move();
	void Collide();
	void BirthParticle();
public:
	//gettersetter
	const bool GetAlive() { return m_Alive; }
	const float GetDamage() { return m_Damage; }

	void SetSound(const bool Sound) { m_Sound = Sound; }
	void SetHit(const bool Hit) { m_Hit = Hit; }
	void SetPlayer(Player* player) { this->player = player; }

private:
	Player* player;
	//生存関係
	bool m_Alive = false;
	int m_AliveTimer = {};

	//ヒットしたかどうか
	bool m_Hit = false;

	int m_NowWidth = {};
	int m_NowHeight = {};
	float m_Damage = 20.0f;
	bool m_Sound = false;
	XMFLOAT3 m_Position = {};
};