#pragma once
#include"DirectXCommon.h"
#include <DirectXMath.h>
#include "IKETexture.h"
#include "ObjCommon.h"
#include "Player.h"
using namespace std;         //  名前空間指定

//毒エリアクラス
class PoisonArea : public ObjCommon{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	PoisonArea();
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
	XMFLOAT3 SetPanelPos(const int width, const int height);
public:
	//gettersetter
	const bool GetAlive() { return panels.Alive; }
	void SetPlayer(Player* player) { this->player = player; }
private:
	
	Player* player;
	//パネル
	struct Panel {
		unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool Alive = false;
		int Width = {};
		int Height = {};
		int Timer = {};
		float scale = {};
		float afterscale = 0.15f;
		float frame = {};
		int DamageTimer = 19;
		bool Damage = false;
	};

	Panel panels;

	enum PoisonState{
		POISON_THROW,
		POISON_DROP,
		POISON_WIDE,
		POISON_END,
	}_PoisonState = POISON_THROW;
	bool m_BulletAlive = false;

	float m_Damage = {};
	int m_TargetTimer = {};
};