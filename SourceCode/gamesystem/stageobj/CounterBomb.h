#pragma once
#include"DirectXCommon.h"
#include <DirectXMath.h>
#include "IKETexture.h"
#include "ObjCommon.h"
#include "Player.h"
using namespace std;         //  名前空間指定

//毒エリアクラス
class CounterBomb : public ObjCommon {
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	CounterBomb();
public:
	//初期化
	bool Initialize() override;
	//ステータス初期化
	void InitState(const int width, const int height);
	//更新
	void Update() override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;
	//ImGui
	void ImGuiDraw();
private:
	void Move();
	void Collide();
	void BirthExplosionParticle();//パーティクル
	XMFLOAT3 SetPanelPos(const int width, const int height);
	void BirthPredict(const int Width, const int Height);//予測エリア
public:
	//gettersetter
	void SetPlayer(Player* player) { this->player = player; }
	const bool GetAlive() { return m_Alive; }
private:
	Player* player;
	XMFLOAT3 m_AfterPos = {};
	int m_NowWidth = {};
	int m_NowHeight = {};
	enum BombState {
		BOMB_THROW,
		BOMB_DROP,
		BOMB_DELETE
	}_BombState = BOMB_THROW;

	bool m_Alive = false;
	
	float m_Damage = {};
	int m_DeathTimer = {};
	int m_TargetTimer = {};
	bool m_Hit = false;
};